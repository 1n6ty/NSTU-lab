/**
 * @file    mix.h
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Header file for mix of two Pearson-VII distributions.
 */

#ifndef MIX_H
#define MIX_H

#include "../pearson/pearson.h"
#include <string>
#include <stdexcept>

class Mix {
private:
    double p;
    PearsonVII d1;
    PearsonVII d2;

public:
    Mix(const PearsonVII& d1, const PearsonVII& d2, const double p);
    Mix(const std::string& filename);
    
    const PearsonVII& getFstComponent() const;
    const PearsonVII& getSecComponent() const;
    double getP() const;
    
    double computeDensity(const double x) const;
    double computeExpectation() const;
    double computeVariance() const;
    double computeSkewness() const;
    double computeKurtosis() const;
    double generateRandom() const;
    
    void saveToFile(const std::string& filename) const;
};

// Исключения для класса
class MixException : public std::runtime_error {
public:
    explicit MixException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif