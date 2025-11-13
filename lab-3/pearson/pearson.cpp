/**
 * @file    pirson.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Implementation of PearsonVII class.
 */

#include "pearson.h"
#include <fstream>
#include <cmath>
#include <random>

/**
 * @version 1.0
 * 
 * @brief   Constructor of pearson-VII distribution.
 * 
 * @details Throws PearsonException if invalid parameters.
 * 
 * @param   shift shift parameter
 * @param   scale scale parameter
 * @param   shape shape parameter
 */
PearsonVII::PearsonVII(double shift, double scale, double shape){
    if (shape <= 0.5 || scale <= 0) throw PearsonException("Invalid parameters for pearson distribution (shape > 0.5 and scale > 0).");

    this->shift = shift;
    this->scale = scale;
    this->shape = shape;

    updateNormalizationConstant();
}

/**
 * @version 1.0
 * 
 * @brief   Constructor of pearson-VII distribution (load from file).
 * 
 * @details Throws PearsonException if invalid parameters or file errors.
 * 
 * @param   filename name of file to load from
 */
PearsonVII::PearsonVII(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw PearsonException("Cannot open file for reading: " + filename);
    }
    
    double shift, scale, shape;
    if (!(file >> shift >> scale >> shape)) {
        throw PearsonException("Invalid file format");
    }
    
    if (shape <= 0.5 || scale <= 0){
        file.close();
        throw PearsonException("Invalid parameters for pearson distribution (shape > 0.5 and scale > 0).");
    }

    this->shift = shift;
    this->scale = scale;
    this->shape = shape;

    updateNormalizationConstant();

    file.close();
}

/**
 * @version 1.0
 * 
 * @brief   Method to update normalization constant.
 */
inline void PearsonVII::updateNormalizationConstant(){
    this->normalization_constant = 1.0 / (scale * std::beta(this->shape - 0.5, 0.5) * std::pow(1.0, this->shape - 0.5));
}

/**
 * @version 1.0
 * 
 * @brief   Getters.
 */
double PearsonVII::getShift() const { return this->shift; }
double PearsonVII::getScale() const { return this->scale; }
double PearsonVII::getShape() const { return this->shape; }

/**
 * @version 1.0
 * 
 * @brief   Setters.
 */
void PearsonVII::setShift(double const shift) { this->shift = shift; }
void PearsonVII::setScale(double const scale) { this->scale = (scale > 0) ? scale: throw PearsonException("Invalid parameters for pearson distribution: scale must be > 0."); }
void PearsonVII::setShape(double const shape) { this->shape = (shape > 0.5) ? shape: throw PearsonException("Invalid parameters for pearson distribution: shape must be > 0.5."); }

/**
 * @version 1.0
 * 
 * @brief   Computes density of pearson-VII distribution.
 * 
 * @param   x point at which the density will be computed
 * @return  probability density at @p x point
 */
double PearsonVII::computeDensity(double x) const {
    double z = (x - this->shift) / this->scale;
    return this->normalization_constant * std::pow(1.0 + z * z, -this->shape);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of pearson-VII distribution.
 * 
 * @details If `shape` is less or equal `1` then throws PearsonException
 * 
 * @return  Mathematical Expectation
 */
double PearsonVII::computeExpectation() const {
    if (this->shape <= 1.0){
        throw PearsonException("Expectation does not exist for shape <= 1");
    }
    return this->shift;
}

/**
 * @version 1.0
 * 
 * @brief   Computes Variance of pearson-VII distribution.
 * 
 * @details If `shape` is less or equal `1.5` then throws PearsonException
 * 
 * @return  Dispersion
 */
double PearsonVII::computeVariance() const {
    if (this->shape <= 1.5){
        throw PearsonException("Variance does not exist for shape <= 1.5");
    }
    return this->scale * this->scale / (2.0 * this->shape - 3.0);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Skewness of pearson-VII distribution.
 * 
 * @details If `shape` is less or equal `1.5` then throws PearsonException
 * 
 * @return  Skewness
 */
double PearsonVII::computeSkewness() const {
    if (this->shape <= 1.5){
        throw PearsonException("Skewness does not exist for shape <= 1.5");
    }
    return 0.0;
}

/**
 * @version 1.0
 * 
 * @brief   Computes Kurtosis of pearson-VII distribution.
 * 
 * @details If `shape` is less or equal `2.0` then throws PearsonException
 * 
 * @return  Kurtosis
 */
double PearsonVII::computeKurtosis() const {
    if (this->shape <= 2.0){
        throw PearsonException("Kurtosis does not exist for shape <= 2");
    }
    return 6.0 / (2.0 * this->shape - 5.0);
}

/**
 * @version 1.0
 * 
 * @brief   Generates value out of pearson-VII distribution.
 * 
 * @return  generated value
 */
double PearsonVII::generateRandom() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    double u = dis(gen);
    double t = std::pow(u, -1.0 / (this->shape - 0.5)) - 1.0;
    
    return this->shift + this->scale * std::sqrt(t) * std::cos(2.0 * M_PI * dis(gen));
}

/**
 * @version 1.0
 * 
 * @brief   Saves Pearson-VII distribution parameters to file.
 */
void PearsonVII::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()){
        throw PearsonException("Cannot open file for writing: " + filename);
    }
    
    file << this->shift << " " << this->scale << " " << this->shape << std::endl;
    file.close();
}
