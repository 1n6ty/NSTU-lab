#include "simpson.h"
#include "../grid/grid.h"

#include <vector>
#include <stdexcept>

double Simpson::simpson(const Simpson::Func& f, double a, double b, size_t n) {
    std::vector<double> x = generate_grid(a, b, n + 1);
    
    std::vector<double> h(n);
    for (size_t i = 0; i < n; ++i) h[i] = x[i + 1] - x[i];

    double term_1 = h[0] * f(a) + h[n - 1] * f(b),
           sum_midpoints = 0.,
           sum_nodes = 0.;

    for (size_t i = 0; i < n; ++i) sum_midpoints += h[i] * f((x[i] + x[i + 1]) / 2.);
    double term_2 = 4. * sum_midpoints;

    for (size_t i = 0; i < n - 1; ++i) sum_nodes += (h[i] + h[i + 1]) * f(x[i + 1]);
    double term_3 = sum_nodes;
    
    return (1. / 6.) * (term_1 + term_2 + term_3);
}