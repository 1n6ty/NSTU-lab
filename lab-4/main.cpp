#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <algorithm>

using Func = std::function<double(const std::vector<double>&, double)>;
using Grad = std::function<std::vector<double>(const std::vector<double>&, double)>;

// --- Vector Operations ---
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

// --- Target Functions ---
double quadraticFunc(const std::vector<double>& x) {
    return 5 * std::pow(6 * x[0] + 5 * x[1] - 60, 2) + std::pow(x[0] - 2 * x[1] - 4, 2);
}

double g(const std::vector<double>& x) {
    return 5 * x[0] + 3 * x[1] - 30;
}

double Q(const std::vector<double>& x, double r) {
    double penalty = std::max(0.0, g(x));
    return quadraticFunc(x) + r * std::pow(penalty, 2);
}

std::vector<double> grad_Q(const std::vector<double>& x, double r) {
    double penalty = std::max(0.0, g(x));
    
    double df_dx1 = 362 * x[0] + 296 * x[1] - 3608;
    double dq_dx1 = df_dx1 + 2 * r * penalty * 5; 

    double df_dx2 = 296 * x[0] + 258 * x[1] - 2984;
    double dq_dx2 = df_dx2 + 2 * r * penalty * 3;

    return {dq_dx1, dq_dx2};
}

// --- Golden-Ratio One-Dimensional Search ---
double lineSearch(const std::vector<double>& x0, double r, const std::vector<double>& dir, Func f, double eps_1d) {
    auto f_lambda = [&](double lambda) {
        return f(x0 + dir * lambda, r);
    };
    
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

std::vector<double> methodRosenbrock(std::vector<double> x, double r, Func f, double eps, double eps_1d, int maxIter, std::ofstream& log_file) {
    int n = static_cast<int>(x.size());
    std::vector<std::vector<double>> S(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) S[i][i] = 1.0;

    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_start = x, lambdas(n);

        if (log_file.is_open()) {
            for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
            log_file << '\n';
        }

        for (int i = 0; i < n; ++i) {
            lambdas[i] = lineSearch(x, r, S[i], f, eps_1d);
            for (int j = 0; j < n; ++j) x[j] += lambdas[i] * S[i][j];
        }

        std::vector<double> A1 = x - x_start;
        double norm_A1 = norm(A1);

        if (norm_A1 < eps && std::abs(f(x, r) - f(x_start, r)) < eps) {
            break;
        }

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
            for (int m = 0; m < n; ++m) S_new[0][m] = S[0][m];
        }

        for (int i = 1; i < n; ++i) {
            double norm_Ai = norm(A[i]), norm_Ai_prev = norm(A[i - 1]);
            double dot_Ai_Ai_prev = dot(A[i], A[i - 1]);

            if (norm_Ai < 1e-10 || norm_Ai_prev < 1e-10) {
                for (int m = 0; m < n; ++m) S_new[i][m] = S[i][m];
                continue;
            }

            double coeff = (norm_Ai / norm_Ai_prev) * (dot_Ai_Ai_prev / (norm_Ai_prev * norm_Ai_prev));
            std::vector<double> B = A[i] - A[i - 1] * coeff;
            double norm_B = norm(B);

            if (norm_B > 1e-10) {
                for (int m = 0; m < n; ++m) S_new[i][m] = B[m] / norm_B;
            } else {
                for (int m = 0; m < n; ++m) S_new[i][m] = S[i][m];
            }
        }
        S = S_new;
    }
    return x;
}

std::vector<double> conjugateGradientFR(std::vector<double> x, double r, Func f, Grad gr, double eps, double eps1d, int maxIter, std::ofstream& log_file) {
    int n = x.size();
    std::vector<double> grad = gr(x, r);
    std::vector<double> s = grad * -1.0;

    for (int iter = 0; iter < maxIter; ++iter) {
        double lambda = lineSearch(x, r, s, f, eps1d);
        std::vector<double> x_next = x + s * lambda;
        std::vector<double> grad_next = gr(x_next, r);
        
        double omega = dot(grad_next, grad_next) / dot(grad, grad);
        
        if (log_file.is_open()) {
            for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
            log_file << '\n';
        }

        if (iter % n == 0 && iter > 0) {
            s = grad_next * -1.0;
        } else {
            s = (grad_next * -1.0) + (s * omega);
        }
        x = x_next;
        grad = grad_next;

        if (norm(grad) < eps) {
            break;
        }
    }
    return x;
}

int main() {
    std::cout << std::fixed << std::setprecision(8);
    
    double eps = 1e-8;
    double eps_1d = 1e-8;
    int maxIter = 10000;
    
    std::vector<double> x_start = {10.0, 0.0};

    std::cout << "--- Penalty Function Method ---\n";

    std::cout << "Fletcher-Reeves (Conjugate Gradient)\n";
    double r_cg = 0.1;
    std::vector<double> x_cg = x_start;
    std::ofstream cg_log("cg_path.txt");

    std::cout << "x = [" << std::setw(10) << x_cg[0] << ", " << std::setw(10) << x_cg[1] << std::endl;
    for (int penalty_step = 1; penalty_step <= 20; ++penalty_step) {
        x_cg = conjugateGradientFR(x_cg, r_cg, Q, grad_Q, eps, eps_1d, maxIter, cg_log);
        
        double constraint_val = g(x_cg);
        std::cout << "Step " << penalty_step << " | r = " << std::setw(8) << r_cg 
                  << " | x = [" << std::setw(10) << x_cg[0] << ", " << std::setw(10) << x_cg[1] 
                  << "] | g(x) = " << constraint_val << "\n";
                  
        if (constraint_val <= eps) {
            break;
        }
        r_cg *= 10.0;
    }
    std::cout << "Final F(x) = " << quadraticFunc(x_cg) << "\n\n";
    cg_log.close();


    std::cout << "Rosenbrock (Rotating Coordinates)\n";
    double r_ros = 0.1;
    std::vector<double> x_ros = x_start;
    std::ofstream ros_log("rosenbrock_path.txt");

    std::cout << "x = [" << std::setw(10) << x_ros[0] << ", " << std::setw(10) << x_ros[1] << std::endl;
    for (int penalty_step = 1; penalty_step <= 20; ++penalty_step) {
        x_ros = methodRosenbrock(x_ros, r_ros, Q, eps, eps_1d, maxIter, ros_log);
        
        double constraint_val = g(x_ros);
        std::cout << "Step " << penalty_step << " | r = " << std::setw(8) << r_ros 
                  << " | x = [" << std::setw(10) << x_ros[0] << ", " << std::setw(10) << x_ros[1] 
                  << "] | g(x) = " << constraint_val << "\n";
                  
        if (constraint_val <= eps) {
            break;
        }
        r_ros *= 10.0;
    }
    std::cout << "Final F(x) = " << quadraticFunc(x_ros) << "\n";
    ros_log.close();

    return 0;
}