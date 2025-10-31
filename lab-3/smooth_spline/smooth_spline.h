#pragma once

#include <vector>

class Smooth_Spline {

    public:
        
        Smooth_Spline(std::vector<double> x, std::vector<double> y, std::vector<double> weights, double p = 0.5);
        Smooth_Spline(std::vector<double> x, std::vector<double> y, double p = 0.5);

        double operator()(double x_val);

    private:
        
        std::vector<double> m_x;
        std::vector<double> m_g;

        static std::vector<double> thomas_algorithm(
            const std::vector<double>& lower_diag,
            const std::vector<double>& main_diag,
            const std::vector<double>& upper_diag,
            const std::vector<double>& right_side
        );
};