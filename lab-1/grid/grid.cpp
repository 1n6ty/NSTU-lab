#include "grid.h"
#include <cmath>
#include <vector>

std::vector<double> generate_grid(const double a, const double b, const size_t k, const double r){
    std::vector<double> grid;
    grid.reserve(k);
    grid.push_back(a);

    double h;
    if(r - 1 < 1e-15){
        h = (b - a) / (k - 1);
    } else{
        h = (b - a) * (r - 1) / (std::pow(r, k - 1) - 1);
    }

    for(size_t i = 1; i < k - 1; ++i){
        grid.push_back(grid.at(i - 1) + h);
        h *= r;
    }

    grid.push_back(b);
    return grid;
}