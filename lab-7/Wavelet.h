#pragma once

#include <vector>
#include <complex>

#include "FT.h"

class Wavelet {
public:
    static std::vector<std::complex<double>> psi(const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j, const size_t k);
    static std::vector<std::complex<double>> phi(const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j, const size_t k);

    static std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> coefs(const std::vector<std::complex<double>>& signal, const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j);
};
