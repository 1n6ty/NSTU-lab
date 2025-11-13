/**
 * @file    mix.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for Mix of two Pearson-VII distributions
 *          (shear-scale transformed). For more see @ref mixd.h "Mix of two Pearson-VII header file".
 */

#include "mix.h"
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <random>

/**
 * @version 2.0
 * 
 * @brief   Constructor of the Mix distribution.
 * 
 * @details Also checks if @ref p is in `[0, 1]`.
 *          If conditions doesn't met - throws MixException
 * 
 * @param   d1 First PearsonVII object of first distribution
 * @param   d2 Second PearsonVII object of first distribution
 * @param   p mix parameter
 */
Mix::Mix(const PearsonVII& d1, const PearsonVII& d2, const double p)
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
Mix::Mix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw MixException("Cannot open file for reading: " + filename);

    double shift1, scale1, shape1,
           shift2, scale2, shape2,
           p_val;

    if (!(file >> shift1 >> scale1 >> shape1)) throw MixException("Invalid file format: missing first distribution parameters");
    if (!(file >> shift2 >> scale2 >> shape2)) throw MixException("Invalid file format: missing second distribution parameters");
    if (!(file >> p_val)) throw MixException("Invalid file format: missing mix parameter p");

    file.close();

    if (p_val < 0.0 || p_val > 1.0) throw MixException("Mix parameter p must be in [0, 1]");

    PearsonVII dist1(shift1, scale1, shape1);
    PearsonVII dist2(shift2, scale2, shape2);

    this->d1 = dist1;
    this->d2 = dist2;
    this->p = p_val;
}

/**
 * @version 1.0
 * 
 * @brief   Getters.
 */
PearsonVII& Mix::getFstComponent() { return this->d1; };
PearsonVII& Mix::getSecComponent() { return this->d2; };
double Mix::getP() const { return this->p; };

/**
 * @version 2.0
 * 
 * @brief   Computes density of mix distribution.
 * 
 * @param   x point at which the density will be computed
 * 
 * @return  probability density at @p x point
 */
double Mix::computeDensity(double x) const{
    return (1 - this->p) * this->d1.computeDensity(x) + this->p * this->d2.computeDensity(x);
}

/**
 * @version 2.0
 * 
 * @brief   Computes Mathematical Expectation of mix distribution.
 * 
 * @return  Mathematical Expectation
 */
double Mix::computeExpectation() const {
    return (1 - this->p) * this->d1.computeExpectation() + this->p * this->d2.computeExpectation();
}

/**
 * @version 2.0
 * 
 * @brief   Computes Variance of mix distribution.
 * 
 * @return  Variance
 */
double Mix::computeVariance() const {
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
double Mix::computeSkewness() const {
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
double Mix::computeKurtosis() const {
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
double Mix::generateRandom() const {
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
void Mix::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw MixException("Cannot open file for writing: " + filename);
    }

    file << d1.getShift() << " " << d1.getScale() << " " << d1.getShape() << "\n";
    file << d2.getShift() << " " << d2.getScale() << " " << d2.getShape() << "\n";
    file << p << "\n";

    file.close();
}