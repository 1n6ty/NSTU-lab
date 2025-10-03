#include <vector>
#include <algorithm>
#include <stdexcept>

#include "cubic.h"

Cubic::Cubic(const std::function<double(double)> &f, const std::vector<double>& grid): grid(grid){
    size_t grid_size = grid.size();

    std::vector<double> f_grid(grid_size);
    for(size_t i = 0; i < grid_size; i++) f_grid[i] = f(grid[i]);
    
    std::vector<double> h(grid_size - 1);
    for(size_t i = 0; i < grid_size - 1; i++) h[i] = grid[i + 1] - grid[i];
    
    std::vector<double> main_diag(grid_size, 0),
                        lower_diag(grid_size, 0),
                        upper_diag(grid_size, 0),
                        right_side(grid_size, 0);
    
    main_diag[0] = 1.0;
    for (size_t i = 1; i < grid_size - 1; i++) {
        lower_diag[i] = h[i - 1];
        upper_diag[i] = h[i];
        main_diag[i] = 2.0 * (h[i - 1] + h[i]);
        right_side[i] = 6.0 * ((f_grid[i + 1] - f_grid[i]) / h[i] - (f_grid[i] - f_grid[i - 1]) / h[i - 1]);
    }
    main_diag[grid_size - 1] = 1.0;
    
    std::vector<double> sol = this->thomas_algorithm(lower_diag, main_diag, upper_diag, right_side);
    
    a.reserve(grid_size - 1);
    b.reserve(grid_size - 1);
    c.reserve(grid_size - 1);
    d.reserve(grid_size - 1);
    for (size_t i = 0; i < grid_size - 1; i++) {
        a[i] = f_grid[i];
        b[i] = (f_grid[i + 1] - f_grid[i]) / h[i] - h[i] * (2 * sol[i] + sol[i + 1]) / 6;
        c[i] = sol[i] / 2;
        d[i] = (sol[i + 1] - sol[i]) / (6.0 * h[i]);
    }
}

double Cubic::operator()(const double x){
    size_t left = 0, right = grid.size() - 1;
    
    while(left <= right){
        size_t mid = (right + left) / 2;

        if(x < this->grid[mid]){
            right = mid - 1;
        } else if(x > this->grid[mid + 1]){
            left = mid + 1;
        } else {
            double dx = x - this->grid[mid];
            return a[mid] + b[mid] * dx + c[mid] * dx * dx + d[mid] * dx * dx * dx;
        }
    }

    throw std::runtime_error("Interval not found");
}

double Cubic::df(double x){
    size_t left = 0, right = grid.size() - 1;
    
    while(left <= right){
        size_t mid = (right + left) / 2;

        if(x < this->grid[mid]){
            right = mid - 1;
        } else if(x > this->grid[mid + 1]){
            left = mid + 1;
        } else {
            double dx = x - this->grid[mid];
            return b[mid] + 2 * c[mid] * dx + 3 * d[mid] * dx * dx;
        }
    }

    throw std::runtime_error("Interval not found");
}

double Cubic::d2f(double x){
    size_t left = 0, right = grid.size() - 1;
    
    while(left <= right){
        size_t mid = (right + left) / 2;

        if(x < this->grid[mid]){
            right = mid - 1;
        } else if(x > this->grid[mid + 1]){
            left = mid + 1;
        } else {
            double dx = x - this->grid[mid];
            return 2 * c[mid] + 6 * d[mid] * dx;
        }
    }

    throw std::runtime_error("Interval not found");
}

std::vector<double> Cubic::thomas_algorithm(
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