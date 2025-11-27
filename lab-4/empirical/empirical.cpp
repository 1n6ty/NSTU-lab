/**
 * @file    empirical.cpp
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          File for empirical distribution.
 */

#include "empirical.h"
#include <stdlib.h>
#include <fstream>
#include <random>
#include <cmath>

/**
 * @version 1.0
 * 
 * @brief   Default Constructor of Empirical distribution.
 */
Empirical::Empirical()
    : n(1)
{
    this->sample = new double[n];
    
    buildHistogram();
}

/**
 * @version 1.0
 * 
 * @brief   Function to build bins, put values at them and compute density.
 */
void Empirical::buildHistogram() {
    this->min_val = this->sample[0];
    this->max_val = this->sample[0];
    for (size_t i = 1; i < n; ++i) {
        if (this->sample[i] < this->min_val) this->min_val = sample[i];
        if (this->sample[i] > this->max_val) this->max_val = sample[i];
    }

    if (this->min_val == this->max_val) {
        this->k = 1;
        this->bin_width = 1.0;
    } else {
        this->k = static_cast<size_t>(std::floor(std::log2(static_cast<double>(n))) + 1);
        this->bin_width = (this->max_val - this->min_val) / static_cast<double>(k);
    }

    this->density_bins = new double[k]();

    double norm_factor;
    size_t bin;
    for (size_t i = 0; i < n; ++i) {
        if (this->sample[i] >= this->max_val) {
            bin = k - 1;
        } else {
            bin = static_cast<size_t>((this->sample[i] - this->min_val) / this->bin_width);
            if (bin >= k) bin = k - 1;
        }
        this->density_bins[bin] += 1.0;
    }

    norm_factor = static_cast<double>(n) * bin_width;
    for (size_t i = 0; i < k; ++i) this->density_bins[i] /= norm_factor;
}

/**
 * @version 1.0
 * 
 * @brief   Deep copy function.
 */
void Empirical::copyFrom(const Empirical& other) {
    this->n = other.n;
    this->k = other.k;
    this->min_val = other.min_val;
    this->max_val = other.max_val;
    this->bin_width = other.bin_width;

    this->sample = new double[n];
    for (size_t i = 0; i < n; ++i) this->sample[i] = other.sample[i];

    this->density_bins = new double[k];
    for (size_t i = 0; i < k; ++i) this->density_bins[i] = other.density_bins[i];
}

Empirical::Empirical(const double* data, size_t size)
    : n((size > 0) ? size: throw EmpiricalException("Sample size must be positive."))
{
    if (!data) throw EmpiricalException("Null pointer passed as sample data.");
    this->sample = new double[n];
    for (size_t i = 0; i < n; ++i) this->sample[i] = data[i];
    
    buildHistogram();
}

Empirical::Empirical(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw EmpiricalException("Cannot open file: " + filename);
    }

    const size_t initial_capacity = 64;
    size_t capacity = initial_capacity,
           size = 0;
    double* data = new double[capacity];

    double val;
    while (file >> val) {
        if (size >= capacity) {
            size_t new_capacity = capacity * 2;
            double* new_data = new double[new_capacity];
            for (size_t i = 0; i < size; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        data[size++] = val;
    }
    file.close();

    if (size == 0) {
        delete[] data;
        throw EmpiricalException("File contains no valid numbers.");
    }

    this->n = size;
    this->sample = data;

    buildHistogram();
}

Empirical::Empirical(const IDistribution& dist, size_t size)
    : n((size > 0) ? size: throw EmpiricalException("Sample size must be positive."))
{
    this->sample = new double[n];
    for (size_t i = 0; i < n; ++i) this->sample[i] = dist.generateRandom();
    buildHistogram();
}

Empirical::Empirical(const Empirical& other) {
    copyFrom(other);
}

Empirical& Empirical::operator=(const Empirical& other) {
    if (this == &other) return *this;

    delete[] this->sample;
    delete[] this->density_bins;

    copyFrom(other);
    return *this;
}

Empirical::~Empirical() {
    delete[] this->sample;
    delete[] this->density_bins;
}

/**
 * @version 1.0
 * 
 * @brief   Computes density of empirical distribution.
 * 
 * @param   x point at which the density will be computed
 * 
 * @return  probability density at @p x point
 */
double Empirical::computeDensity(double x) const {
    if (x < this->min_val || x > this->max_val) return 0.0;

    size_t bin;
    if (x >= this->max_val) {
        bin = k - 1;
    } else {
        bin = static_cast<size_t>((x - this->min_val) / this->bin_width);
        if (bin >= k) bin = k - 1;
    }
    return this->density_bins[bin];
}

/**
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of empirical distribution.
 * 
 * @return  Mathematical Expectation
 */
double Empirical::computeExpectation() const {
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) sum += this->sample[i];
    return sum / static_cast<double>(this->n);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Variance of empirical distribution.
 * 
 * @return  Variance
 */
double Empirical::computeVariance() const {
    double mean = computeExpectation(),
           sum = 0.0, diff;
    for (size_t i = 0; i < n; ++i) {
        diff = this->sample[i] - mean;
        sum += diff * diff;
    }
    return sum / static_cast<double>(this->n);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Skewness of empirical distribution.
 * 
 * @return  Skewness
 */
double Empirical::computeSkewness() const {
    double mean = computeExpectation(),
           var = computeVariance();
    
    if (var == 0.0) return 0.0;

    double sigma = std::sqrt(var),
           sum = 0.0, z;
    for (size_t i = 0; i < n; ++i) {
        z = (this->sample[i] - mean) / sigma;
        sum += z * z * z;
    }
    return sum / static_cast<double>(this->n);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Kurtosis of empirical distribution.
 * 
 * @return  Kurtosis
 */
double Empirical::computeKurtosis() const {
    double mean = computeExpectation(),
           var = computeVariance();
    
    if (var == 0.0) return 0.0;

    double sigma = std::sqrt(var),
           sum = 0.0, z;
    for (size_t i = 0; i < n; ++i) {
        z = (this->sample[i] - mean) / sigma;
        sum += z * z * z * z;
    }
    return sum / static_cast<double>(this->n) - 3.0;
}

/**
 * @version 1.0
 * 
 * @brief   Generates x out of empirical distribution.
 * 
 * @return  generated x
 */
double Empirical::generateRandom() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    double u = dis(gen);
    double cum_prob = 0.0;

    for (size_t i = 0; i < k; ++i) {
        double bin_prob = this->density_bins[i] * this->bin_width;
        cum_prob += bin_prob;
        if (u <= cum_prob) {
            double local_u = dis(gen);
            return this->min_val + static_cast<double>(i) * this->bin_width + local_u * this->bin_width;
        }
    }

    return this->min_val + dis(gen) * (this->max_val - this->min_val);
}

/**
 * @version 2.0
 * 
 * @brief   Saves samples of empirical distribution to file.
 * 
 * @param   filename Name of file where samples will be written
 */
void Empirical::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) throw EmpiricalException("Cannot open file for writing: " + filename);

    for (size_t i = 0; i < n; ++i) {
        if (i > 0) file << ' ';
        file << this->sample[i];
    }
    file << '\n';

    file.close();
}
