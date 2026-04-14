#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>
#include <functional>

using Func = std::function<double(const std::vector<double>&)>;

int eval_count = 0;

double target_function(const std::vector<double>& x) {
    eval_count++;

    // Restricting area, fine if we leave
    if (x[0] < -10.0 || x[0] > 10.0 || x[1] < -10.0 || x[1] > 10.0) {
        return -1e9;
    }

    const double C[6] = {2, 3, 8, 3, 2, 8};
    const double a[6] = {3, -5, 0, 3, -4, 6};
    const double b[6] = {-4, -6, -1, 7, 0, 5};

    double sum = 0.0;
    for (int i = 0; i < 6; ++i) {
        sum += C[i] / (1.0 + std::pow(x[0] - a[i], 2) + std::pow(x[1] - b[i], 2));
    }
    return sum;
}

double negative_target(const std::vector<double>& x) {
    return -target_function(x);
}

double lineSearch(const std::vector<double>& x0, const std::vector<double>& dir, Func f, double eps_1d) {
    auto f_lambda = [&](double lambda) {
        std::vector<double> x = x0;
        for (size_t i = 0; i < x.size(); ++i) x[i] += lambda * dir[i];
        return f(x);
    };

    double h = 0.01;
    double lambda0 = 0.0;
    double f_minus = f_lambda(lambda0 - h);
    double f0 = f_lambda(lambda0);
    double f_plus = f_lambda(lambda0 + h);
    double a, b;

    if (f_minus >= f0 && f_plus >= f0) {
        a = lambda0 - h; b = lambda0 + h;
    } else {
        double step = h, l_prev = lambda0, l_curr = lambda0, f_curr = f0;
        if (f_minus < f0) { step = -h; l_curr = lambda0 - h; f_curr = f_minus; }
        else { step = h; l_curr = lambda0 + h; f_curr = f_plus; }

        while (true) {
            step *= 2.0;
            double l_next = l_curr + step;
            double f_next = f_lambda(l_next);
            if (f_next >= f_curr) {
                a = std::min(l_prev, l_next); b = std::max(l_prev, l_next);
                break;
            }
            l_prev = l_curr; l_curr = l_next; f_curr = f_next;
        }
    }

    double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    double x1 = b - (b - a) / phi, x2 = a + (b - a) / phi;
    double f1 = f_lambda(x1), f2 = f_lambda(x2);

    while (std::abs(b - a) > eps_1d) {
        if (f1 < f2) {
            b = x2; x2 = x1; f2 = f1; x1 = b - (b - a) / phi; f1 = f_lambda(x1);
        } else {
            a = x1; x1 = x2; f1 = f2; x2 = a + (b - a) / phi; f2 = f_lambda(x2);
        }
    }
    return (a + b) / 2.0;
}

std::vector<double> methodGauss(std::vector<double> x0, Func f, double eps, double eps_1d, int maxIter, std::ofstream &log_file) {
    std::vector<double> x = x0;
    int n = static_cast<int>(x.size());
    for (int k = 0; k < maxIter; ++k) {
        std::vector<double> x_prev = x;
        for (int i = 0; i < n; ++i) {
            std::vector<double> dir(n, 0.0);
            dir[i] = 1.0;
            double lambda = lineSearch(x, dir, f, eps_1d);
            for (int j = 0; j < n; ++j) x[j] += lambda * dir[j];

            log_file << "(" << x[0] << "," << x[1] << ")" << " ";
        }
        double diff = 0.0, diff_x = 0.0;
        for (int i = 0; i < n; ++i) {
            diff_x += std::pow(x[i] - x_prev[i], 2);
            diff += std::pow(f(x) - f(x_prev), 2);
        }
        if (std::sqrt(diff) < eps && std::sqrt(diff_x) < eps) break;
    }
    return x;
}


// --------------------------------------------------------------------------------------------------


std::vector<double> getRandomPoint(std::mt19937& gen) {
    std::uniform_real_distribution<double> dist(-10.0, 10.0);
    return {dist(gen), dist(gen)};
}

void simpleRandomSearch(int N, std::mt19937& gen) {
    std::ofstream alglog("path.txt");

    eval_count = 0;
    std::vector<double> best_x = getRandomPoint(gen);
    double best_f = target_function(best_x);

    for (int i = 1; i < N; ++i) {
        std::vector<double> x = getRandomPoint(gen);
        double f = target_function(x);
        if (f > best_f) {
            best_f = f;
            best_x = x;
        }

        alglog << "(" << x[0] << "," << x[1] << ")" << '\n';
    }
    std::cout << "Simple random search (N=" << N << "): f=" << std::fixed << std::setprecision(15) << best_f
              << " in point (" << best_x[0] << ", " << best_x[1] << "). Function evals: " << eval_count << "\n";

    alglog.close();
}

void globalAlgorithm1(int m, std::mt19937& gen) {
    std::ofstream alglog("path_1.txt");

    eval_count = 0;
    std::vector<double> best_x;
    double best_f = -1e9;
    int failures = 0;

    while (failures < m) {
        std::vector<double> start_x = getRandomPoint(gen);

        alglog << "(" << start_x[0] << "," << start_x[1] << ")" << ' ';

        std::vector<double> local_opt = methodGauss(start_x, negative_target, 1e-4, 1e-4, 1000, alglog);

        alglog << '\n';

        double local_f = target_function(local_opt);

        if (local_f > best_f + 1e-4) {
            best_f = local_f;
            best_x = local_opt;
            failures = 0;
        } else {
            failures++;
        }
    }
    std::cout << std::fixed << std::setprecision(15)
              << "Algo 1 (m=" << m << "): f=" << best_f << " in point ("
              << best_x[0] << ", " << best_x[1] << "). Function evals: " << eval_count << "\n";

    alglog.close();
}

void globalAlgorithm2(int m, std::mt19937& gen) {
    std::ofstream alglog("path_2.txt");

    eval_count = 0;
    std::vector<double> best_x = methodGauss(getRandomPoint(gen), negative_target, 1e-4, 1e-4, 1000, alglog);

    alglog << '\n';

    double best_f = target_function(best_x);
    int failures = 0;

    while (failures < m) {
        std::vector<double> rand_x = getRandomPoint(gen);

        alglog << "(" << rand_x[0] << "," << rand_x[1] << ")" << ' ';

        if (target_function(rand_x) > best_f) {
            std::vector<double> local_opt = methodGauss(rand_x, negative_target, 1e-4, 1e-4, 1000, alglog);
            double local_f = target_function(local_opt);
            if (local_f > best_f + 1e-4) {
                best_f = local_f;
                best_x = local_opt;
                failures = 0;
            } else {
                failures++;
            }
        } else {
            failures++;
        }

        alglog << '\n';
    }
    std::cout << std::fixed << std::setprecision(15)
              << "Algo 2 (m=" << m << "): f=" << best_f << " in point ("
              << best_x[0] << ", " << best_x[1] << "). Function evals: " << eval_count << "\n";
    
    alglog.close();
}

void globalAlgorithm3(int m, std::mt19937& gen) {
    std::ofstream alglog("path_3.txt");

    eval_count = 0;
    std::vector<double> best_x = methodGauss(getRandomPoint(gen), negative_target, 1e-4, 1e-4, 1000, alglog);

    double best_f = target_function(best_x);
    int failures = 0;
    std::uniform_real_distribution<double> angle_dist(0.0, 2 * M_PI);

    while (failures < m) {
        double angle = angle_dist(gen);
        std::vector<double> dir = {std::cos(angle), std::sin(angle)};
        std::vector<double> curr_x = best_x;
        double curr_f = best_f;
        double step = 0.5;

        // Moving untill we going up
        bool escaped = false;
        while (true) {
            std::vector<double> next_x = {curr_x[0] + step * dir[0], curr_x[1] + step * dir[1]};
            if (next_x[0] < -10 || next_x[0] > 10 || next_x[1] < -10 || next_x[1] > 10) break;

            double next_f = target_function(next_x);
            if (next_f > curr_f) { escaped = true; break; }

            curr_x = next_x;
            curr_f = next_f;

            alglog << "(" << curr_x[0] << "," << curr_x[1] << ")" << ' ';
        }

        if (!escaped) {
            failures++;
            continue;
        }

        std::vector<double> local_opt = methodGauss(curr_x, negative_target, 1e-4, 1e-4, 1000, alglog);
        double local_f = target_function(local_opt);

        if (local_f > best_f + 1e-4) {
            best_f = local_f;
            best_x = local_opt;
            failures = 0;
        } else {
            failures++;
        }
    }
    std::cout << std::fixed << std::setprecision(15)
              << "Algo 3 (m=" << m << "): f=" << best_f << " in point ("
              << best_x[0] << ", " << best_x[1] << "). Function evals: " << eval_count << "\n";

    alglog.close();
}

int main() {
    std::mt19937 gen(42);

    std::cout << "=== Simple Random Search Research ===\n";
    simpleRandomSearch(92103, gen);
    simpleRandomSearch(18420679, gen);
    simpleRandomSearch(184205, gen);

    std::cout << "\n=== Global search research ===\n";
    std::vector<int> m_values = {5, 10, 20};

    for (int m : m_values) {
        std::cout << "--- m = " << m << " ---\n";
        globalAlgorithm1(m, gen);
        globalAlgorithm2(m, gen);
        globalAlgorithm3(m, gen);
    }

    return 0;
}