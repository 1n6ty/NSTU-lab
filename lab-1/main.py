import numpy as np
import pandas as pd
from pathlib import Path
from scipy.stats import fisher_exact


TARGET = "TARGET"

CATEGORICAL_COLS = [
    "NAME_CONTRACT_TYPE", "CODE_GENDER", "FLAG_OWN_CAR", "FLAG_OWN_REALTY",
    "NAME_TYPE_SUITE", "NAME_INCOME_TYPE", "NAME_EDUCATION_TYPE",
    "NAME_FAMILY_STATUS", "NAME_HOUSING_TYPE", "OCCUPATION_TYPE",
    "WEEKDAY_APPR_PROCESS_START", "ORGANIZATION_TYPE",
    "FONDKAPREMONT_MODE", "HOUSETYPE_MODE", "WALLSMATERIAL_MODE",
    "EMERGENCYSTATE_MODE",
]
MIN_LEVELS = 5                                # "large number of levels"
THRESHOLDS = [0.005, 0.01, 0.02, 0.05]        # rare = relative frequency < threshold
N_BOOT = 1000
N_PERM = 1000
ALPHA = 0.05
MISSING = "__MISSING__"


# CORE METRICS

def encode(series: pd.Series):
    """Integer codes; NaN becomes its own level (missingness may carry signal)."""
    s = series.astype(object).where(series.notna(), MISSING)
    codes, uniques = pd.factorize(s)
    return codes.astype(np.int64), len(uniques)


def symmetric_uncertainty(x: np.ndarray, y: np.ndarray, kx: int, ky: int) -> float:
    """SU(X, Y) = 2 * MI(X, Y) / (H(X) + H(Y)), entropies in bits."""
    n = len(x)
    if n == 0:
        return 0.0
    joint = np.bincount(x * ky + y, minlength=kx * ky).reshape(kx, ky) / n
    px, py = joint.sum(axis=1), joint.sum(axis=0)
    h_x = -np.sum(px[px > 0] * np.log2(px[px > 0]))
    h_y = -np.sum(py[py > 0] * np.log2(py[py > 0]))
    if h_x == 0 or h_y == 0:
        return 0.0
    nz = joint > 0
    mi = np.sum(joint[nz] * np.log2(joint[nz] / np.outer(px, py)[nz]))
    return 2.0 * mi / (h_x + h_y)


# RARE-LEVEL TRANSFORMATIONS

def rare_mask(x: np.ndarray, k: int, thr: float) -> np.ndarray:
    """Boolean per level: True if the level is rare (freq < thr) in this sample."""
    return np.bincount(x, minlength=k) / len(x) < thr


def su_drop(x, y, k, ky, thr):
    """Variant A: delete rows that belong to rare levels."""
    keep = ~rare_mask(x, k, thr)[x]
    return symmetric_uncertainty(x[keep], y[keep], k, ky)


def su_merge(x, y, k, ky, thr):
    """Variant B: keep all rows, merge rare levels into one level OTHER."""
    x2 = np.where(rare_mask(x, k, thr)[x], k, x)
    return symmetric_uncertainty(x2, y, k + 1, ky)


def ci_and_sig(diffs):
    lo = np.percentile(diffs, ALPHA / 2 * 100)
    hi = np.percentile(diffs, (1 - ALPHA / 2) * 100)
    return lo, hi, (lo > 0) or (hi < 0)


# STATISTICAL TESTS

def permutation_test_su(x, y, k, ky, rng):
    """H0: SU(X, TARGET) = 0 (independence). Returns null mean SU and p-value.

    The null mean shows the upward finite-sample bias of SU for many levels.
    """
    obs = symmetric_uncertainty(x, y, k, ky)
    null = np.array([symmetric_uncertainty(x, rng.permutation(y), k, ky) for _ in range(N_PERM)])
    p = (1 + np.sum(null >= obs)) / (1 + N_PERM)
    return null.mean(), p


def fisher_rare_vs_rest(x, y, k, thr):
    """Fisher test: is TARGET distribution in the pooled rare levels different from the rest?"""
    rare = rare_mask(x, k, thr)[x]
    if not rare.any():
        return np.nan, "no rare levels"
    table = [[np.sum(rare & (y == 0)), np.sum(rare & (y == 1))],
             [np.sum(~rare & (y == 0)), np.sum(~rare & (y == 1))]]
    p = fisher_exact(table)[1]
    return p, ("difference detected" if p < ALPHA else "no evidence of difference")


# ANALYSIS

def analyse_column(df: pd.DataFrame, col: str, y: np.ndarray, ky: int, rng) -> list:
    x, k = encode(df[col])
    n = len(x)

    su_orig = symmetric_uncertainty(x, y, k, ky)
    null_mean, perm_p = permutation_test_su(x, y, k, ky, rng)

    idxs = [rng.integers(0, n, n) for _ in range(N_BOOT)]
    boots = [(x[i], y[i]) for i in idxs]
    boot_orig = [symmetric_uncertainty(xb, yb, k, ky) for xb, yb in boots]

    rows = []
    for thr in THRESHOLDS:
        rare = rare_mask(x, k, thr)
        row = {
            "Feature": col, "Thr": thr,
            "Levels": k, "Rare levels": int(rare.sum()),
            "Rows in rare": int(rare[x].sum()),
            "SU orig": su_orig, "SU null mean": null_mean, "Perm p (SU>0)": perm_p,
        }
        for name, fn in (("drop", su_drop), ("merge", su_merge)):
            su_new = fn(x, y, k, ky, thr)
            diffs = [fn(xb, yb, k, ky, thr) - so for (xb, yb), so in zip(boots, boot_orig)]
            lo, hi, sig = ci_and_sig(diffs)
            row.update({
                f"SU {name}": su_new, f"dSU {name}": su_new - su_orig,
                f"CI lo {name}": lo, f"CI hi {name}": hi, f"Sig {name}": sig,
            })
        row["Fisher p"], row["Fisher"] = fisher_rare_vs_rest(x, y, k, thr)
        rows.append(row)
    return rows


def run(data_path: Path, out_path: Path) -> pd.DataFrame:
    df = pd.read_csv(data_path, encoding="unicode_escape")
    y, uniq = pd.factorize(df[TARGET])
    y = y.astype(np.int64)
    ky = len(uniq)

    cols = [c for c in CATEGORICAL_COLS if c in df.columns and df[c].nunique(dropna=False) >= MIN_LEVELS]
    print(f"Rows: {len(df)}, analysed categorical variables (>= {MIN_LEVELS} levels, NaN counted as a level): {len(cols)}")
    print(cols, "\n")

    rng = np.random.default_rng(42)
    rows = []
    for col in cols:
        print("processing", col, flush=True)
        rows += analyse_column(df, col, y, ky, rng)

    res = pd.DataFrame(rows)
    res.to_csv(out_path, index=False)

    pd.set_option("display.max_columns", None)
    pd.set_option("display.width", 250)
    for thr in THRESHOLDS:
        print("\n" + "=" * 100)
        print(f"Rare level threshold = {thr:.3f}  (bootstrap {N_BOOT}, permutation {N_PERM}, alpha {ALPHA})")
        print("=" * 100)
        part = res[res["Thr"] == thr].drop(columns="Thr")
        print(part.round(5).to_string(index=False))
    print(f"\nSaved to {out_path}")
    return res


if __name__ == "__main__":
    BASE_DIR = Path(__file__).resolve().parent
    run(BASE_DIR / "DataHomeCredit.csv", BASE_DIR / "results.csv")
