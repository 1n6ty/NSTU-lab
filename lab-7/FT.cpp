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

std::vector<std::complex<double>> FT::FFT(const std::vector<std::complex<double>> &signal){
    size_t N = signal.size();

    std::vector<std::complex<double>> u, v;
    if(N % 2 == 0){
        for(size_t i = 0; i < N / 2; i++){
            u.push_back(signal[2 * i]);
            v.push_back(signal[2 * i + 1]);
        }
        u = FFT(u);
        v = FFT(v);
    } else {
        return FT::DFT(signal);
    }

    const std::complex<double> im(0., 1.);

    std::vector<std::complex<double>> result;
    
    std::complex<double> angle;
    for(size_t m = 0; m < N / 2; m++){
        angle = -2. * M_PI * static_cast<double>(m) / N;
        result.push_back(u[m] + std::exp(angle * im) * v[m]);
    }
    for(size_t m = 0; m < N / 2; m++){
        angle = -2. * M_PI * static_cast<double>(m) / N;
        result.push_back(u[m] - std::exp(angle * im) * v[m]);
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