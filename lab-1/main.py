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

BASE_DIR = Path(__file__).resolve().parent

DATA_HOME_CREDIT_PATH = BASE_DIR / "DataHomeCredit.csv"
DESCRIPTION_HOME_CREDIT = BASE_DIR / "DescriptionHomeCredit.csv"

desc_df = pd.read_csv(DESCRIPTION_HOME_CREDIT, sep=";", encoding='unicode_escape', comment="#")

df = pd.read_csv(DATA_HOME_CREDIT_PATH, sep=",", encoding='unicode_escape', comment="#")[list(desc_df['Column'])]

def clear_k_least_freq(df: pd.DataFrame, col_name: str, k = 1):
    counts = df[col_name].value_counts(dropna=True)
    least_frequent_categories = counts.nsmallest(k).index
    return df[~df[col_name].isin(least_frequent_categories)]

for col in df.columns:
    print(f"--- Relative frequency for {col} ---")
    print(df[col].value_counts(normalize=True, dropna=False) * 100)
    print("\n")

print("\n\n-------(k = 0)-------\n\n")
for i in list(desc_df['Column']):
    if i != "TARGET": 
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(df[i], df['TARGET']))
        
print("\n\n-------(k = 1)-------\n\n")
for i in list(desc_df['Column']):
    filtered = clear_k_least_freq(df, i, 1)
    if i != "TARGET": 
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(filtered[i], filtered['TARGET']))

print("\n\n-------(k = 2)-------\n\n")
for i in list(desc_df['Column']):
    filtered = clear_k_least_freq(df, i, 2)
    if i != "TARGET": 
        print(f"{i} vs TARGET: ", symmetric_uncertainty_sklearn(filtered[i], filtered['TARGET']))
        
        
from scipy.stats import chi2_contingency, fisher_exact

def test_rare_category_significance(df, col_name, k=2):
    # 1. Находим k самых редких категорий
    counts = df[col_name].value_counts(dropna=True)
    rare_cats = counts.nsmallest(k).index
    
    # 2. Делим выборку на "редкие категории" и "остальные"
    is_rare = df[col_name].isin(rare_cats)
    
    # Строим таблицу сопряженности 2x2: [Rare/Not Rare] x [TARGET 0/1]
    contingency_table = pd.crosstab(is_rare, df['TARGET'])
    contingency_table.index = ['Other Categories', 'Rare Categories']
    
    print(f"=== Проверка для колонки: {col_name} ===")
    print("Таблица сопряженности:")
    print(contingency_table)
    print("\nДоля TARGET=1:")
    print(df.groupby(is_rare)['TARGET'].mean().rename({False: 'Other Categories', True: 'Rare Categories'}))
    
    # 3. Выполняем тест (Chi-square или Fisher)
    # Если объемы в редких категориях совсем малы (< 5), лучше использовать exact Fisher test
    if contingency_table.values.min() < 5:
        res = fisher_exact(contingency_table)
        p_val = res.pvalue
        test_name = "Fisher's Exact Test"
    else:
        chi2, p_val, dof, ex = chi2_contingency(contingency_table)
        test_name = "Chi-Square Test"
        
    print(f"\n{test_name} p-value: {p_val:.5f}")
    if p_val < 0.05:
        print("Результат: Отличие СТАТИСТИЧЕСКИ ЗНАЧИМО. Редкие категории содержали важный сигнал!")
    else:
        print("Результат: Отличие НЕЗНАЧИТЕЛЬНО. Редкие категории были шумом.")
    print("-" * 50)
    
print(test_rare_category_significance(df, 'ORGANIZATION_TYPE', k=40))