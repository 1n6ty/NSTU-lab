#include <vector>
#include <complex>
#include <utility>

#include "FT.h"
#include "Wavelet.h"

std::vector<std::complex<double>> Wavelet::psi(const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j, const size_t k){
    const size_t N = u_tr.size();
    const std::complex<double> im(0., 1.);

    std::vector<std::complex<double>> result;
    for(size_t n = 0; n < N; n++){
        std::complex<double> mul(1., 0.);
        double angle = -2. * M_PI * n * static_cast<double>(k << j) / N;
        for(size_t i = 0; i < j - 1; i++) mul *= u_tr[(n << i) % N];
        mul *= std::exp(im * angle) * v_tr[(n << (j - 1)) % N];
        result.push_back(mul);
    }

    return FT::IFFT(result);
}

std::vector<std::complex<double>> Wavelet::phi(const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j, const size_t k){
    const size_t N = u_tr.size();
    const std::complex<double> im(0., 1.);

    std::vector<std::complex<double>> result;
    for(size_t n = 0; n < N; n++){
        std::complex<double> mul(1., 0.);
        double angle = -2. * M_PI * n * static_cast<double>(k << j) / N;
        for(size_t i = 0; i < j; i++) mul *= u_tr[(n << i) % N];
        mul *= std::exp(im * angle);
        result.push_back(mul);
    }

    return FT::IFFT(result);
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> get_wavelet_spectra(const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, size_t j){
    size_t N = u_tr.size();
    std::vector<std::complex<double>> psi_spec(N), phi_spec(N);

    for(size_t n = 0; n < N; n++){
        std::complex<double> product(1., 0.);

        for(size_t i = 0; i < j; i++){
            if (i == j - 1) psi_spec[n] = product * v_tr[(n << (j - 1)) % N];
            product *= u_tr[(n << i) % N];
        }
        phi_spec[n] = product;
    }

    return {psi_spec, phi_spec};
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> Wavelet::coefs(const std::vector<std::complex<double>>& signal, const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, const size_t j){
    const size_t N = signal.size();
    const size_t stride = size_t(1) << j;
    const size_t coef_count = N / stride;

    std::vector<std::complex<double>> sig_tr = FT::FFT(signal);

    auto spectra = get_wavelet_spectra(u_tr, v_tr, j);
    const auto& psi_base_tr = spectra.first;
    const auto& phi_base_tr = spectra.second;

    std::vector<std::complex<double>> conv_psi_tr(N), conv_phi_tr(N);

    for(size_t n = 0; n < N; n++){
        conv_psi_tr[n] = sig_tr[n] * std::conj(psi_base_tr[n]);
        conv_phi_tr[n] = sig_tr[n] * std::conj(phi_base_tr[n]);
    }

    std::vector<std::complex<double>> result_psi_full = FT::IFFT(conv_psi_tr), result_phi_full = FT::IFFT(conv_phi_tr);

    std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> result;
    result.first.reserve(coef_count);
    result.second.reserve(coef_count);

    for(size_t k = 0; k < coef_count; k++){
        size_t index = k * stride;
        result.first.push_back(result_phi_full[index]);
        result.second.push_back(result_psi_full[index]);
    }

    return result;
}