#pragma once

#include <functional>

class Gauss3 {
public:
    using Func = std::function<double(double)>;

    static double gauss3(const Func& f, double a, double b, int n = 1000);

private:
    static double gauss3_inner(const Func& f, double a, double b);
};