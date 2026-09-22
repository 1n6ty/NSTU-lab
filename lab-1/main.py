import pandas as pd
from pathlib import Path

from sklearn.metrics import mutual_info_score
import numpy as np

def calculate_entropy(labels):
    labels = np.asarray(labels, dtype=str)
    _, counts = np.unique(labels, return_counts=True)
    probabilities = counts / len(labels)
    return -np.sum(probabilities * np.log2(probabilities))

def symmetric_uncertainty_sklearn(x, y):
    # Очистка от NaN
    valid_mask = pd.notna(x) & pd.notna(y)
    x_clean = np.asarray(x[valid_mask], dtype=str)
    y_clean = np.asarray(y[valid_mask], dtype=str)

    if len(x_clean) == 0:
        return 0.0

    h_x = calculate_entropy(x_clean)
    h_y = calculate_entropy(y_clean)
    
    if h_x == 0 or h_y == 0:
        return 0.0
    
    mi = mutual_info_score(x_clean, y_clean) / np.log(2)
    return 2.0 * mi / (h_x + h_y)

REMOVE_K = 50

BASE_DIR = Path(__file__).resolve().parent

DATA_HOME_CREDIT_PATH = BASE_DIR / "DataHomeCredit.csv"
DESCRIPTION_HOME_CREDIT = BASE_DIR / "DescriptionHomeCredit.csv"

desc_df = pd.read_csv(DESCRIPTION_HOME_CREDIT, sep=";", encoding='unicode_escape', comment="#")

df = pd.read_csv(DATA_HOME_CREDIT_PATH, sep=",", encoding='unicode_escape', comment="#")[list(desc_df['Column'])]
initial_df = df

def clear_k_least_freq(df: pd.DataFrame, col_name: str, k = 1):
    counts = df[col_name].value_counts(dropna=True)
    least_frequent_categories = counts.nsmallest(k).index
    return df[~df[col_name].isin(least_frequent_categories)]

for col in df.columns:
    print(f"--- Relative frequency for {col} ---")
    print(df[col].value_counts(normalize=True, dropna=False) * 100)
    print("\n")

LL_COLUMNS = ['OCCUPATION_TYPE', 'ORGANIZATION_TYPE']
TARGET_COLUMN = 'TARGET'

print(f"\nFiltering only large-level variables: {LL_COLUMNS}")

df = df[LL_COLUMNS + [TARGET_COLUMN]]

print("\n\n-------(SU before removing)-------\n")
for i in LL_COLUMNS:
    if i != "TARGET":
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(df[i], df['TARGET']))

print(f"\n\n-------(SU after removing {REMOVE_K} layers with lowest frequences)-------\n")
for i in LL_COLUMNS:
    filtered = clear_k_least_freq(df, i, REMOVE_K)
    if i != "TARGET": 
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(filtered[i], filtered['TARGET']))
        
print(f"\n\n-------(SU diff)-------\n")
for i in LL_COLUMNS:
    filtered = clear_k_least_freq(df, i, REMOVE_K)
    if i != "TARGET": 
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(filtered[i], filtered['TARGET']) - symmetric_uncertainty_sklearn(df[i], df['TARGET']))

print("\n\n--------Statistical hypothesis testing---------\n")

from scipy.stats import chi2_contingency, fisher_exact

def test_rare_category_significance(df, col_name, k=2):
    counts = df[col_name].value_counts(dropna=True)
    rare_cats = counts.nsmallest(k).index
    
    is_rare = df[col_name].isin(rare_cats)
    
    contingency_table = pd.crosstab(is_rare, df['TARGET'])
    contingency_table.index = ['Other Categories', 'Rare Categories']
    
    print(f"-------Test for column: {col_name}--------")
    print("Contingency Table:")
    print(contingency_table)
    print("\nPart of TARGET=1:")
    print(df.groupby(is_rare)['TARGET'].mean().rename({False: 'Other Categories', True: 'Rare Categories'}))
    
    res = fisher_exact(contingency_table)
    p_val = res.pvalue
        
    print(f"\nFisher's Exact Test p-value: {p_val:.5f}")
    if p_val < 0.05:
        print("Result: Low-freq categories contained necessary signal.")
    else:
        print("Result: Low-freq categories are noise.")
    print("-" * 50)
    
for i in LL_COLUMNS:
    test_rare_category_significance(initial_df, i, k=REMOVE_K)