/**
 * @file    mix.tpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for Mix of two distributions
 *          (shear-scale transformed). For more see @ref mixd.h "Mix of two distributions header file".
 */

#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <random>

#include "mix.h"

/**
 * @version 1.0
 * 
 * @brief   Default Constructor of the Mix distribution.
 */
template<class D1, class D2>
Mix<D1, D2>::Mix()
    : p(0.5), d1(D1()), d2(D2()) {};


/**
 * @version 2.0
 * 
 * @brief   Constructor of the Mix distribution.
 * 
 * @details Also checks if @ref p is in `[0, 1]`.
 *          If conditions doesn't met - throws MixException
 * 
 * @param   d1 First Distribution object
 * @param   d2 Second Distribution object
 * @param   p mix parameter
 */
template<class D1, class D2>
Mix<D1, D2>::Mix(const D1& d1, const D2& d2, const double p)
    : p((0 <= p && 1 >= p) ? p: throw MixException("p should be in [0, 1]")), d1(d1), d2(d2) {};

/**
 * @version 2.0
 * 
 * @brief   Constructor of the Mix distribution (load from file).
 * 
 * @details Throws MixException if invalid parameters or file errors.
 * 
 * @param   filename name of file to load from
 */
template<class D1, class D2>
Mix<D1, D2>::Mix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw MixException("Cannot open file: " + filename);

    std::string f1, f2;
    double p_val;

    if (!(file >> f1)) throw MixException("Invalid file: missing first component file");
    if (!(file >> f2)) throw MixException("Invalid file: missing second component file");
    if (!(file >> p_val)) throw MixException("Missing mix coefficient p");

    if (p_val < 0.0 || p_val > 1.0) throw MixException("p must be in [0,1]");

    file.close();

    this->d1 = D1(f1);
    this->d2 = D2(f2);
    this->p = p_val;
}

/**
 * @version 1.0
 * 
 * @brief   Getters.
 */
template<class D1, class D2>
D1& Mix<D1, D2>::getFstComponent() { return this->d1; };
template<class D1, class D2>
D2& Mix<D1, D2>::getSecComponent() { return this->d2; };
template<class D1, class D2>
double Mix<D1, D2>::getP() const { return this->p; };

/**
 * @version 1.0
 * 
 * @brief   Setters.
 */
template<class D1, class D2>
void Mix<D1, D2>::setP(double const p) { this->p = (0 <= p && 1 >= p) ? p: throw MixException("p should be in [0, 1]"); }

/**
 * @version 2.0
 * 
 * @brief   Computes density of mix distribution.
 * 
 * @param   x point at which the density will be computed
 * 
 * @return  probability density at @p x point
 */
template<class D1, class D2>
double Mix<D1, D2>::computeDensity(double x) const{
    return (1 - this->p) * this->d1.computeDensity(x) + this->p * this->d2.computeDensity(x);
}

/**
 * @version 2.0
 * 
 * @brief   Computes Mathematical Expectation of mix distribution.
 * 
 * @return  Mathematical Expectation
 */
template<class D1, class D2>
double Mix<D1, D2>::computeExpectation() const {
    return (1 - this->p) * this->d1.computeExpectation() + this->p * this->d2.computeExpectation();
}

/**
 * @version 2.0
 * 
 * @brief   Computes Variance of mix distribution.
 * 
 * @return  Variance
 */
template<class D1, class D2>
double Mix<D1, D2>::computeVariance() const {
    return (1 - this->p) * (std::pow(this->d1.computeExpectation(), 2) + this->d1.computeVariance())
            + this->p * (std::pow(this->d2.computeExpectation(), 2) + this->d2.computeVariance()) - std::pow(this->computeExpectation(), 2);
}

/**
 * @version 2.0
 * 
 * @brief   Computes Skewness of mix distribution.
 * 
 * @return  Skewness
 */
template<class D1, class D2>
double Mix<D1, D2>::computeSkewness() const {
    double mix_exp = Mix::computeExpectation(),
           d1_exp = this->d1.computeExpectation(),
           d1_var = this->d1.computeVariance(),
           d2_exp = this->d2.computeExpectation(),
           d2_var = this->d2.computeVariance();

    return ((1 - this->p) * (std::pow(this->d1.computeExpectation() - mix_exp, 3) + 3 * (d1_exp - mix_exp) * d1_var + std::pow(d1_var, 1.5) * this->d1.computeSkewness()) +
           p * (std::pow(this->d2.computeExpectation() - mix_exp, 3) + 3 * (d2_exp - mix_exp) * d2_var + std::pow(d2_var, 1.5) * this->d2.computeSkewness())) / std::pow(this->computeVariance(), 1.5);
}

/**
 * @version 2.0
 * 
 * @brief   Computes Kurtosis of mix distribution.
 * 
 * @return  Kurtosis
 */
template<class D1, class D2>
double Mix<D1, D2>::computeKurtosis() const {
    double mix_exp = Mix::computeExpectation(),
           d1_exp = this->d1.computeExpectation(),
           d1_var = this->d1.computeVariance(),
           d2_exp = this->d2.computeExpectation(),
           d2_var = this->d2.computeVariance();
    
    return ((1 - this->p) * (std::pow(d1_exp - mix_exp, 4) + 6 * std::pow(d1_exp - mix_exp, 2) * d1_var + 4 * (d1_exp - mix_exp) * std::pow(d1_var, 1.5) * this->d1.computeSkewness() + std::pow(d1_var, 2) * (this->d1.computeKurtosis() + 3)) + 
           this->p * (std::pow(d2_exp - mix_exp, 4) + 6 * std::pow(d2_exp - mix_exp, 2) * d2_var + 4 * (d2_exp - mix_exp) * std::pow(d2_var, 1.5) * this->d2.computeSkewness() + std::pow(d2_var, 2) * (this->d2.computeKurtosis() + 3))) / 
           std::pow(this->computeVariance(), 2) - 3;
}

/**
 * @version 2.0
 * 
 * @brief   Generates x out of mix distribution.
 * 
 * @return  generated x
 */
template<class D1, class D2>
double Mix<D1, D2>::generateRandom() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    return (dis(gen) <= (1 - this->p)) ? this->d1.generateRandom(): this->d2.generateRandom();
}

/**
 * @version 2.0
 * 
 * @brief   Saves parameters of Mix distribution to file.
 * 
 * @param   filename Name of file where parameters will be written
 */
template<class D1, class D2>
void Mix<D1, D2>::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw MixException("Cannot open file for writing: " + filename);
    }

    file << filename << "_d1" << "\n";
    this->d1.saveToFile(filename + "_d1");

    file << filename << "_d2" << "\n";
    this->d2.saveToFile(filename + "_d2");

    file << p << "\n";

    file.close();
}