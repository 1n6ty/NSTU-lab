#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>

using Func = std::function<double(const std::vector<double>&)>;
using Grad = std::function<std::vector<double>(const std::vector<double>&)>;

// Vectors Operations
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
    return 10.0 * std::pow(x[0] + x[1] - 10.0, 2) + std::pow(x[0] - x[1] + 4.0, 2);
}
double rosenbrockFunc(const std::vector<double>& x) {
    return 100.0 * std::pow(x[1] - x[0] * x[0], 2) + std::pow(1.0 - x[0], 2);
}

std::vector<double> grad_quad(const std::vector<double>& x) {
    return {
        20.0 * (x[0] + x[1] - 10.0) + 2.0 * (x[0] - x[1] + 4.0),
        20.0 * (x[0] + x[1] - 10.0) - 2.0 * (x[0] - x[1] + 4.0)
    };
}

std::vector<double> grad_rosen(const std::vector<double>& x) {
    return {
        -400.0 * x[0] * (x[1] - x[0] * x[0]) - 2.0 * (1.0 - x[0]),
        200.0 * (x[1] - x[0] * x[0])
    };
}

double lineSearchSPI(const std::vector<double>& x0, const std::vector<double>& dir, Func f, double eps_1d) {
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        for (size_t i = 0; i < x.size(); ++i) x[i] += lambda * dir[i];
        return f(x);
    };
    
    // 1. Find the initial 3 points (x1, x2, x3) using your dynamic edging
    // (I am simplifying the bracket finding here for brevity, but you should 
    // ensure x1 < x2 < x3 and f(x2) < f(x1), f(x2) < f(x3))
    double x1 = 0.0;
    double step = 0.01;
    double x2 = x1 + step;
    
    while (f_lambda(x2) > f_lambda(x1)) {
        step /= 2.0;
        x2 = x1 + step;
    }
    
    double x3 = x2 + step;
    while (f_lambda(x3) < f_lambda(x2)) {
        step *= 2.0;
        x1 = x2;
        x2 = x3;
        x3 = x3 + step;
    }

    double f1 = f_lambda(x1), f2 = f_lambda(x2), f3 = f_lambda(x3);
    double x_new = x2;
    double f_new = f2;

    // 2. Successive Parabolic Interpolation Loop
    int max_iter = 100;
    for (int i = 0; i < max_iter; ++i) {
        // Standard formula for parabolic minimum
        double num = std::pow(x2 - x1, 2) * (f2 - f3) - std::pow(x2 - x3, 2) * (f2 - f1);
        double den = (x2 - x1) * (f2 - f3) - (x2 - x3) * (f2 - f1);

        // Safeguard against division by zero (collinear points)
        if (std::abs(den) < 1e-10) break; 

        double x_prev = x_new;
        x_new = x2 - 0.5 * (num / den);
        f_new = f_lambda(x_new);

        // Convergence check
        if (std::abs(x_new - x_prev) < eps_1d) break;

        // Update the points (Keep the bracket tight around the lowest values)
        if (x_new > x2) {
            if (f_new < f2) { x1 = x2; f1 = f2; x2 = x_new; f2 = f_new; }
            else { x3 = x_new; f3 = f_new; }
        } else {
            if (f_new < f2) { x3 = x2; f3 = f2; x2 = x_new; f2 = f_new; }
            else { x1 = x_new; f1 = f_new; }
        }
    }
    
    return x_new;
}

// One-Dimensional Search
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

std::vector<double> steepestDescent(std::vector<double> x, Func f, Grad g, double eps, double eps1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }

    int n = x.size();

    for (int iter = 0; iter < maxIter; ++iter) {
        std::vector<double> grad = g(x);
        
        std::vector<double> s = grad * (-1.0 / norm(grad));
        double lambda = lineSearch(x, s, f, eps1d);
        
        for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
        log_file << '\n';
        
        if (norm(s * lambda) < eps) {
            std::cout << "Convergence at iteration " << iter + 1 << std::endl;
            return x;
        }

        x = x + s * lambda;
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

            if (norm(grad) < eps) {
                std::cout << "Convergence at iteration " << iter + 1 << " and resets count " << reset << std::endl;
                return x;
            }
        }
    }

    log_file.close();

    return x;
}

std::vector<double> conjugateGradientPR(std::vector<double> x, Func f, Grad g, double eps, double eps1d, int maxIter, int maxRestart, std::string log_file_name = "") {
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

    for (int iter = 0; iter < maxIter; ++iter) {
        double lambda = lineSearch(x, s, f, eps1d);
        std::vector<double> x_next = x + s * lambda;
        std::vector<double> grad_next = g(x_next);
        
        std::vector<double> grad_diff = grad_next - grad;
        double omega = dot(grad_next, grad_diff) / dot(grad, grad);
        
        for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
        log_file << '\n';

        if (iter % n == 0 && iter > 0) {
            s = grad_next * -1.0;
        } else {
            s = (grad_next * -1.0) + (s * omega);
        }
        x = x_next;
        grad = grad_next;

        if (norm(grad) < eps) {
            std::cout << "Convergence at iteration " << iter + 1 << std::endl;
            return x;
        }
    }

    log_file.close();

    return x;
}

std::vector<double> broydenMethod(std::vector<double> x, Func f, Grad g, double eps, double eps1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }
    
    int n = x.size();
    std::vector<std::vector<double>> H(n, std::vector<double>(n, 0.0));
    for(int i=0; i<n; ++i) H[i][i] = 1.0;

    for (int iter = 0; iter < maxIter; ++iter) {
        std::vector<double> grad = g(x);
        std::vector<double> s(n, 0.0);
        for(int i=0; i<n; ++i) 
            for(int j=0; j<n; ++j) s[i] -= H[i][j] * grad[j];

        double lambda = lineSearch(x, s, f, eps1d);
        std::vector<double> x_next = x + s * lambda;
        std::vector<double> delta_x = x_next - x;
        std::vector<double> delta_g = g(x_next) - grad;

        std::vector<double> H_delta_g(n, 0.0);
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                H_delta_g[i] += H[i][j] * delta_g[j];
            }
        }

        std::vector<double> v(n, 0.0);
        for(int i = 0; i < n; ++i) {
            v[i] = delta_x[i] - H_delta_g[i];
        }

        double denom = dot(v, delta_g);
        if (abs(denom) > 1e-10) { 
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    H[i][j] += (v[i] * v[j]) / denom; 
                }
            }
        }

        for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
        log_file << '\n';

        x = x_next;
        
        if (norm(grad) < eps) {
            std::cout << "Convergence at iteration " << iter + 1 << std::endl;
            return x;
        }
    }

    log_file.close();

    return x;
}

std::vector<double> broydenMethodDFP(std::vector<double> x, Func f, Grad g, double eps, double eps1d, int maxIter, std::string log_file_name = "") {
    std::ofstream log_file;
    if (log_file_name.length() > 0) {
        log_file.open(log_file_name);
        if (!log_file.is_open()) {
            std::cerr << "Could not open file " << log_file_name << std::endl;
            return {};
        }
    }
    
    int n = x.size();
    std::vector<std::vector<double>> H(n, std::vector<double>(n, 0.0));
    for(int i=0; i<n; ++i) H[i][i] = 1.0;

    for (int iter = 0; iter < maxIter; ++iter) {
        std::vector<double> grad = g(x);
        std::vector<double> s(n, 0.0);
        for(int i = 0; i < n; ++i) 
            for(int j = 0; j < n; ++j) s[i] -= H[i][j] * grad[j]; 

        double lambda = lineSearch(x, s, f, eps1d);
        std::vector<double> x_next = x + s * lambda;
        
        std::vector<double> delta_x = x_next - x; 
        std::vector<double> delta_g = g(x_next) - grad; 

        double dot_xg = dot(delta_x, delta_g);

        std::vector<double> H_dg(n, 0.0);
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                H_dg[i] += H[i][j] * delta_g[j];
            }
        }

        double dot_gHg = dot(delta_g, H_dg);

        if (abs(dot_xg) > 1e-10 && abs(dot_gHg) > 1e-10) { 
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    double term1 = (delta_x[i] * delta_x[j]) / dot_xg;
                    double term2 = (H_dg[i] * H_dg[j]) / dot_gHg;
                    H[i][j] += term1 - term2; 
                }
            }
        }

        for (int j = 0; j < n; ++j) log_file << x[j] << ' ';
        log_file << '\n';

        x = x_next;
        
        if (norm(grad) < eps) {
            std::cout << "Convergence at iteration " << iter + 1 << std::endl;
            return x;
        }
    }

    log_file.close();

    return x;
}

int main() {
    std::cout << std::fixed << std::setprecision(8);
    
    double eps = 1e-4;
    double eps_1d = 1e-8;
    int maxIter = 100000, maxReset = 100;

    std::vector<double> x0_quad = {0.0, 0.0};
    std::vector<double> x0_ros = {-1.2, 1.0};
    std::vector<double> x0_ros2 = {-0.5, 3};

    std::cout << "Minimization of Quadratic Function\n";
    std::cout << "Starting point: [0.0, 0.0]\n";
    std::cout << "Expected minimum: [3.0, 7.0]\n\n";
    
    std::vector<double> res1 = steepestDescent(x0_quad, quadraticFunc, grad_quad, eps, eps_1d, maxIter, "sd_quad.txt");
    std::cout << "Grad Descent Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << quadraticFunc(res1) << '\n' << std::endl;

    std::vector<double> res2 = conjugateGradientFR(x0_quad, quadraticFunc, grad_quad, eps, eps_1d, maxIter, maxReset, "cg_quad.txt");
    std::cout << "Flatcher-Rieves Result: [" << res2[0] << ", " << res2[1] 
              << "] F = " << quadraticFunc(res2) << '\n' << std::endl;
    
    std::vector<double> res3 = conjugateGradientPR(x0_quad, quadraticFunc, grad_quad, eps, eps_1d, maxIter, maxReset, "cgpr_quad.txt");
    std::cout << "Polak-Ribier Result: [" << res3[0] << ", " << res3[1] 
              << "] F = " << quadraticFunc(res3) << '\n' << std::endl;

    std::vector<double> res4 = broydenMethod(x0_quad, quadraticFunc, grad_quad, eps, eps_1d, maxIter, "bm_quad.txt");
    std::cout << "Broiden Result: [" << res4[0] << ", " << res4[1] 
              << "] F = " << quadraticFunc(res4) << '\n' << std::endl;

    std::vector<double> res5 = broydenMethodDFP(x0_quad, quadraticFunc, grad_quad, eps, eps_1d, maxIter, "bmdpf_quad.txt");
    std::cout << "DFP Result: [" << res5[0] << ", " << res5[1] 
              << "] F = " << quadraticFunc(res5) << '\n' << std::endl;

    std::cout << "\n\nMinimization of Rosenbrock Function\n";
    std::cout << "Starting point: [-1.2, 1.0]\n";
    std::cout << "Expected minimum: [1.0, 1.0]\n\n";
    
    res1 = steepestDescent(x0_ros, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "sd_rosen.txt");
    std::cout << "Grad Descent Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << rosenbrockFunc(res1) << '\n' << std::endl;

    res2 = conjugateGradientFR(x0_ros, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, maxReset, "cg_rosen.txt");
    std::cout << "Flatcher-Rieves Result: [" << res2[0] << ", " << res2[1] 
              << "] F = " << rosenbrockFunc(res2) << '\n' << std::endl;

    res3 = conjugateGradientPR(x0_ros, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, maxReset, "cgpr_rosen.txt");
    std::cout << "Polak-Ribier Result: [" << res3[0] << ", " << res3[1] 
              << "] F = " << rosenbrockFunc(res3) << '\n' << std::endl;

    res4 = broydenMethod(x0_ros, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "bm_rosen.txt");
    std::cout << "Broiden Result: [" << res4[0] << ", " << res4[1] 
              << "] F = " << rosenbrockFunc(res4) << '\n' << std::endl;
    
    res5 = broydenMethodDFP(x0_ros, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "bmdpf_rosen.txt");
    std::cout << "DFP Result: [" << res5[0] << ", " << res5[1] 
              << "] F = " << rosenbrockFunc(res5) << '\n' << std::endl;

    std::cout << "\n\nMinimization of Rosenbrock Function\n";
    std::cout << "Starting point: [-0.5, 3]\n";
    std::cout << "Expected minimum: [1.0, 1.0]\n\n";
    
    res1 = steepestDescent(x0_ros2, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "sd_rosen2.txt");
    std::cout << "Grad Descent Result: [" << res1[0] << ", " << res1[1] 
              << "] F = " << rosenbrockFunc(res1) << '\n' << std::endl;

    res2 = conjugateGradientFR(x0_ros2, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, maxReset, "cg_rosen2.txt");
    std::cout << "Flatcher-Rieves Result: [" << res2[0] << ", " << res2[1] 
              << "] F = " << rosenbrockFunc(res2) << '\n' << std::endl;

    res3 = conjugateGradientPR(x0_ros2, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, maxReset, "cgpr_rosen2.txt");
    std::cout << "Polak-Ribier Result: [" << res3[0] << ", " << res3[1] 
              << "] F = " << rosenbrockFunc(res3) << '\n' << std::endl;

    res4 = broydenMethod(x0_ros2, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "bm_rosen2.txt");
    std::cout << "Broiden Result: [" << res4[0] << ", " << res4[1] 
              << "] F = " << rosenbrockFunc(res4) << '\n' << std::endl;
    
    res5 = broydenMethodDFP(x0_ros2, rosenbrockFunc, grad_rosen, eps, eps_1d, maxIter, "bmdpf_rosen2.txt");
    std::cout << "DFP Result: [" << res5[0] << ", " << res5[1] 
              << "] F = " << rosenbrockFunc(res5) << '\n' << std::endl;

    return 0;
}