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

#include <string>
#include <stdexcept>

#include "../core/interface.h"

template<class D1, class D2>
class Mix : public IPersistent, public IDistribution{
private:
    double p;
    D1 d1;
    D2 d2;

public:
    Mix();
    Mix(const D1& d1, const D2& d2, const double p);
    Mix(const std::string& filename);
    
    D1& getFstComponent();
    D2& getSecComponent();
    double getP() const;

    void setP(double const p);
    
    double computeDensity(const double x) const override;
    double computeExpectation() const override;
    double computeVariance() const override;
    double computeSkewness() const override;
    double computeKurtosis() const override;
    double generateRandom() const override;
    
    void saveToFile(const std::string& filename) const override;
};

// Исключения для класса
class MixException : public std::runtime_error {
public:
    explicit MixException(const std::string& message) 
        : std::runtime_error(message) {}
};

#include "mix.tpp"

#endif