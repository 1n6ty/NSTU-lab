import matplotlib.pyplot as plt
import numpy as np
import re

def target_function(x, y):
    """
    Define the target function to draw the contour map.
    REPLACE the C, a, and b arrays with the coefficients for your specific variant.
    Currently using dummy coefficients as a placeholder.
    """
    C = np.array([5, 5, 3, 6, 2, 3])
    a = np.array([-9, -10, 8, -6, -9, 1])
    b = np.array([-3, -1, 8, -5, -3, 5])
    
    result = np.zeros_like(x)
    for i in range(len(C)):
        result += C[i] / (1.0 + (x - a[i])**2 + (y - b[i])**2)
    return result

def parse_trajectories(filename):
    """
    Parses the trajectory file where each line is a list of (x,y) coordinates.
    """
    trajectories = []
    with open(filename, 'r') as file:
        for line in file:
            # Use regex to extract all (x, y) pairs on the line
            matches = re.findall(r'\(([^,]+),([^)]+)\)', line)
            if matches:
                # Convert string matches to floats
                trajectory = [(float(x), float(y)) for x, y in matches]
                trajectories.append(trajectory)
    return trajectories

def plot_optimization(filename):
    trajectories = parse_trajectories(filename)

    # 1. Create a grid to evaluate the target function for the background
    x_vals = np.linspace(-10, 10, 400)
    y_vals = np.linspace(-10, 10, 400)
    X, Y = np.meshgrid(x_vals, y_vals)
    Z = target_function(X, Y)

    # 2. Set up the plot
    plt.figure(figsize=(10, 8))
    
    # Draw the contour map of the target function
    contour = plt.contourf(X, Y, Z, levels=30, cmap='viridis', alpha=0.8)
    plt.colorbar(contour, label='Target Function Value')

    # 3. Plot each trajectory on top of the contour
    for i, traj in enumerate(trajectories):
        xs = [p[0] for p in traj]
        ys = [p[1] for p in traj]

        # Draw the path
        plt.plot(xs, ys, marker='.', linestyle='-', linewidth=1.5, color='white', alpha=0.7)
        
        # Highlight the Start point (Green) and End point (Red)
        plt.plot(xs[0], ys[0], marker='o', color='lime', markersize=6)
        plt.plot(xs[-1], ys[-1], marker='X', color='red', markersize=8)

    # 4. Final plot formatting
    plt.title('Optimization Trajectories on Target Function')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.grid(True, linestyle='--', alpha=0.3)
    
    # Custom legend for start/end markers
    plt.plot([], [], 'o', color='lime', label='Start Point')
    plt.plot([], [], 'X', color='red', label='End Point')
    plt.plot([], [], '-', color='white', label='Search Path')
    plt.legend(loc='upper right')

    plt.show()

if __name__ == "__main__":
    # Ensure this matches the name of your text file
    plot_optimization('path_1.txt')