#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <algorithm>

// Типы для функций и векторов
using Func = std::function<double(const std::vector<double>&)>;
using Grad = std::function<std::vector<double>(const std::vector<double>&)>;

// --- Операции с векторами ---
std::vector<double> operator-(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] - b[i];
    return res;
}

std::vector<double> operator+(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] + b[i];
    return res;
}

std::vector<double> operator*(const std::vector<double>& a, double b) {
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); ++i) res[i] = a[i] * b;
    return res;
}

double dot(const std::vector<double>& a, const std::vector<double>& b) {
    double res = 0;
    for (size_t i = 0; i < a.size(); ++i) res += a[i] * b[i];
    return res;
}

double norm(const std::vector<double>& a) {
    return sqrt(dot(a, a));
}

// Test Functions
double quadraticFunc(const std::vector<double>& x) {
    return 5 * std::pow(6 * x[0] + 5 * x[1] - 60, 2) + std::pow(x[0] - 2 * x[1] - 4, 2);
}
double g(const std::vector<double>& x) {
    return 5 * x[0] + 3 * x[1] - 30;
}

double Q(const std::vector<double>& x, double r) {
    return quadraticFunc(x) + r * std::pow((g(x) + std::abs(g(x))) / 2, 2);
}
std::vector<double> grad_Q(const std::vector<double>& x, double r) {
    return {
        46 * x[1] - 546 + r * ((g(x) >= 0) ? g(x) + std::abs(g(x)): 0) * 2 * (3 * x[1] - 25),
        62 * x[0] - 562 + r * ((g(x) >= 0) ? g(x) + std::abs(g(x)): 0) * 2 * (5 * x[0] - 27)
    };
}


// Golden-Ratio One-Dimensional Search
double lineSearch(const std::vector<double>& x0, const std::vector<double>& dir, Func f, double eps_1d) {
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        x = x + dir * lambda;
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

double parabolaMin(std::vector<double> p1, std::vector<double> p2, std::vector<double> p3) {
    double det = (p1[0] - p2[0]) * (p1[0] - p3[0]) * (p2[0] - p3[0]);
    
    double a = (p3[0] * (p2[1] - p1[1]) + p2[0] * (p1[1] - p3[1]) + p1[0] * (p3[1] - p2[1])) / det;
    double b = (p3[0]*p3[0] * (p1[1] - p2[1]) + p2[0]*p2[0] * (p3[1] - p1[1]) + p1[0]*p1[0] * (p2[1] - p3[1])) / det;

    return -b / 2 / a;
}

// Paraboloid One-Dimensional Search
double lineSearchP(const std::vector<double>& x0, const std::vector<double>& dir, Func f, double eps_1d) {
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        x = x + dir * lambda;
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
    
    return parabolaMin({a, f_lambda(a)}, {(a + b) / 2, f_lambda((a + b) / 2)}, {b, f_lambda(b)});
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

        std::vector<double> A1 = x - x_start;
        double norm_A1 = norm(A1);
        
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
            double norm_Ai = norm(A[i]), norm_Ai_prev = norm(A[i - 1]);
            double dot_Ai_Ai_prev = dot(A[i], A[i - 1]);

            if (norm_Ai < 1e-10 || norm_Ai_prev < 1e-10) {
                // Fallback
                for (int m = 0; m < n; ++m) S_new[i][m] = S[i][m];
                continue;
            }

            double coeff = (norm_Ai / norm_Ai_prev) * (dot_Ai_Ai_prev / (norm_Ai_prev * norm_Ai_prev));
            
            std::vector<double> B = A[i] - A[i - 1] * coeff;
            double norm_B = norm(B);

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

std::vector<double> conjugateGradientFR(std::vector<double> x, Func f, Grad g, double eps, double eps1d, int maxIter, int maxRestart, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }
    
    int n = x.size();
    std::vector<double> grad = g(x);
    std::vector<double> s = grad * -1.0;

    for (int reset = 0; reset < maxRestart; ++reset){
        for (int iter = 0; iter < maxIter; ++iter) {
            double lambda = lineSearch(x, s, f, eps1d);
            std::vector<double> x_next = x + s * lambda;
            std::vector<double> grad_next = g(x_next);
            
            double omega = dot(grad_next, grad_next) / dot(grad, grad);
            
            for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
            log_file << '\n';

            s = (grad_next * -1.0) + (s * omega);
            x = x_next;
            grad = grad_next;

            if (norm(s) < eps) {
                std::cout << "Convergence at iteration " << iter + 1 << " and resets count " << reset << std::endl;
                return x;
            }
        }
    }

    log_file.close();

    return x;
}