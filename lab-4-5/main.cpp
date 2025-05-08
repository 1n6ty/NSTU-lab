#include <iostream>
#include <chrono>
#include "linalg.h"
#include <limits>

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

/**
 * Method to generate initial matrix lab-5 `N*N`
 * @param N size of matrix
 * @return generated matrix
 */
linalg::Mat generate_mat2(size_t N){
    linalg::Mat out;
    for(size_t i = 0; i < N; i++){
        out.push_back(linalg::Vec());
        for(size_t j = 0; j < N; j++){
            out[i].push_back(1 / (1 + 0.8 * i + 2 * j));
        }
    }
    return out;
}

int main(int argc, char *argv[]){
    if(argc < 3) throw std::runtime_error("N and K must be provided"); 
    size_t N = static_cast<size_t>(atoi(argv[1])), K = static_cast<size_t>(atoi(argv[2]));

    linalg::Mat A = generate_mat2(N);

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

    stime = 0;
    std::array<linalg::Mat, 3> SVD = linalg::get_SVD(A);
    linalg::Mat buff;
    for(size_t t = 0; t < K; t++){
        start = std::chrono::steady_clock::now();
        SVD[0] = linalg::transpose(SVD[0]);
        buff = linalg::op_diag(SVD[1]);
        SVD[2] = linalg::transpose(SVD[2]);

        x = linalg::matmul(SVD[2], linalg::matmul(buff, linalg::matmul(SVD[0], f)));
        stop = std::chrono::steady_clock::now();

        stime += std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }
    printf("[SVD] AVG duration: %f ms, delta: %.20f\n", stime / K, linalg::norm_2(linalg::Mat2Vec(linalg::sub(x_sh, x), 0)));

    size_t n = A.size();
    double max_s = __DBL_MIN__, min_s = __DBL_MAX__;
    for(size_t i = 1; i < n; i++){
        if(SVD[1][i][i] > max_s) max_s = SVD[1][i][i];
        if(SVD[1][i][i] < min_s && std::abs(SVD[1][i][i]) > 1e-15) min_s = SVD[1][i][i];
    }

    printf("Cond(A): %.20f, sigma_max: %.20f, sigma_min: %.20f\n", max_s / min_s, max_s, min_s);

    return 0;
}