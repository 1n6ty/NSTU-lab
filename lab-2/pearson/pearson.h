/**
 * @file    pirson.h
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Header file for Pearson-VII distribution class.
 */

#ifndef PEARSON_H
#define PEARSON_H

#include <string>
#include <stdexcept>

class PearsonVII {
private:
    double shift;
    double scale;
    double shape;
    
    double normalization_constant;
    
    inline void updateNormalizationConstant();

public:
    PearsonVII(double shift = 0.0, double scale = 1.0, double shape = 2.0);
    PearsonVII(const std::string& filename);
    
    double getShift() const;
    double getScale() const; 
    double getShape() const;
    
    double computeDensity(double x) const;
    double computeExpectation() const;
    double computeVariance() const;
    double computeSkewness() const;
    double computeKurtosis() const;
    double generateRandom() const;
    
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

// Исключения для класса
class PearsonException : public std::runtime_error {
public:
    explicit PearsonException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif