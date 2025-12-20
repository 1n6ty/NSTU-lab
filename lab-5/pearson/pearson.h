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

#include "../core/interface.h"

class PearsonVII : public IPersistent, public IDistribution {
private:
    double shift;
    double scale;
    double shape;
    
    double normalization_constant;
    
    inline void updateNormalizationConstant();

public:
    PearsonVII(double shift = 0.0, double scale = 1.0, double shape = 2.0);
    PearsonVII(const std::string& filename);
    
    void setShift(double const shift);
    void setScale(double const scale);
    void setShape(double const shape);

    double getShift() const;
    double getScale() const; 
    double getShape() const;
    
    double computeDensity(double x) const override;
    double computeExpectation() const override;
    double computeVariance() const override;
    double computeSkewness() const override;
    double computeKurtosis() const override;
    double generateRandom() const override;
    
    void saveToFile(const std::string& filename) const override;
};

// Исключения для класса
class PearsonException : public std::runtime_error {
public:
    explicit PearsonException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif