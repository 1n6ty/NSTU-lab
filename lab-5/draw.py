import matplotlib.pyplot as plt
import numpy as np

# --- 1. Ваша функция ---
def my_func(x, c):
    denominator = 1.0 + c * np.exp(-(x * x) / 4.0);
    return np.exp(-(x * x) / 2.0) / (denominator * denominator);

def main():
    c = float(input("Input c: "))

    # Загрузка данных (замените названия на свои файлы)
    files = ['normal_data', 'poisoning_data', 'poisoned_data']
    colors = ['#ff9999', '#99ff99', '#9999ff']
    labels = ['Истинные данные', 'Атака', 'Отравленныйе данные']

    plt.figure(figsize=(10, 6))

    all_data = []
    max_hist_height = 0  # Сюда сохраним высоту самого высокого столбца

    # 2. Отрисовка гистограмм
    for i, filename in enumerate(files):
        try:
            data = np.loadtxt(filename)
            
            all_data.extend(data)

            # Рисуем гистограмму и получаем значения столбцов (counts)
            counts, bins, patches = plt.hist(data, bins=30, color=colors[i], density=True,
                                             alpha=0.5, label=labels[i], edgecolor='white')
            
            # Находим самый высокий столбец среди всех файлов
            max_hist_height = max(max_hist_height, counts.max())
            
        except Exception as e:
            print(f"Ошибка при чтении {filename}: {e}")

    # 3. Нормализация и отрисовка функции
    if all_data:
        x_min, x_max = min(all_data), max(all_data)
        x_vals = np.linspace(x_min, x_max, 1000)
        y_vals = my_func(x_vals, c)

        # --- КЛЮЧЕВОЙ МОМЕНТ: Масштабирование функции ---
        # Мы делим функцию на её собственный максимум (делаем её высоту равной 1)
        # и умножаем на высоту самого высокого столбца гистограммы.
        y_max_func = y_vals.max()
        if y_max_func > 0:
            y_vals_scaled = y_vals * (max_hist_height / y_max_func)
        else:
            y_vals_scaled = y_vals

        plt.plot(x_vals, y_vals_scaled, color='black', linewidth=3, 
                 label='Весовая функция', linestyle='-')

    # Оформление
    plt.title("Гистограммы и функция одинаковой высоты")
    plt.legend()
    plt.grid(axis='y', alpha=0.3)
    plt.show()

if __name__ == "__main__":
    main()