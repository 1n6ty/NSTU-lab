#include "linalg.h"
#include <vector>
#include <array>
#include <math.h>

/**
 * Methods that implements matrix multiplication
 * @param input_1 right matrix `n*m`
 * @param input_2 left matrix `m*k`
 * @return matrix `n*k`
*/
linalg::Mat linalg::matmul(linalg::Mat &input_1, linalg::Mat &input_2){
    size_t input_1_n = input_1.size(), input_2_m = input_2.size();
    if(input_1_n == 0 || input_2_m == 0){
        char buff[126];
        snprintf(buff, sizeof(buff), "Some arguments are empty: input_1: %lu, input_2: %lu", input_1_n, input_2_m);
        throw std::length_error(buff);
    }

    size_t input_1_m = input_1[0].size(), input_2_k = input_2[0].size();

    if(input_1_m != input_2_m){
        char buff[172];
        snprintf(buff, sizeof(buff), "Invalid shapes: (%lu, %lu) x (%lu, %lu), %lu != %lu", input_1_n, input_1_m, input_2_m, input_2_k, input_1_m, input_2_m);
        throw std::invalid_argument(buff);
    }

    linalg::Mat output = linalg::Mat(input_1_n, linalg::Vec(input_2_k, 0)); 
    for(size_t n = 0; n < input_1_n; n++){
        for(size_t k = 0; k < input_2_k; k++){
            for(size_t m = 0; m < input_1_m; m++) output[n][k] += input_1[n][m] * input_2[m][k];
        }
    }

    return output;
};

/**
 * Method to swap two rows in matrix
 * 
 * Mutates original matrix `mat`
 * @param mat matrix `n*m`
 * @param fst_ind first row index to be swaped
 * @param sec_ind second row index to be swaped
 * @return void
 */
void linalg::swap_rows(linalg::Mat &mat, size_t fst_ind, size_t sec_ind){
    size_t mat_n = mat.size();
    if(mat_n == 0) throw std::length_error("mat is empty");

    std::swap(mat[fst_ind], mat[sec_ind]);
}

/**
 * Method to swap two columns in matrix
 * 
 * Mutates original matrix `mat`
 * @param mat matrix `n*m`
 * @param fst_ind first column index to be swaped
 * @param sec_ind second column index to be swaped
 * @return void
 */
void linalg::swap_cols(linalg::Mat &mat, size_t fst_ind, size_t sec_ind){
    size_t mat_n = mat.size();
    if(mat_n == 0) throw std::length_error("mat is empty");

    for(size_t i = 0; i < mat_n; i++) std::swap(mat[i][fst_ind], mat[i][sec_ind]);
}

/**
 * Method to add weighted row to another row in matrix
 * 
 * Mutates original matrix `mat`
 *  
 * @param mat matrix `n*m`
 * @param from row to be added
 * @param to row to which we add
 * @param coef weight of row `from`
 * @return void
 */
void linalg::row_comb(linalg::Mat &mat, size_t from, size_t to, double coef){
    size_t mat_n = mat.size();
    if(mat_n == 0) throw std::length_error("mat is empty");

    size_t mat_m = mat[0].size();
    for(size_t j = 0; j < mat_m; j++){
        mat[to][j] += mat[from][j] * coef; 
    }
}

linalg::Vec linalg::add(const linalg::Vec &a, const linalg::Vec &b){
    size_t a_n = a.size(), b_n = b.size();
    if(a_n != b_n) throw std::invalid_argument("a and b must have same number of cols");

    linalg::Vec out = linalg::Vec(a);
    for(size_t i = 0; i < a_n; i++) out[i] = a[i] + b[i];

    return out;
}

linalg::Vec linalg::mul(const linalg::Vec &a, double b){
    size_t a_n = a.size();

    linalg::Vec out = linalg::Vec(a);
    for(size_t i = 0; i < a_n; i++) out[i] = a[i] * b;

    return out;
}

linalg::Vec linalg::sub(const linalg::Vec &a, const linalg::Vec &b){
    return linalg::add(a, mul(b, -1));
}

linalg::Mat linalg::add(const linalg::Mat &a, const linalg::Mat &b){
    size_t a_n = a.size(), b_n = b.size();
    if(a_n != b_n) throw std::invalid_argument("a and b must have same number of rows");

    linalg::Mat out = linalg::Mat(a);
    for(size_t i = 0; i < a_n; i++) out[i] = linalg::add(a[i], b[i]);
    return out;
}

linalg::Mat linalg::mul(const linalg::Mat &a, double b){
    size_t a_n = a.size();

    linalg::Mat out = linalg::Mat(a);
    for(size_t i = 0; i < a_n; i++) out[i] = linalg::mul(a[i], b);

    return out;
}

linalg::Mat linalg::sub(const linalg::Mat &a, const linalg::Mat &b){
    return linalg::add(a, mul(b, -1));
}

/**
 * Method to print a matrix into stream
 * 
 * @param stream stream to be used
 * @param mat matrix `n*m`
*/
void printMatrix(std::ostream &stream, linalg::Mat &mat){
    size_t mat_n = mat.size();
    if(mat_n == 0) return;

    size_t mat_m = mat[0].size();

    for(size_t n = 0; n < mat_n; n++){
        for(size_t m = 0; m < mat_m; m++){
            stream << std::scientific << mat[n][m] << ' ';
        }
        stream << '\n';
    }
    stream << std::endl;
}

/**
 * Method to solve a system of linear equations `Ax = f` (Gauss method)
 * 
 * @param A operator matrix `n * m`
 * @param f right 1-column matrix of the equation `n * 1`
 * @return x - 1-column matrix of unknown `m * 1`
 */
linalg::Mat linalg::Gauss(linalg::Mat &A, linalg::Mat &f){
    size_t A_n = A.size(), f_n = f.size();
    if(A_n == 0 || f_n == 0){
        char buff[81];
        snprintf(buff, sizeof(buff), "Some arguments are empty: A: %lu, f: %lu", A_n, f_n);
        throw std::length_error(buff);
    }

    size_t A_m = A[0].size();
    size_t pos_x[A_m];
    for(size_t i = 0; i < A_m; i++) pos_x[i] = i;

    linalg::Mat f_cpy = linalg::Mat(f);
    linalg::Mat A_cpy = linalg::Mat(A);

    // Transform A into upper triangular
    size_t max_e_pos[2];
    double coef;
    for(size_t iter = 0; iter < A_n - 1; iter++){
        // Search for max value over matrix
        max_e_pos[0] = max_e_pos[1] = iter;
        for(size_t i = iter; i < A_n; i++){
            for(size_t j = iter; j < A_m; j++){
                if(A_cpy[i][j] > A_cpy[max_e_pos[0]][max_e_pos[1]]) max_e_pos[0] = i, max_e_pos[1] = j;
            }
        }
        
        // Swap rows of 0 and max_e_pos[0]
        linalg::swap_rows(A_cpy, iter, max_e_pos[0]);
        std::swap(f_cpy[iter], f_cpy[max_e_pos[0]]);
        // Swap cols of 0 and max_e_pos[1]
        linalg::swap_cols(A_cpy, iter, max_e_pos[1]);
        std::swap(pos_x[iter], pos_x[max_e_pos[1]]);
        
        for(size_t i = iter + 1; i < A_n; i++){
            coef = -A_cpy[i][iter] / A_cpy[iter][iter];

            linalg::row_comb(A_cpy, iter, i, coef);
            linalg::row_comb(f_cpy, iter, i, coef);
        }
    }
    
    // Find x
    linalg::Mat x = linalg::Mat(A_m, linalg::Vec(1, 0));
    for(size_t i = 0, ir; i < A_n; i++){
        ir = A_n - 1 - i;

        x[pos_x[ir]][0] = f_cpy[ir][0];
        for(size_t j = ir + 1; j < A_m; j++) x[pos_x[ir]][0] -= x[pos_x[j]][0] * A_cpy[ir][j];
        x[pos_x[ir]][0] /= A_cpy[ir][ir];
    }

    return x;
}

/**
 * Method to convert specific column of matrix into vector
 * @param mat matrix to convert
 * @param ind index of a column
 * @return vector 
 */
linalg::Vec linalg::Mat2Vec(linalg::Mat mat, size_t ind){
    size_t mat_n = mat.size();
    if(mat_n == 0) throw std::length_error("mat is empty");

    size_t mat_m = mat[0].size();
    if(ind >= mat_m) throw std::invalid_argument("ind should not be greater than row dim");
    
    linalg::Vec out = linalg::Vec(mat_n, 0);
    for(size_t i = 0; i < mat_n; i++) out[i] = mat[i][ind];

    return out;
}

/**
 * Method to compute dot product `(v1, v2)`
 * @param v1 - first vector
 * @param v2 - second vector
 * @return result of dot product
 */
double linalg::dotProduct(const linalg::Vec& v1, const linalg::Vec& v2){
    double result = 0;
    for(size_t i = 0; i < v1.size(); i++) result += v1[i] * v2[i];
    return result;
}

/**
 * Method to substract projection of one vector on another's axis from another vector
 * @param v vector from which we substruct
 * @param projection what we substract
 * @return void
 */
void _subtractProjection(linalg::Vec& v, const linalg::Vec projection){
    double c = linalg::dotProduct(v, projection) / linalg::dotProduct(projection, projection);
    for(size_t i = 0; i < v.size(); ++i) v[i] -= c * projection[i];
}

/**
 * Method to transpose matrixes
 * @param A matrix to be transposed
 * @return transposed matrix
 */
linalg::Mat linalg::transpose(const linalg::Mat &A){
    size_t A_n = A.size();
    if(A_n == 0) throw std::length_error("mat is empty");

    size_t A_m = A[0].size();
    
    linalg::Mat transposed = linalg::Mat(A_m, linalg::Vec(A_n));
    
    for(size_t i = 0; i < A_n; i++) {
        for (size_t j = 0; j < A_m; j++) transposed[j][i] = A[i][j];
    }
    
    return transposed;
}

/**
 * Method to get QR decomposition of A
 * @param A operator matrix `n * m`
 * @return pair of QR
 */
std::array<linalg::Mat, 2> linalg::_get_QR(linalg::Mat &A){
    linalg::Mat Q_t = linalg::transpose(A);
    
    size_t Q_t_n = Q_t.size();
    if(Q_t_n == 0) throw std::length_error("mat is empty");

    size_t Q_t_m = Q_t[0].size();

    double norm;
    for(size_t i = 0; i < Q_t_n; i++){
        for(size_t j = 0; j < i; j++) _subtractProjection(Q_t[i], Q_t[j]);
        
        norm = sqrt(linalg::dotProduct(Q_t[i], Q_t[i]));
        if(norm <= 1e-15) break; 
        for(size_t k = 0; k < Q_t_m; ++k) Q_t[i][k] /= norm;
    }

    return {linalg::transpose(Q_t), linalg::matmul(Q_t, A)};
}

/**
 * Method that solves a system of linear equations `Ax = f` by `A = QR` and Gram-Shmidt Orthogonalization
 * @param A operator matrix `n * m`
 * @param f right 1-column matrix of the equation `n * 1`
 * @return x - 1-column matrix of unknown `m * 1`
 */
linalg::Mat linalg::QR_dec(linalg::Mat &A, linalg::Mat &f){
    std::array<linalg::Mat, 2> QR = linalg::_get_QR(A);

    linalg::Mat Q_t = linalg::transpose(QR[0]);
    linalg::Mat y = linalg::matmul(Q_t, f);
    // Find x
    size_t Q_t_n = Q_t.size();
    linalg::Mat x = linalg::Mat(Q_t_n, linalg::Vec(1, 0));
    for(size_t i = 0, ir; i < Q_t_n; i++){
        ir = Q_t_n - 1 - i;

        x[ir][0] = y[ir][0];
        for(size_t j = ir + 1; j < Q_t_n; j++) x[ir][0] -= x[j][0] * (QR[1])[ir][j];
        x[ir][0] /= (QR[1])[ir][ir];
    }

    return x;
}

/**
 * Method to compute 2-norm `sqrt(sum(vec_i^2))` of a vector
 * @param vec vector, which norm will be computed
 * @return 2-norm
 */
double linalg::norm_2(const linalg::Vec &vec){
    return std::sqrt(dotProduct(vec, vec));
}