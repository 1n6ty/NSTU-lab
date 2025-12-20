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
#include <forward_list>

#include "../core/interface.h"

class Empirical: public IPersistent, public IDistribution {
private:
    double* sample;
    double* density_bins;
    size_t n;
    size_t k;
    double min_val;
    double max_val;
    double bin_width;

    std::forward_list<IObserver *> observers;

    void buildHistogram();
    void copyFrom(const Empirical& other);

public:
    Empirical();
    Empirical(const double* data, size_t size);
    Empirical(const std::string& filename);

    Empirical(const IDistribution& dist, size_t size);

    Empirical(const Empirical& other);
    Empirical& operator=(const Empirical& other);
    ~Empirical();

    double computeDensity(double x) const override;
    double computeExpectation() const override;
    double computeVariance() const override;
    double computeSkewness() const override;
    double computeKurtosis() const override;
    double generateRandom() const override;

    size_t getSize() const { return n; }
    const double* getSample() const { return sample; }
    double getMin() const { return min_val; }
    double getMax() const { return max_val; }

    void saveToFile(const std::string& filename) const override;

    void attach(IObserver* obs){this->observers.push_front(obs);}
    void detach(IObserver* obs){this->observers.remove(obs);}
    void notify(){for(auto& elem : this->observers) elem->update();}
};

class EmpiricalException : public std::runtime_error {
public:
    explicit EmpiricalException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif