import numpy as np
import matplotlib.pyplot as plt

# 1. Define the Objective Function f(x) 
def f(x1, x2):
    return 5 * (6*x1 + 5*x2 - 60)**2 + (x1 - 2*x2 - 4)**2

# 2. Define the Constraint g(x) <= 0 [cite: 5, 42, 47]
def g(x1, x2):
    return 5*x1 + 3*x2 - 30

# Setup the coordinate grid
x = np.linspace(-5, 15, 400)
y = np.linspace(-5, 20, 400)
X, Y = np.meshgrid(x, y)
Z = f(X, Y)

plt.figure(figsize=(10, 8))

# 3. Plot Contours of the Objective Function
# We use log scaling for levels to see the "valley" clearly
contours = plt.contour(X, Y, Z, levels=np.logspace(2, 6, 20), cmap='viridis', alpha=0.6)
plt.clabel(contours, inline=True, fontsize=8)

# 4. Plot the Constraint Boundary g(x) = 30 
# 5*x1 + 3*x2 = 30  =>  x2 = (30 - 5*x1) / 3
x_boundary = np.linspace(-10, 20, 100)
y_boundary = (30 - 5*x_boundary) / 3
plt.plot(x_boundary, y_boundary, 'r--', label='$g(x)=0$')

# 5. Shade the Infeasible Region (where g(x) > 0) [cite: 25, 26]
plt.fill_between(x_boundary, y_boundary, 25, color='red', alpha=0.1, label='$g(x) > 0$')

# 6. Load and Plot the Optimization Path from C++ output
try:
    # Change 'cg_path.txt' to 'rosenbrock_path.txt' to see the other method
    path = np.loadtxt('cg_path.txt') 
    plt.plot(path[:, 0], path[:, 1], 'ko-', markersize=3, linewidth=1, label='Траектория')
    # Mark the start and end
    plt.plot(path[0, 0], path[0, 1], 'go', label='Старт')
    plt.plot(path[-1, 0], path[-1, 1], 'ro', label='Найденный минимум')
except FileNotFoundError:
    print("Log file not found. Run the C++ code first to generate cg_path.txt.")

plt.title('Penalty Method Optimization Path')
plt.xlabel('$x_1$')
plt.ylabel('$x_2$')
plt.xlim(-5, 15)
plt.ylim(-5, 20)
plt.legend()
plt.grid(True, linestyle=':', alpha=0.6)
plt.show()