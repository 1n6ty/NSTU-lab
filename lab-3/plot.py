import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

sns.set_theme(style="whitegrid")
plt.rcParams['figure.dpi'] = 100

def load_data_from_rows(filepath):
    try:
        data_rows = np.loadtxt(filepath)
        
        if data_rows.shape[0] != 6:
            raise ValueError(f"Ожидалось 6 строк данных в файле {filepath}, получено {data_rows.shape[0]}.")

        # Присваиваем имена столбцам
        x_values = data_rows[0, :]
        
        # Создаем словарь для удобного построения графиков
        data_dict = {
            'X': x_values,
            'D (Исходные данные)': data_rows[1, :],
            'SP, p=0.0': data_rows[2, :],
            'SP, p=0.4': data_rows[3, :],
            'SP, p=0.8': data_rows[4, :],
            'SP, p=0.99': data_rows[5, :],
        }
        return pd.DataFrame(data_dict)

    except FileNotFoundError:
        print(f"Ошибка: Файл {filepath} не найден. Убедитесь, что C++ код отработал и создал файлы.")
        return None
    except Exception as e:
        print(f"Ошибка при чтении файла {filepath}: {e}")
        return None

def plot_spline_data(df, title):
    """Отрисовывает все сплайны и исходные данные на одной фигуре."""
    if df is None:
        return

    # Создаем новую фигуру (новое окно)
    plt.figure(figsize=(14, 7)) 
    
    # 1. Исходные данные (точки)
    # Используем plt.scatter для наглядности исходного зашумленного ряда D
    plt.scatter(df['X'], df['D (Исходные данные)'], label='Исходные данные D', s=5, alpha=0.6, color='gray')

    # 2. Отрисовка сплайнов (линии)
    line_styles = ['-', '--', ':', '-.'] # Для разнообразия линий
    spline_cols = [col for col in df.columns if col.startswith('SP,')]
    
    for i, col in enumerate(spline_cols):
        plt.plot(df['X'], df[col], label=col, 
                 linewidth=2, 
                 linestyle=line_styles[i % len(line_styles)])

    plt.title(title, fontsize=16)
    plt.xlabel('X(i)', fontsize=12)
    plt.ylabel('Value', fontsize=12)
    plt.legend(title='p', loc='upper right')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.tight_layout()

FILE_UNIFORM = "spline_uniform_weights.txt"
FILE_VARIABLE = "spline_variable_weights.txt"

# 1. Загрузка данных с равномерными весами (w=1.0)
df_uniform = load_data_from_rows(FILE_UNIFORM)
plot_spline_data(df_uniform, 
                 "Сглаживающие сплайны: Равномерные веса (w_i = 1.0)")


# 2. Загрузка данных с неравномерными весами (w_i = 0.5 в [200, 700])
df_variable = load_data_from_rows(FILE_VARIABLE)
plot_spline_data(df_variable, 
                 "Сглаживающие сплайны: Неравномерные веса (w_i=0.5 в области 200-700)")

# 3. Отображение всех созданных окон
plt.show()

print("Программа завершена. Два графика отображены в отдельных окнах.")
