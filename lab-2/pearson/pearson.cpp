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

PearsonVII::PearsonVII(double shift, double scale, double shape){
    if (shape <= 0.5 || scale <= 0) throw PearsonException("Invalid parameters for pearson distribution (shape > 0.5 and scale > 0).");

    this->shift = shift;
    this->scale = scale;
    this->shape = shape;

    updateNormalizationConstant();
}

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

    file.close();
}

inline void PearsonVII::updateNormalizationConstant(){
    this->normalization_constant = 1.0 / (scale * std::beta(this->shape - 0.5, 0.5) * std::pow(1.0, this->shape - 0.5));
}

double PearsonVII::getShift() const { return this->shift; }
double PearsonVII::getScale() const { return this->scale; }
double PearsonVII::getShape() const { return this->shape; }

double PearsonVII::computeDensity(double x) const {
    double z = (x - this->shift) / this->scale;
    return this->normalization_constant * std::pow(1.0 + z * z, -this->shape);
}

double PearsonVII::computeExpectation() const {
    if (this->shape <= 1.0){
        throw PearsonException("Expectation does not exist for shape <= 1");
    }
    return this->shift;
}

double PearsonVII::computeVariance() const {
    if (this->shape <= 1.5){
        throw PearsonException("Variance does not exist for shape <= 1.5");
    }
    return this->scale * this->scale / (2.0 * this->shape - 3.0);
}

double PearsonVII::computeSkewness() const {
    if (this->shape <= 1.5){
        throw PearsonException("Skewness does not exist for shape <= 1.5");
    }
    return 0.0;
}

double PearsonVII::computeKurtosis() const {
    if (this->shape <= 2.0){
        throw PearsonException("Kurtosis does not exist for shape <= 2");
    }
    return 6.0 / (2.0 * this->shape - 5.0);
}

double PearsonVII::generateRandom() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    double u = dis(gen);
    double t = std::pow(u, -1.0 / (this->shape - 0.5)) - 1.0;
    
    return this->shift + this->scale * std::sqrt(t) * std::cos(2.0 * M_PI * dis(gen));
}

void PearsonVII::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()){
        throw PearsonException("Cannot open file for writing: " + filename);
    }
    
    file << this->shift << " " << this->scale << " " << this->shape << std::endl;
    file.close();
}
