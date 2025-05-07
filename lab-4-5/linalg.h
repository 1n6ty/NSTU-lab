#pragma once
#include <ostream>
#include <utility>
#include <array>
#include <vector>
#include <array>

namespace linalg{
    typedef std::vector<double> Vec;
    typedef std::vector<Vec> Mat;

    Vec sub(const Vec &a, const Vec &b);
    Vec add(const Vec &a, const Vec &b);
    Vec mul(const Vec &a, double c);

    Mat sub(const Mat &a, const Mat &b);
    Mat add(const Mat &a, const Mat &b);
    Mat mul(const Mat &a, double c);

    Vec Mat2Vec(Mat mat, size_t ind);

    double dotProduct(const Vec& v1, const Vec& v2);
    double norm_2(const Vec &vec);

    Mat matmul(Mat &input_1, Mat &input_2);
    Mat transpose(const Mat &A);

    void swap_rows(Mat &mat, size_t fst_ind, size_t sec_ind);
    void swap_cols(Mat &mat, size_t fst_ind, size_t sec_ind);
    void row_comb(Mat &mat, size_t from, size_t to, double coef);

    Mat Gauss(Mat &A, Mat &f);
    Mat QR_dec(Mat &A, Mat &f);

    std::array<Mat, 2> _get_QR(Mat &A);
}

void printMatrix(std::ostream &stream, linalg::Mat &mat);