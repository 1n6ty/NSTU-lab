#include <iostream>
#include <vector>
#include <cmath>

#include "grid/grid.h"
#include "cubic/cubic.h"

double f(double x){
    return std::exp(x);
}

int main(){
    std::vector<double> grid;

    grid = generate_grid(0.01, 0.37, 3);
    std::cout << "Task for h = " << (0.37 - 0.01) / 2 << std::endl;

    std::cout << "Grid: ";
    for(auto i: grid) std::cout << i << ' ';
    std::cout << std::endl;

    std::cout << "e^x: ";
    for(auto i: grid) std::cout << f(i) << ' ';
    std::cout << '\n' << std::endl;

    Cubic spline_1(f, grid);


    grid = generate_grid(0.01, 0.37, 5);
    std::cout << "Task for h = " << (0.37 - 0.01) / 4 << std::endl;

    std::cout << "Grid: ";
    for(auto i: grid) std::cout << i << ' ';
    std::cout << std::endl;

    std::cout << "e^x: ";
    for(auto i: grid) std::cout << f(i) << ' ';
    std::cout << '\n' << std::endl;

    Cubic spline_2(f, grid);


    grid = generate_grid(0.01, 0.37, 9);
    std::cout << "Task for h = " << (0.37 - 0.01) / 8 << std::endl;

    std::cout << "Grid: ";
    for(auto i: grid) std::cout << i << ' ';
    std::cout << std::endl;

    std::cout << "e^x: ";
    for(auto i: grid) std::cout << f(i) << ' ';
    std::cout << '\n' << std::endl;

    Cubic spline_3(f, grid);


    std::cout << "Grid with other points: ";
    std::vector<double> grid_other;
    for(size_t i = 0; i < grid.size() - 1; i++){
        grid_other.push_back((grid[i] + grid[i + 1]) / 2);
        std::cout << grid_other[i] << ' ';
    }
    std::cout << std::endl;
    std::cout << "e^x other: ";
    for(auto i: grid_other) std::cout << f(i) << ' ';
    std::cout << '\n' << std::endl;


    std::cout << "Spline_1 other: \n";
    std::cout << "Value: ";
    for(auto i: grid_other) std::cout << spline_1(i) << ' ';
    std::cout << '\n';
    std::cout << "Error: ";
    for(auto i: grid_other) std::cout << std::abs(spline_1(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "df: ";
    for(auto i: grid_other) std::cout << spline_1.df(i) << ' ';
    std::cout << '\n';
    std::cout << "Error df: ";
    for(auto i: grid_other) std::cout << std::abs(spline_1.df(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "d2f: ";
    for(auto i: grid_other) std::cout << spline_1.d2f(i) << ' ';
    std::cout << '\n';
    std::cout << "Error d2f: ";
    for(auto i: grid_other) std::cout << std::abs(spline_1.d2f(i) - f(i)) << ' ';
    std::cout << '\n' << std::endl;

    std::cout << "Spline_2 other: \n";
    std::cout << "Value: ";
    for(auto i: grid_other) std::cout << spline_2(i) << ' ';
    std::cout << '\n';
    std::cout << "Error: ";
    for(auto i: grid_other) std::cout << std::abs(spline_2(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "df: ";
    for(auto i: grid_other) std::cout << spline_2.df(i) << ' ';
    std::cout << '\n';
    std::cout << "Error df: ";
    for(auto i: grid_other) std::cout << std::abs(spline_2.df(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "d2f: ";
    for(auto i: grid_other) std::cout << spline_2.d2f(i) << ' ';
    std::cout << '\n';
    std::cout << "Error d2f: ";
    for(auto i: grid_other) std::cout << std::abs(spline_2.d2f(i) - f(i)) << ' ';
    std::cout << '\n' << std::endl;

    std::cout << "Spline_3 other: \n";
    std::cout << "Value: ";
    for(auto i: grid_other) std::cout << spline_3(i) << ' ';
    std::cout << '\n';
    std::cout << "Error: ";
    for(auto i: grid_other) std::cout << std::abs(spline_3(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "df: ";
    for(auto i: grid_other) std::cout << spline_3.df(i) << ' ';
    std::cout << '\n';
    std::cout << "Error df: ";
    for(auto i: grid_other) std::cout << std::abs(spline_3.df(i) - f(i)) << ' ';
    std::cout << '\n';
    std::cout << "d2f: ";
    for(auto i: grid_other) std::cout << spline_3.d2f(i) << ' ';
    std::cout << '\n';
    std::cout << "Error d2f: ";
    for(auto i: grid_other) std::cout << std::abs(spline_3.d2f(i) - f(i)) << ' ';
    std::cout << '\n' << std::endl;

    return 0;
}