#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <algorithm>

using Func = std::function<double(const std::vector<double>&)>;

// Test Functions
double quadraticFunc(const std::vector<double>& x) {
    return 10.0 * std::pow(x[0] + x[1] - 10.0, 2) + std::pow(x[0] - x[1] + 4.0, 2);
}
double rosenbrockFunc(const std::vector<double>& x) {
    return 100.0 * std::pow(x[1] - x[0] * x[0], 2) + std::pow(1.0 - x[0], 2);
}

// Golden-Ratio One-Dimensional Search
double lineSearch(const std::vector<double>& x0, const std::vector<double>& dir, Func f, double eps_1d) {
    double a = -10.0, b = 10.0, phi = (1.0 + std::sqrt(5.0)) / 2.0;
    
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        for (size_t i = 0; i < x.size(); ++i) x[i] += lambda * dir[i];
        return f(x);
    };
    
    double x1 = b - (b - a) / phi, x2 = a + (b - a) / phi;
    double f1 = f_lambda(x1), f2 = f_lambda(x2);
    
    while (std::abs(b - a) > eps_1d) {
        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - (b - a) / phi;
            f1 = f_lambda(x1);
        } else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + (b - a) / phi;
            f2 = f_lambda(x2);
        }
    }
    
    return (a + b) / 2.0;
}

std::vector<double> derivative(const std::vector<double>& x, Func f, const double h){
    size_t n = x.size();
    std::vector<double> d(n, 0.0);
    for (int i = 0; i < n; i++) {
        
    }
    return {
        1 / (2 * h) * (f({x[0] - 2 * h, x[1]}) + 8 * f({x[0] + h, x[1]}) - f({x[0] - h, x[1]})),
        1 / (2 * h) * (f({x[0], x[1] + h}) - f({x[0], x[1] - h})),
    };
}

std::vector<double> gradientDescent(const std::vector<double>& x0, Func f, const int maxIter, const double h, const double eps, const double eps_1d){
    std::vector<double> x = x0;
    int n = static_cast<int>(x.size());
    std::cout << "\nGradient Descent\n";
    
    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_prev = x;
        for (int i = 0; i < n; ++i) {
            std::vector<double> dir = derivative(x, f, h);
            for (int j = 0; j < n; ++j) dir[j] = -dir[j];
            double lambda = lineSearch(x, dir, f, eps_1d);
            for (int j = 0; j < n; ++j) x[j] += lambda * dir[j];
        }

        double diff = 0.0, diff_x = 0.0;
        for (int i = 0; i < n; ++i) {
            diff_x += std::pow(x[i] - x_prev[i], 2);
            diff += std::pow(f(x) - f(x_prev), 2);
        }
        
        if (std::sqrt(diff) < eps && std::sqrt(diff_x) < eps) {
            std::cout << "Convergence at iteration " << k + 1 << std::endl;
            break;
        }
    }
    return x;
}

int main() {
    std::cout << std::fixed << std::setprecision(8);
    
    double eps = 1e-12;
    double eps_1d = 1e-8;
    double h = 1e-6;
    int maxIter = 100;
    double delta0 = 0.01;

    std::vector<double> x0_quad = {0.0, 0.0};
    std::vector<double> x0_ros = {-1.2, 1.0};

    std::cout << "Minimization of Quadratic Function\n";
    std::cout << "Starting point: [0.0, 0.0]\n";
    std::cout << "Expected minimum: [3.0, 7.0]\n\n";
    
    std::vector<double> res1 = gradientDescent(x0_quad, quadraticFunc, maxIter, h, eps, eps_1d);
    std::cout << "Gradient Descent Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << quadraticFunc(res1) << std::endl;

    std::cout << "\n\nMinimization of Rosenbrock Function\n";
    std::cout << "Starting point: [-1.2, 1.0]\n";
    std::cout << "Expected minimum: [1.0, 1.0]\n\n";
    
    res1 = gradientDescent(x0_quad, rosenbrockFunc, maxIter, h, eps, eps_1d);
    std::cout << "Gradient Descent Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << rosenbrockFunc(res1) << std::endl;

    return 0;
}