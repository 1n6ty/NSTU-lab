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

std::vector<std::complex<double>> get_P(const std::vector<std::complex<double>>& signal, const std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>>& coefs, const std::vector<std::complex<double>>& u_tr, const std::vector<std::complex<double>>& v_tr, size_t j, const std::vector<std::complex<double>>& prev_P){
    size_t N = signal.size();

    std::vector<std::complex<double>> sum = prev_P;
    for(size_t i = 0; i < (N / (1 << j)); i++){
        std::vector<std::complex<double>> psi = Wavelet::psi(u_tr, v_tr, j, i);
        for(size_t n = 0; n < psi.size(); n++){
            sum[n] += psi[n] * coefs.second[i];
        }
    }

    return sum;
}

void process_wavelet(std::string name, std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>> (*filter_func)(size_t), const std::vector<std::complex<double>>& signal, size_t N){
    auto filters = filter_func(N);
    auto u_tr = FT::FFT(filters.first);
    auto v_tr = FT::FFT(filters.second);

    auto coefs1 = Wavelet::coefs(signal, u_tr, v_tr, 1);
    auto coefs2 = Wavelet::coefs(signal, u_tr, v_tr, 2);
    auto coefs3 = Wavelet::coefs(signal, u_tr, v_tr, 3);
    auto coefs4 = Wavelet::coefs(signal, u_tr, v_tr, 4);

    auto P3 = get_P(signal, coefs4, u_tr, v_tr, 4);
    auto P2 = get_P(signal, coefs3, u_tr, v_tr, 3, P3);
    auto P1 = get_P(signal, coefs2, u_tr, v_tr, 2, P2);
    auto P0 = get_P(signal, coefs1, u_tr, v_tr, 1, P1);

    std::ofstream f3_1(name + "_task3_phi4.txt");
    f3_1 << "phi4\n";
    for(size_t i = 0; i < coefs4.first.size(); i++){
        f3_1 << coefs4.first[i].real() << "\n";
    }
    f3_1.close();

    std::ofstream f3_2(name + "_task3_psi4.txt");
    f3_2 << "psi4\n";
    for(size_t i = 0; i < coefs4.second.size(); i++){
        f3_2 << coefs4.second[i].real() << "\n";
    }
    f3_2.close();

    std::ofstream f3_3(name + "_task3_psi3.txt");
    f3_3 << "psi3\n";
    for(size_t i = 0; i < coefs3.second.size(); i++){
        f3_3 << coefs3.second[i].real() << "\n";
    }
    f3_3.close();

    std::ofstream f3_4(name + "_task3_psi2.txt");
    f3_4 << "psi2\n";
    for(size_t i = 0; i < coefs2.second.size(); i++){
        f3_4 << coefs2.second[i].real() << "\n";
    }
    f3_4.close();

    std::ofstream f3_5(name + "_task3_psi1.txt");
    f3_5 << "psi1\n";
    for(size_t i = 0; i < coefs1.second.size(); i++){
        f3_5 << coefs1.second[i].real() << "\n";
    }
    f3_5.close();

    std::ofstream f4(name + "_task4.txt");
    f4 << "P-1 P-2 P-3\n";
    for(size_t i = 0; i < N; i++){
        f4 << P1[i].real() << " " 
           << P2[i].real() << " "
           << P3[i].real() << "\n";
    }
    f4.close();

    auto coefs2_copy = coefs2;
    coefs2_copy.second = std::vector<std::complex<double>>(coefs2.second.size(), 0);
    P1 = get_P(signal, coefs2_copy, u_tr, v_tr, 2, P2);

    std::ofstream f5(name + "_task5.txt");
    f5 << "P-1 filtered\n";
    
    for (size_t i = 0; i < P1.size(); i++) {
        f5 << P1[i].real() << "\n";
    }
    f5.close();

    if(name == "d6_m"){
        auto coefs5 = Wavelet::coefs(signal, u_tr, v_tr, 5);
        auto coefs6 = Wavelet::coefs(signal, u_tr, v_tr, 6);
        auto coefs7 = Wavelet::coefs(signal, u_tr, v_tr, 7);
        auto coefs8 = Wavelet::coefs(signal, u_tr, v_tr, 8);

        coefs1.second = std::vector<std::complex<double>>(coefs1.second.size(), 0);
        coefs2.second = std::vector<std::complex<double>>(coefs2.second.size(), 0);
        coefs3.second = std::vector<std::complex<double>>(coefs3.second.size(), 0);
        coefs4.second = std::vector<std::complex<double>>(coefs4.second.size(), 0);
        coefs5.second = std::vector<std::complex<double>>(coefs5.second.size(), 0);
        coefs6.second = std::vector<std::complex<double>>(coefs6.second.size(), 0);
        coefs7.second = std::vector<std::complex<double>>(coefs7.second.size(), 0);
        coefs8.second = std::vector<std::complex<double>>(coefs8.second.size(), 0);

        P3 = get_P(signal, coefs4, u_tr, v_tr, 4);
        P2 = get_P(signal, coefs3, u_tr, v_tr, 3);
        P1 = get_P(signal, coefs2, u_tr, v_tr, 2);
        P0 = get_P(signal, coefs1, u_tr, v_tr, 1);
        auto P4 = get_P(signal, coefs5, u_tr, v_tr, 5);
        auto P5 = get_P(signal, coefs6, u_tr, v_tr, 6);
        auto P6 = get_P(signal, coefs7, u_tr, v_tr, 7);
        auto P7 = get_P(signal, coefs8, u_tr, v_tr, 8);

        std::ofstream f7(name + "_task7.txt");
        f7 << "P-0 P-1 P-2 P-3 P-4 P-5 P-6 P-7\n";
        for (size_t i = 0; i < P3.size(); i++) {
            f7 << P0[i].real() << " " << P1[i].real() << " " << P2[i].real() << " " << P3[i].real() << " " << P4[i].real() << " " << P5[i].real() << " " << P6[i].real() << " " << P7[i].real() << "\n";
        }
        f7.close();
    }
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