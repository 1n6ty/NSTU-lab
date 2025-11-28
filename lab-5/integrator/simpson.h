#pragma once

#include <functional>

#include "../grid/grid.h"

class Simpson {
public:
    using Func = std::function<double(double)>;

    static double simpson(const Func& f, double a, double b, size_t n = 1000);
};
