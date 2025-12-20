/**
 * @file    interface.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for all interfaces.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

struct IDistribution {
    virtual double computeDensity(double x) const = 0;
    virtual double computeExpectation() const = 0;
    virtual double computeVariance() const = 0;
    virtual double computeSkewness() const = 0;
    virtual double computeKurtosis() const = 0;
    virtual double generateRandom() const = 0;
};

struct IPersistent {
    virtual void saveToFile(const std::string& filename) const = 0;
};

struct IObserver {
    virtual void update() = 0;
};

#endif