/**
 * @file    empirical.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for empirical distribution.
 */

#ifndef EMPIRICAL_H
#define EMPIRICAL_H

#include <string>
#include <stdexcept>

#include "../pearson/pearson.h"
#include "../mix/mix.h"

class Empirical {
private:
    double* sample;
    double* density_bins;
    size_t n;
    size_t k;
    double min_val;
    double max_val;
    double bin_width;

    void buildHistogram();
    void copyFrom(const Empirical& other);

public:
    Empirical(const double* data, size_t size);
    Empirical(const std::string& filename);
    Empirical(const PearsonVII& dist, size_t size);
    Empirical(const Mix& dist, size_t size);
    Empirical(const Empirical& other);
    Empirical& operator=(const Empirical& other);
    ~Empirical();

    double computeDensity(double x) const;
    double computeExpectation() const;
    double computeVariance() const;
    double computeSkewness() const;
    double computeKurtosis() const;
    double generateRandom() const;

    size_t getSize() const { return n; }
    const double* getSample() const { return sample; }
    double getMin() const { return min_val; }
    double getMax() const { return max_val; }

    void saveToFile(const std::string& filename) const;
};

class EmpiricalException : public std::runtime_error {
public:
    explicit EmpiricalException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif