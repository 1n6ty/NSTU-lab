import sys, argparse
import numpy as np
import matplotlib.pyplot as plt

# 1. Задаем функцию
def quadratic_func(x, y):
    return 10.0 * (x + y - 10.0)**2 + (x - y + 4.0)**2
def rosenbrock_func(x, y):
    return 100.0 * (y - x * x) ** 2 + (1.0 - x) ** 2

FUNCTIONS = {
    'quadratic': quadratic_func,
    'rosenbrock': rosenbrock_func
}

def main():
    # 2. Настройка парсера аргументов
    parser = argparse.ArgumentParser(description='Визуализация оптимизации функции.')
    parser.add_argument('file', type=str, help='Путь к файлу с точками (например, gauss_quad.txt)')
    parser.add_argument('--func', choices=FUNCTIONS.keys(), default='quadratic', 
                        help='Выберите функцию для отрисовки уровней (по умолчанию: quadratic)')

    args = parser.parse_args()

    # 3. Чтение данных
    try:
        # Считываем данные, пропуская возможные текстовые заголовки (как "")
        # Используем genfromtxt, так как он лучше справляется с нестандартным текстом
        data = np.genfromtxt(args.file, invalid_raise=False)
        
        # Если в файле были строки с текстом, genfromtxt может оставить NaN, убираем их
        data = data[~np.isnan(data).any(axis=1)]
        
        x_path = data[:, 0]
        y_path = data[:, 1]
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        sys.exit(1)

    # 4. Подготовка графики
    target_func = FUNCTIONS[args.func]
    
    # Динамические границы с отступами
    pad = 2
    x_min, x_max = x_path.min() - pad, x_path.max() + pad
    y_min, y_max = y_path.min() - pad, y_path.max() + pad
    
    X, Y = np.meshgrid(np.linspace(x_min, x_max, 500), 
                       np.linspace(y_min, y_max, 500))
    Z = target_func(X, Y)

    plt.figure(figsize=(10, 7))
    
    # Рисуем линии уровня (для Розенброка логарифмическая шкала обязательна)
    levels = np.logspace(-1, 5, 35) if args.func == 'rosenbrock' else 30
    contour = plt.contour(X, Y, Z, levels=levels, cmap='viridis', alpha=0.8)
    plt.colorbar(contour, label=f'f(x, y) [{args.func}]')

    # Рисуем путь 
    plt.plot(x_path, y_path, 'r.-', linewidth=1, markersize=5, label='Траектория')
    plt.plot(x_path[0], y_path[0], 'go', label='Старт')

    plt.title(f'Визуализация: {args.func}\nФайл: {args.file}')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.grid(True, linestyle=':', alpha=0.5)
    
    plt.show()

if __name__ == "__main__":
    main()