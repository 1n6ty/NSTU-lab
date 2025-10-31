#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "smooth_spline/smooth_spline.h"

std::vector<double> generate_normal(size_t count, double mean, double stddev){
    std::random_device rd;
    std::mt19937 generator(rd());

    std::normal_distribution<> distribution(mean, stddev);

    std::vector<double> data;
    for(size_t i = 0; i < count; i++) data.push_back(distribution(generator));

    return data;
}

int main() {
    std::cout.precision(10); 
    
    const size_t N = 1157;

    std::vector<double> x;
    for(size_t i = 0; i < N; i++) x.push_back(static_cast<double>(i));

    std::vector<double> d = generate_normal(N, 1.05, 5.5);
    std::vector<double> w_uniform(N, 1.0);

    Smooth_Spline sp_0 = Smooth_Spline(x, d, w_uniform, 0.0);
    Smooth_Spline sp_4 = Smooth_Spline(x, d, w_uniform, 0.4);
    Smooth_Spline sp_8 = Smooth_Spline(x, d, w_uniform, 0.8);
    Smooth_Spline sp_99 = Smooth_Spline(x, d, w_uniform, 0.99);

    std::vector<double> w_variable = w_uniform;
    for(size_t i = 200; i < 700; i++) w_variable[i] = 0.5;

    Smooth_Spline sp_0w = Smooth_Spline(x, d, w_variable, 0.0);
    Smooth_Spline sp_4w = Smooth_Spline(x, d, w_variable, 0.4);
    Smooth_Spline sp_8w = Smooth_Spline(x, d, w_variable, 0.8);
    Smooth_Spline sp_99w = Smooth_Spline(x, d, w_variable, 0.99);


    std::ofstream file_uniform("spline_uniform_weights.txt");
    if(!file_uniform.is_open()){
        std::cerr << "Error spline_uniform_weights.txt\n";
        return 1;
    }

    for(size_t i = 0; i < N; i++) file_uniform << x[i] << ' ';
    file_uniform << '\n';
    for(size_t i = 0; i < N; i++) file_uniform << d[i] << ' ';
    file_uniform << '\n';
    for(size_t i = 0; i < N; i++) file_uniform << sp_0(x[i]) << ' ';
    file_uniform << '\n';
    for(size_t i = 0; i < N; i++) file_uniform << sp_4(x[i]) << ' ';
    file_uniform << '\n';
    for(size_t i = 0; i < N; i++) file_uniform << sp_8(x[i]) << ' ';
    file_uniform << '\n';
    for(size_t i = 0; i < N; i++) file_uniform << sp_99(x[i]) << ' ';
    file_uniform << '\n';

    file_uniform.close();


    std::ofstream file_variable("spline_variable_weights.txt");
    if(!file_variable.is_open()){
        std::cerr << "Error spline_variable_weights.txt\n";
        return 1;
    }

    for(size_t i = 0; i < N; i++) file_variable << x[i] << ' ';
    file_variable << '\n';
    for(size_t i = 0; i < N; i++) file_variable << d[i] << ' ';
    file_variable << '\n';
    for(size_t i = 0; i < N; i++) file_variable << sp_0w(x[i]) << ' ';
    file_variable << '\n';
    for(size_t i = 0; i < N; i++) file_variable << sp_4w(x[i]) << ' ';
    file_variable << '\n';
    for(size_t i = 0; i < N; i++) file_variable << sp_8w(x[i]) << ' ';
    file_variable << '\n';
    for(size_t i = 0; i < N; i++) file_variable << sp_99w(x[i]) << ' ';
    file_variable << '\n';

    file_variable.close();

    return 0;
}