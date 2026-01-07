#pragma once

#include <vector>
#include <complex>

class FT {
public:
    static std::vector<std::complex<double>> DFT(const std::vector<std::complex<double>> &signal);
    static std::vector<std::complex<double>> IDFT(const std::vector<std::complex<double>> &signal);

    static std::vector<std::complex<double>> FFT(const std::vector<std::complex<double>> &signal);
    static std::vector<std::complex<double>> IFFT(const std::vector<std::complex<double>> &signal);
};