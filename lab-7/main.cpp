#include <vector>
#include <complex>
#include <iostream>
#include <fstream>
#include <chrono>
#include <utility>

#include "FT.h"
#include "Wavelet.h"

std::vector<std::complex<double>> generate(const size_t N){
    std::vector<std::complex<double>> result;

    for(size_t i = 0; i < N; i++){
        if(0 <= i && i < N / 4){
            result.push_back(0);
        } else if(N / 4 <= i && i <= N / 2){
            result.push_back(2.70 + 0.25 * std::cos(2 * M_PI * 180. * static_cast<double>(i) / N));
        } else if(N / 2 < i && i <= 3 * N / 4){
            result.push_back(0);
        } else {
            result.push_back(2.70 + 0.25 * std::cos(2 * M_PI * 180. * static_cast<double>(i) / N));
        }
        
    }

    return result;
}

std::vector<std::complex<double>> generate_m(const size_t N){
    std::vector<std::complex<double>> result;
    for(size_t i = 0; i < N; i++) result.push_back(2.70 * std::cos(2. * M_PI * 2. * static_cast<double>(i) / N + M_PI / 2.) + 0.25 * std::cos(2 * M_PI * 180. * static_cast<double>(i) / N));

    return result;
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> haar(const size_t N){
    std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> result(
        std::vector<std::complex<double>>(N, 0), std::vector<std::complex<double>>(N, 0)
    );

    result.first[0] = 1. / std::sqrt(2);
    result.first[1] = 1. / std::sqrt(2);
    
    result.second[0] = 1. / std::sqrt(2);
    result.second[1] = -1. / std::sqrt(2);

    return result;
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> shannon(const size_t N){
    std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> result(
        std::vector<std::complex<double>>(N, 0), std::vector<std::complex<double>>(N, 0)
    );

    double center = (static_cast<double>(N) - 1.0) / 2.0;

    for (size_t i = 0; i < N; ++i) {
        double n = static_cast<double>(i) - center;

        double h_val = 0.0;
        if (std::abs(n) < 1e-9) {
            h_val = 1.0 / std::sqrt(2); 
        } else {
            h_val = (std::sqrt(2) * std::sin(M_PI * n / 2.0)) / (M_PI * n);
        }
        result.first[i] = h_val;

        double sign = (i % 2 == 0) ? 1.0 : -1.0;
        
        size_t mirror_index = N - 1 - i;
        
        double n_mirror = static_cast<double>(mirror_index) - center;
        double h_mirror = 0.0;
         if (std::abs(n_mirror) < 1e-9) {
            h_mirror = 1.0 / std::sqrt(2);
        } else {
            h_mirror = (std::sqrt(2) * std::sin(M_PI * n_mirror / 2.0)) / (M_PI * n_mirror);
        }

        result.second[i] = sign * h_mirror;
    }

    return result;
}

std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> d6(const size_t N){
    std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> result(
        std::vector<std::complex<double>>(N, 0), std::vector<std::complex<double>>(N, 0)
    );

    const double h0 = 0.33267055295008269;
    const double h1 = 0.80689150931109257;
    const double h2 = 0.45987750211849157;
    const double h3 = -0.13501102001025458;
    const double h4 = -0.08544127388202666;
    const double h5 = 0.03522629188570953;

    result.first[0] = h0;
    result.first[1] = h1;
    result.first[2] = h2;
    result.first[3] = h3;
    result.first[4] = h4;
    result.first[5] = h5;

    result.second[0] =  h5;
    result.second[1] = -h4;
    result.second[2] =  h3;
    result.second[3] = -h2;
    result.second[4] =  h1;
    result.second[5] = -h0;

    return result;
}

std::vector<std::complex<double>> get_P(const std::vector<std::complex<double>>& signal, const std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>>& coefs, const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, size_t j){
    size_t N = signal.size();

    std::vector<std::complex<double>> sum(N, 0.);
    for(size_t i = 0; i < (N / (1 << j)); i++){
        std::vector<std::complex<double>> phi = Wavelet::phi(u_tr, v_tr, j, i), psi = Wavelet::psi(u_tr, v_tr, j, i);
        for(size_t n = 0; n < phi.size(); n++){
            sum[n] += phi[n] * coefs.first[i] + psi[n] * coefs.second[i];
        }
    }

    return sum;
}

void process_wavelet(std::string name, std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> (*filter_func)(size_t), const std::vector<std::complex<double>>& signal, size_t N){
    auto filters = filter_func(N);
    auto u_tr = FT::FFT(filters.first);
    auto v_tr = FT::FFT(filters.second);

    auto coefs1 = Wavelet::coefs(signal, u_tr, v_tr, 1);
    auto P1 = get_P(signal, coefs1, u_tr, v_tr, 1);
    auto coefs2 = Wavelet::coefs(signal, u_tr, v_tr, 2);
    auto P2 = get_P(signal, coefs2, u_tr, v_tr, 2);
    auto coefs3 = Wavelet::coefs(signal, u_tr, v_tr, 3);
    auto P3 = get_P(signal, coefs3, u_tr, v_tr, 3);
    auto coefs4 = Wavelet::coefs(signal, u_tr, v_tr, 4);
    auto P4 = get_P(signal, coefs4, u_tr, v_tr, 4);

    std::ofstream f3_1(name + "_task3_phi4.txt");
    f3_1 << "phi4\n";
    for(size_t i = 0; i < coefs4.first.size(); ++i){
        f3_1 << coefs4.first[i].real() << "\n";
    }
    f3_1.close();

    std::ofstream f3_2(name + "_task3_psi4.txt");
    f3_2 << "psi4\n";
    for(size_t i = 0; i < coefs4.second.size(); ++i){
        f3_2 << coefs4.second[i].real() << "\n";
    }
    f3_2.close();

    std::ofstream f3_3(name + "_task3_psi3.txt");
    f3_3 << "psi3\n";
    for(size_t i = 0; i < coefs3.second.size(); ++i){
        f3_3 << coefs3.second[i].real() << "\n";
    }
    f3_3.close();

    std::ofstream f3_4(name + "_task3_psi2.txt");
    f3_4 << "psi2\n";
    for(size_t i = 0; i < coefs2.second.size(); ++i){
        f3_4 << coefs2.second[i].real() << "\n";
    }
    f3_4.close();

    std::ofstream f3_5(name + "_task3_psi1.txt");
    f3_5 << "psi1\n";
    for(size_t i = 0; i < coefs1.second.size(); ++i){
        f3_5 << coefs1.second[i].real() << "\n";
    }
    f3_5.close();

    std::ofstream f4(name + "_task4.txt");
    f4 << "P1 P2 P3\n";
    for(size_t i = 0; i < N; ++i){
        f4 << P1[i].real() << " " 
           << P2[i].real() << " "
           << P3[i].real() << "\n";
    }
    f4.close();

    coefs2.second = std::vector<std::complex<double>>(coefs2.second.size(), 0);
    P2 = get_P(signal, coefs2, u_tr, v_tr, 2);

    std::ofstream f5(name + "_task5.txt");
    f5 << "P1 filtered\n";
    for (size_t i = 0; i < P2.size(); ++i) {
        f5 << P2[i].real() << "\n";
    }
    f5.close();
}

int main() {
    size_t N = std::pow(2, 11);
    auto signal = generate(N);

    std::ofstream f_sig("original_signal.txt");
    for(auto& s : signal) f_sig << s.real() << "\n";
    f_sig.close();

    process_wavelet("haar", haar, signal, N);
    process_wavelet("shannon", shannon, signal, N);
    process_wavelet("d6", d6, signal, N);

    signal = generate_m(N);
    std::ofstream f_sig_m("original_signal_m.txt");
    for(auto& s : signal) f_sig_m << s.real() << "\n";
    f_sig_m.close();

    process_wavelet("haar_m", haar, signal, N);
    process_wavelet("shannon_m", shannon, signal, N);
    process_wavelet("d6_m", d6, signal, N);

    std::cout << "Analysis complete. Data files generated." << std::endl;

    return 0;
}