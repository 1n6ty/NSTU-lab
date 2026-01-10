#include <vector>
#include <complex>

#include "FT.h"

std::vector<std::complex<double>> FT::DFT(const std::vector<std::complex<double>> &signal){
    size_t N = signal.size();
    const std::complex<double> im(0., 1.);

    std::vector<std::complex<double>> result;

    std::complex<double> sum = 0., angle;
    for(size_t m = 0; m < N; m++){
        for(size_t i = 0; i < N; i++){
            angle = -2. * M_PI * static_cast<double>(i * m)/ N;
            sum += signal[i] * std::exp(im * angle);
        }
        result.push_back(sum);
        sum = 0.;
    }

    return result;
}

std::vector<std::complex<double>> FT::IDFT(const std::vector<std::complex<double>> &signal){
    size_t N = signal.size();
    const std::complex<double> im(0., 1.);

    std::vector<std::complex<double>> result;

    std::complex<double> sum = 0., angle;
    for(size_t m = 0; m < N; m++){
        for(size_t i = 0; i < N; i++){
            angle = 2. * M_PI * static_cast<double>(i * m)/ N;
            sum += signal[i] * std::exp(im * angle);
        }
        result.push_back(sum / static_cast<double>(N));
        sum = 0.;
    }

    return result;
}

void bit_reverse(std::vector<std::complex<double>>& a) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        for(; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if(i < j) std::swap(a[i], a[j]);
    }
}

//Cooley-Tukey algorithm
std::vector<std::complex<double>> FT::FFT(const std::vector<std::complex<double>> &signal) {
    size_t N = signal.size();

    std::vector<std::complex<double>> result(signal);
    bit_reverse(result);

    for(size_t len = 2; len <= N; len <<= 1){
        double ang = 2 * M_PI / len * -1;
        std::complex<double> wlen(std::cos(ang), std::sin(ang));

        for(size_t i = 0; i < N; i += len){
            std::complex<double> w(1);
            for(size_t j = 0; j < len / 2; j++){
                std::complex<double> u = result[i + j], v = result[i + j + len / 2] * w;

                result[i + j] = u + v;
                result[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    return result;
}

std::vector<std::complex<double>> FT::IFFT(const std::vector<std::complex<double>> &signal){
    size_t N = signal.size();

    std::vector<std::complex<double>> result(signal);
    for(auto& x : result) x = std::conj(x);

    result = FFT(result);
    for(auto& x : result) x = std::conj(x) / static_cast<double>(N);

    return result;
}