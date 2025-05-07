#include <iostream>
#include <chrono>
#include "linalg.h"

/**
 * Method to generate initial matrix lab-4 `N*N`
 * @param N size of matrix
 * @return generated matrix
 */
linalg::Mat generate_mat(size_t N){
    linalg::Mat out;
    for(size_t i = 0; i < N; i++){
        out.push_back(linalg::Vec());
        for(size_t j = 0; j < N; j++){
            out[i].push_back((i == j) ? 100: 2 + 0.01 * static_cast<double>(i) + 0.2 * static_cast<double>(j));
        }
    }
    return out;
}

int main(int argc, char *argv[]){
    if(argc < 3) throw std::runtime_error("N and K must be provided"); 
    size_t N = static_cast<size_t>(atoi(argv[1])), K = static_cast<size_t>(atoi(argv[2]));

    linalg::Mat A = generate_mat(N);

    linalg::Mat x_sh = linalg::Mat(N, {1});
    linalg::Mat f = linalg::matmul(A, x_sh);
    
    std::chrono::steady_clock::time_point start, stop;
    double stime = 0;
    linalg::Mat x;
    for(size_t t = 0; t < K; t++){
        start = std::chrono::steady_clock::now();
        x = linalg::Gauss(A, f);
        stop = std::chrono::steady_clock::now();

        stime += std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }
    printf("[Gauss] AVG duration: %f ms, delta: %.20f\n", stime / K, linalg::norm_2(linalg::Mat2Vec(linalg::sub(x_sh, x), 0)));

    stime = 0;
    std::array<linalg::Mat, 2> QR = linalg::_get_QR(A);
    linalg::Mat Q_t = linalg::transpose(QR[0]);
    linalg::Mat y = linalg::matmul(Q_t, f);
    size_t Q_t_n = Q_t.size();
    for(size_t t = 0; t < K; t++){
        start = std::chrono::steady_clock::now();
        x = linalg::Mat(Q_t_n, linalg::Vec(1, 0));
        for(size_t i = 0, ir; i < Q_t_n; i++){
            ir = Q_t_n - 1 - i;

            x[ir][0] = y[ir][0];
            for(size_t j = ir + 1; j < Q_t_n; j++) x[ir][0] -= x[j][0] * (QR[1])[ir][j];
            x[ir][0] /= (QR[1])[ir][ir];
        }
        stop = std::chrono::steady_clock::now();

        stime += std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }
    printf("[QR GSH without decomposition] AVG duration: %f ms, delta: %.20f\n", stime / K, linalg::norm_2(linalg::Mat2Vec(linalg::sub(x_sh, x), 0)));

    stime = 0;
    for(size_t t = 0; t < K; t++){
        start = std::chrono::steady_clock::now();
        x = linalg::QR_dec(A, f);
        stop = std::chrono::steady_clock::now();

        stime += std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }
    printf("[QR GSH] AVG duration: %f ms, delta: %.20f\n", stime / K, linalg::norm_2(linalg::Mat2Vec(linalg::sub(x_sh, x), 0)));

    

    return 0;
}