#include "integrator/gauss3.h"
#include "integrator/simpson.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>

double f(const double x){
    return std::sin(x);
}

int main(){

    double a = 0.1,
           b = 0.56;
    
    double h = b - a, I, I2, I4, I8, Ish, k;
    Ish = -std::cos(b) + std::cos(a);
    
    I = Gauss3::gauss3(f, a, b, 1);
    I2 = Gauss3::gauss3(f, a, b, 2);

    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));

    std::cout << "\nGauss-3: h = " << h << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I 
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    I = I2;
    I2 = Gauss3::gauss3(f, a, b, 4);
    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));
    std::cout << "\nGauss-3: h / 2 = " << h / 2 << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    I = I2;
    I2 = Gauss3::gauss3(f, a, b, 8);
    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));
    std::cout << "\nGauss-3: h / 4 = " << h / 4 << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    I = Simpson::simpson(f, a, b, 1);
    I2 = Simpson::simpson(f, a, b, 2);

    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));

    std::cout << "\nSimpson: h = " << h << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I 
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    I = I2;
    I2 = Simpson::simpson(f, a, b, 4);
    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));
    std::cout << "\nSimpson: h / 2 = " << h / 2 << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    I = I2;
    I2 = Simpson::simpson(f, a, b, 8);
    k = std::log2(std::abs(1 + (I2 - I) / (Ish - I2)));
    std::cout << "\nSimpson: h / 4 = " << h / 4 << std::endl;
    std::cout << std::scientific << std::setprecision(10)
    << "I* = " << Ish 
    << ";\nIh = " << I 
    << ";\nI* - Ih = " << Ish - I
    << ";\n(I* - Ih) / (I*- I(h/2)) = " << (Ish - I) / (Ish - I2)
    << ";\nk = " << k
    << ";\n(I(h/2) - Ih) / (2^k - 1) = " << (I2 - I) / (std::pow(2, k) - 1)
    << ";\nIR = " << I2 + (I2 - I) / (std::pow(2, k) - 1)
    << ";\nI* - IR = " << Ish - (I2 + (I2 - I) / (std::pow(2, k) - 1))
    << std::endl;

    return 0;
}