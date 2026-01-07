#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

struct LotkaVolterra {
    double alpha, beta, gamma, delta;

    LotkaVolterra(double a, double b, double g, double d)
    : alpha(a), beta(b), gamma(g), delta(d) {}

    void operator()(double x, double y, double& dxdt, double& dydt) const {
        dxdt = alpha * x - beta * x * y;
        dydt = -gamma * y + delta * x * y;
    }
};

void euler_step(const LotkaVolterra& lv, double& x, double& y, double h) {
    double dxdt, dydt;
    lv(x, y, dxdt, dydt);
    x += h * dxdt;
    y += h * dydt;
}

void rk4_step(const LotkaVolterra& lv, double& x, double& y, double h) {
    double k1x, k1y, k2x, k2y, k3x, k3y, k4x, k4y;

    lv(x, y, k1x, k1y);
    lv(x + 0.5 * h * k1x, y + 0.5 * h * k1y, k2x, k2y);
    lv(x + 0.5 * h * k2x, y + 0.5 * h * k2y, k3x, k3y);
    lv(x + h * k3x, y + h * k3y, k4x, k4y);

    x += (h / 6.0) * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
    y += (h / 6.0) * (k1y + 2.0 * k2y + 2.0 * k3y + k4y);
}

int main(){
    double alpha = 0.3, beta  = 0.015, gamma = 0.6, delta = 0.01;

    double x0 = 70.0, y0 = 10.0, T = 365.0, h = 1.0;
    int steps = static_cast<int>(T / h);

    LotkaVolterra lv(alpha, beta, gamma, delta);
    std::vector<double> t_vals, x_vals, y_vals, x1_vals, y1_vals;
    double t = 0.0, x = x0, y = y0, x1 = x0, y1 = y0;

    t_vals.push_back(t);
    x_vals.push_back(x);
    y_vals.push_back(y);

    for(size_t i = 0; i < steps; i++){
        rk4_step(lv, x, y, h);
        euler_step(lv, x1, y1, h);
        t += h;
        t_vals.push_back(t);
        x_vals.push_back(x);
        y_vals.push_back(y);
        x1_vals.push_back(x1);
        y1_vals.push_back(y1);
    }

    std::ofstream file("lotka_volterra.txt");
    file << "t x y x1 y1\n";
    for(size_t i = 0; i < t_vals.size(); i++){
        file << std::fixed << std::setprecision(6)
             << t_vals[i] << " " << x_vals[i] << " " << y_vals[i] << "\n";
    }
    file.close();
    return 0;
}