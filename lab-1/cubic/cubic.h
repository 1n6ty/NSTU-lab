#pragma once

#include <vector>
#include <functional>

class Cubic{
    public:
        Cubic(const std::function<double(double)> &f, const std::vector<double>& grid);
        double operator()(double x);
        double df(double x);
        double d2f(double x);
    private:
        std::vector<double> a, b, c, d, grid;
        static std::vector<double> thomas_algorithm(
            const std::vector<double>& lower_d,
            const std::vector<double>& main_d,
            const std::vector<double>& upper_d,
            const std::vector<double>& right_f
        );
};