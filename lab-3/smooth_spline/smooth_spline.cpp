#include "smooth_spline.h"

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <cmath>

std::vector<double> Smooth_Spline::thomas_algorithm(
    const std::vector<double>& lower_diag,
    const std::vector<double>& main_diag,
    const std::vector<double>& upper_diag,
    const std::vector<double>& right_side
){
    size_t N = right_side.size();
    std::vector<double> result(N);
    
    std::vector<double> upper_diag_ext(N, 0);
    std::vector<double> right_side_ext(N, 0);
    
    upper_diag_ext[0] = upper_diag[0] / main_diag[0];
    right_side_ext[0] = right_side[0] / main_diag[0];

    for(size_t i = 1; i < N; i++){
        double m = 1.0 / (main_diag[i] - lower_diag[i] * upper_diag_ext[i - 1]);
        upper_diag_ext[i] = upper_diag[i] * m;
        right_side_ext[i] = (right_side[i] - lower_diag[i] * right_side_ext[i - 1]) * m;
    }

    result[N - 1] = right_side_ext[N - 1];
    for (int i = N - 2; i >= 0; i--){
        result[i] = right_side_ext[i] - upper_diag_ext[i] * result[i + 1];
    }

    return result;
}

Smooth_Spline::Smooth_Spline(std::vector<double> x, std::vector<double> y, std::vector<double> weights, double p)
    : m_x(x)
{
    size_t n = m_x.size();

    if(n == 0) return;

    if(n == 1) {
        m_g.resize(1);
        m_g[0] = y[0];
        return;
    }

    if(y.size() != n || weights.size() != n) throw std::invalid_argument("Sizes aren't equal!");

    if(p < 0.0 || p > 1.0) throw std::invalid_argument("p isn't in [0, 1]!");

    std::vector<double> h(n - 1);
    for(size_t i = 0; i < n - 1; ++i) {
        h[i] = m_x[i + 1] - m_x[i];
        if (h[i] <= 0.0) throw std::invalid_argument("x must be strictly increasing!");
    }

    std::vector<double> lower_diag(n), main_diag(n), upper_diag(n), right_side(n);

    lower_diag[0] = 0.;
    main_diag[0] = (1. - p) * weights[0] + p / h[0];
    upper_diag[0] = -p / h[0];
    right_side[0] = (1. - p) * weights[0] * y[0];

    for(size_t k = 1; k < n - 1; k++){
        lower_diag[k] = -p / h[k - 1];
        main_diag[k] = (1. - p) * weights[k] + p / h[k - 1] + p / h[k];
        upper_diag[k] = -p / h[k];
        right_side[k] = (1. - p) * weights[k] * y[k];
    }

    lower_diag[n - 1] = -p / h[n - 2];
    main_diag[n - 1] = (1. - p) * weights[n - 1] + p / h[n - 2];
    upper_diag[n - 1] = 0.;
    right_side[n - 1] = (1. - p) * weights[n - 1] * y[n - 1];

    m_g = Smooth_Spline::thomas_algorithm(lower_diag, main_diag, upper_diag, right_side);
}

Smooth_Spline::Smooth_Spline(std::vector<double> x, std::vector<double> y, double p)
    : Smooth_Spline(x, y, std::vector<double>(x.size(), 1.0), p)
{

}

double Smooth_Spline::operator()(double x_val) {
    size_t n = m_x.size();

    if(n == 0) return 0.;
    if(n == 1) return m_g[0];

    if (x_val <= m_x.front()) return m_g.front();
    if (x_val >= m_x.back()) return m_g.back();

    std::vector<double>::iterator it = std::upper_bound(m_x.begin(), m_x.end(), x_val);
    size_t k = std::distance(m_x.begin(), it) - 1;

    double h_k = m_x[k + 1] - m_x[k];

    return m_g[k] * (m_x[k + 1] - x_val) / h_k + m_g[k + 1] * (x_val - m_x[k]) / h_k;
}