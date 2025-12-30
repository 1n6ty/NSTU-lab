#include <vector>
#include <complex>
#include <iostream>
#include <fstream>
#include <chrono>

#include "FT.h"

std::vector<std::complex<double>> generate(const size_t N){
    std::vector<std::complex<double>> result;
    for(size_t i = 0; i < N; i++) result.push_back(2.70 * std::cos(2. * M_PI * 2. * static_cast<double>(i) / N + M_PI / 2.) + 0.25 * std::cos(2 * M_PI * 180. * static_cast<double>(i) / N));

    return result;
}

int main(){
    size_t N = std::pow(2, 11);
    std::vector<std::complex<double>> signal = generate(N), result, orig;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    result = FT::DFT(signal);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "DFT (elapsed time): " << elapsed_seconds.count() << std::endl;

    start = std::chrono::system_clock::now();
    result = FT::FFT(signal);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "FFT (elapsed time): " << elapsed_seconds.count() << std::endl;
    
    double abs_b, freq_b;
    for(size_t i = 0; i < N; i++){
        abs_b = std::abs(result[i]);
        freq_b = std::atan2(result[i].imag(), result[i].real());
        if(abs_b > 1e-7){
            std::cout << i << ": \n\tDFT: Amplitude = " << abs_b << "; Phase = " << freq_b << "; Real part = " << result[i].real() << "; Imag part = " << result[i].imag()
                        << "\n\tInitial: Real part: " << signal[i].real() << std::endl;
        }
    }

    std::vector<std::complex<double>> cleared = result;
    cleared[180] = std::complex<double>(0, 0);
    cleared[1868] = std::complex<double>(0, 0);

    start = std::chrono::system_clock::now();
    orig = FT::IDFT(cleared);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "IDFT (elapsed time): " << elapsed_seconds.count() << std::endl;

    start = std::chrono::system_clock::now();
    orig = FT::IFFT(cleared);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "IFFT (elapsed time): " << elapsed_seconds.count() << std::endl;

    std::ofstream file("data.txt");

    for (size_t i = 0; i < N; ++i) {
        file << signal[i].real() << " "
             << orig[i].real() << "\n";
    }

    file.close();

    return 0;
}