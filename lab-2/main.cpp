#include <iostream>
#include <fstream>
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
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        for (size_t i = 0; i < x.size(); ++i) x[i] += lambda * dir[i];
        return f(x);
    };
    
    // Dynamic Edging
    double h = 0.01;
    double lambda0 = 0.0;
    
    double f_minus = f_lambda(lambda0 - h);
    double f0 = f_lambda(lambda0);
    double f_plus = f_lambda(lambda0 + h);
    
    double a, b;
    
    if (f_minus >= f0 && f_plus >= f0) {
        a = lambda0 - h;
        b = lambda0 + h;
    } else {
        double step = h;
        double l_prev = lambda0;
        double l_curr = lambda0;
        double f_curr = f0;
        
        if (f_minus < f0) {
            step = -h;
            l_curr = lambda0 - h;
            f_curr = f_minus;
        } else {
            step = h;
            l_curr = lambda0 + h;
            f_curr = f_plus;
        }
        
        while (true) {
            step *= 2.0;
            double l_next = l_curr + step;
            double f_next = f_lambda(l_next);
            
            if (f_next >= f_curr) {
                a = std::min(l_prev, l_next);
                b = std::max(l_prev, l_next);
                break;
            }
            
            l_prev = l_curr;
            l_curr = l_next;
            f_curr = f_next;
        }
    }
    
    // Golden-Ratio Method
    double phi = (1.0 + std::sqrt(5.0)) / 2.0;
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

std::vector<double> methodGauss(std::vector<double> x0, Func f, double eps, double eps_1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }

    std::vector<double> x = x0;
    int n = static_cast<int>(x.size());
    std::cout << "\nGauss Method\n";
    
    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_prev = x;
        for (int i = 0; i < n; ++i) {
            std::vector<double> dir(n, 0.0);
            dir[i] = 1.0;
            double lambda = lineSearch(x, dir, f, eps_1d);
            for (int j = 0; j < n; ++j) x[j] += lambda * dir[j];

            for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
            log_file << '\n';
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

    log_file.close();

    return x;
}

std::vector<double> methodHookeJeeves(std::vector<double> x0, Func f, double eps, double delta0, double eps_1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }

    std::vector<double> x_base = x0, x_curr = x0;
    double delta = delta0;
    int n = static_cast<int>(x0.size());
    std::cout << "\nHooke-Jeeves Method\n";

    // Exploration
    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_trial = x_curr;
        bool success = false;
        
        for (int i = 0; i < n; ++i) {
            std::vector<double> x_plus = x_trial; 
            x_plus[i] += delta;
            std::vector<double> x_minus = x_trial; 
            x_minus[i] -= delta;
            
            if (f(x_plus) < f(x_trial)) {
                x_trial = x_plus;
                success = true;
            } else if (f(x_minus) < f(x_trial)) {
                x_trial = x_minus;
                success = true;
            }
        }

        // Pattern search
        if (success) {
            std::vector<double> dir(n);
            for (int i = 0; i < n; ++i) dir[i] = x_trial[i] - x_base[i];
            
            double lambda = lineSearch(x_trial, dir, f, eps_1d);
            std::vector<double> x_new = x_trial;
            for (int i = 0; i < n; ++i) x_new[i] += lambda * dir[i];

            if (f(x_new) < f(x_curr)) {
                x_base = x_curr;
                x_curr = x_new;
            } else {
                x_base = x_curr;
                x_curr = x_trial;
            }
        } else {
            delta *= 0.5;
            x_base = x_curr;
        }

        for (int j = 0; j < n; ++j) log_file << x_curr[j] << ' ';
        log_file << '\n';

        if (delta < eps) {
            std::cout << "Convergence at iteration " << k + 1 << std::endl;
            break;
        }
    }

    log_file.close();

    return x_curr;
}

std::vector<double> methodRosenbrock(std::vector<double> x0, Func f, double eps, double eps_1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }

    std::vector<double> x = x0;
    int n = static_cast<int>(x.size());
    std::vector<std::vector<double>> S(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) S[i][i] = 1.0;

    std::cout << "\nRosenbrock Method\n";

    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_start = x, lambdas(n);

        for (int i = 0; i < n; ++i) {
            lambdas[i] = lineSearch(x, S[i], f, eps_1d);
            for (int j = 0; j < n; ++j) x[j] += lambdas[i] * S[i][j];
        }

        std::vector<double> A1(n);
        for (int i = 0; i < n; ++i) 
            A1[i] = x[i] - x_start[i];

        double norm_A1 = 0.0;
        for (int i = 0; i < n; ++i) 
            norm_A1 += A1[i] * A1[i];
        norm_A1 = std::sqrt(norm_A1);
        
        for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
        log_file << '\n';

        if (norm_A1 < eps && abs(f(x) - f(x_start)) < eps) {
            std::cout << "Convergence at iteration " << k + 1 << std::endl;
            break;
        }

        // Palmer's Method for Direction Update
        std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                for (int m = 0; m < n; ++m) A[i][m] += lambdas[j] * S[j][m];
            }
        }

        std::vector<std::vector<double>> S_new(n, std::vector<double>(n, 0.0));
        if (norm_A1 > 1e-10) {
            for (int m = 0; m < n; ++m) S_new[0][m] = A[0][m] / norm_A1;
        } else {
            // Fallback
            for (int m = 0; m < n; ++m) S_new[0][m] = S[0][m];
        }

        for (int i = 1; i < n; ++i) {
            double norm_Ai = 0.0, norm_Ai_prev = 0.0;
            for (int m = 0; m < n; ++m) {
                norm_Ai += A[i][m] * A[i][m];
                norm_Ai_prev += A[i-1][m] * A[i-1][m];
            }
            norm_Ai = std::sqrt(norm_Ai);
            norm_Ai_prev = std::sqrt(norm_Ai_prev);

            double dot_Ai_Ai_prev = 0.0;
            for (int m = 0; m < n; ++m) dot_Ai_Ai_prev += A[i][m] * A[i-1][m];

            if (norm_Ai < 1e-10 || norm_Ai_prev < 1e-10) {
                // Fallback
                for (int m = 0; m < n; ++m) S_new[i][m] = S[i][m];
                continue;
            }

            double coeff = (norm_Ai / norm_Ai_prev) * (dot_Ai_Ai_prev / (norm_Ai_prev * norm_Ai_prev));
            
            std::vector<double> B(n);
            for (int m = 0; m < n; ++m) B[m] = A[i][m] - coeff * A[i-1][m];

            double norm_B = 0.0;
            for (int m = 0; m < n; ++m) norm_B += B[m] * B[m];
            norm_B = std::sqrt(norm_B);

            if (norm_B > 1e-10) {
                for (int m = 0; m < n; ++m) S_new[i][m] = B[m] / norm_B;
            } else {
                // Fallback
                for (int m = 0; m < n; ++m) S_new[i][m] = S[i][m];
            }
        }

        S = S_new;
    }

    log_file.close();

    return x;
}

int main() {
    std::cout << std::fixed << std::setprecision(8);
    
    double eps = 1e-4;
    double eps_1d = 1e-4;
    int maxIter = 1000;
    double delta0 = 0.01;

    std::vector<double> x0_quad = {0.0, 0.0};
    std::vector<double> x0_ros = {-1.2, 1.0};

    std::cout << "Minimization of Quadratic Function\n";
    std::cout << "Starting point: [0.0, 0.0]\n";
    std::cout << "Expected minimum: [3.0, 7.0]\n\n";
    
    std::vector<double> res1 = methodGauss(x0_quad, quadraticFunc, eps, eps_1d, maxIter, "gauss_quad.txt");
    std::cout << "Gauss Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << quadraticFunc(res1) << std::endl;

    std::vector<double> res2 = methodHookeJeeves(x0_quad, quadraticFunc, eps, delta0, eps_1d, maxIter, "hj_quad.txt");
    std::cout << "Hooke-Jeeves Result: [" << res2[0] << ", " << res2[1] 
              << "] F = " << quadraticFunc(res2) << std::endl;

    std::vector<double> res3 = methodRosenbrock(x0_quad, quadraticFunc, eps, eps_1d, maxIter, "rosen_quad.txt");
    std::cout << "Rosenbrock Result: [" << res3[0] << ", " << res3[1] 
              << "] F = " << quadraticFunc(res3) << std::endl;

    std::cout << "\n\nMinimization of Rosenbrock Function\n";
    std::cout << "Starting point: [-1.2, 1.0]\n";
    std::cout << "Expected minimum: [1.0, 1.0]\n\n";
    
    res1 = methodGauss(x0_ros, rosenbrockFunc, eps, eps_1d, maxIter, "gauss_rosen.txt");
    std::cout << "Gauss Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << rosenbrockFunc(res1) << std::endl;

    res2 = methodHookeJeeves(x0_ros, rosenbrockFunc, eps, delta0, eps_1d, maxIter, "hj_rosen.txt");
    std::cout << "Hooke-Jeeves Result: [" << res2[0] << ", " << res2[1] 
              << "] F = " << rosenbrockFunc(res2) << std::endl;

    res3 = methodRosenbrock(x0_ros, rosenbrockFunc, eps, eps_1d, maxIter, "rosen_rosen.txt");
    std::cout << "Rosenbrock Result: [" << res3[0] << ", " << res3[1] 
              << "] F = " << rosenbrockFunc(res3) << std::endl;

    return 0;
}