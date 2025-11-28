#include "gauss3.h"
#include "../grid/grid.h"

#include <cmath>

double Gauss3::gauss3_inner(const Gauss3::Func& f, double a, double b) {
    const double t[3] = {
        -std::sqrt(3. / 5.), 0, std::sqrt(3. / 5.)
    }, w[3] = { 
        5. / 9.,
        8. / 9.,
        5. / 9.
    };

    double c1 = (b - a) / 2.0,
           c2 = (b + a) / 2.0;

    double sum = 0.;
    for (int i = 0; i < 3; ++i) {
        sum += w[i] * f(c2 + c1 * t[i]);
    }
    return c1 * sum;
}

double Gauss3::gauss3(const Gauss3::Func& f, double a, double b, int n) {
    std::vector<double> grid = generate_grid(a, b, n + 1);
    int grid_size = grid.size();

    double result = 0.;
    for (int i = 0; i < grid_size - 1; ++i) result += Gauss3::gauss3_inner(f, grid[i], grid[i + 1]);

    return result;
}