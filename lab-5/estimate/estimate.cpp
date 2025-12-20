/**
 * @file    estimate.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Implementation of Estimate class.
 */

#include <cmath>
#include <algorithm>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>

#include "../empirical/empirical.h"
#include "../core/interface.h"
#include "estimate.h"

/// Convergence threshold for the iterative process
#define TOL 1e-4
/// Maximum number of iterations for a single initial guess
#define MAX_ITER 200

/**
 * @brief Computes the value of the loss function (rho-function).
 * 
 * @param x Standardized residual (z = (data - mu) / sigma).
 * @return The value of the loss function at point @ref x.
 */
double Estimate::rho(double x) const {
    const double x_sq = x * x;

    if (this->c == 0) return -std::exp(-x_sq / 2.0);

    double denominator = 1.0 + this->c * std::exp(-x_sq / 4.0);
    return -1. / denominator - std::log(denominator);
}

/**
 * @brief Computes the value of the weight function w(z).
 * 
 * @details The weight function is used to reweight data points during the iterative process.
 * 
 * @param x Standardized residual (z = (data - mu) / sigma).
 * @return The weight value at point @ref x.
 */
double Estimate::weight(double x) {
    const double x_sq = x * x;

    double denominator = 1.0 + this->c * std::exp(-x_sq / 4.0);
    return std::exp(-x_sq / 2.0) / (denominator * denominator);
}

/**
 * @brief Main algorithm to find the robust estimate of the location parameter (mu).
 */
void Estimate::estimate() {
    const double* data = e.getSample();
    size_t n = e.getSize();

    std::vector<double> initial_guesses(this->p);

    std::vector<double> sorted_data(data, data + n);
    std::sort(sorted_data.begin(), sorted_data.end());
    initial_guesses[0] = (n % 2 == 0) ?
        (sorted_data[n/2 - 1] + sorted_data[n/2]) / 2.0 :
        sorted_data[n/2];

    if (this->p > 1) initial_guesses[1] = e.computeExpectation();

    if (this->p > 2) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(e.getMin(), e.getMax());

        for (int i = 2; i < this->p; ++i) initial_guesses[i] = dis(gen);
    }

    double best_mu = 0.0;
    double min_loss = std::numeric_limits<double>::max();

    for (double init_mu : initial_guesses) {
        double current_mu = init_mu,
            prev_mu = current_mu + 10.0;

        int iter = 0;
        while (std::abs(current_mu - prev_mu) > TOL && iter < MAX_ITER) {
            prev_mu = current_mu;

            double numerator = 0.0;
            double denominator = 0.0;

            for (size_t i = 0; i < n; ++i) {
                double z = (data[i] - current_mu) / sigma;
                double w_z = Estimate::weight(z);

                numerator += data[i] * w_z;
                denominator += w_z;
            }

            if (std::abs(denominator) > 1e-10) current_mu = numerator / denominator;
            iter++;
        }

        double loss = 0.0;
        for (size_t i = 0; i < n; ++i) loss += rho((data[i] - current_mu) / sigma);

        if (loss < min_loss) {
            min_loss = loss;
            best_mu = current_mu;
        }
    }

    this->mu = best_mu;
}

/**
 * @brief Constructor for the Estimate class.
 * 
 * @param e0 Reference to the empirical distribution (data object).
 * @param sigma0 Scale parameter (standard deviation).
 * @param c0 Parameter for the compromise function (must be > -1).
 * @param p0 Number of initial guesses for global optimization.
 * @throw std::invalid_argument If parameters c or p are invalid.
 */
Estimate::Estimate(Empirical& e0, double sigma0,  double c0, int p0) :
    e(e0), sigma(sigma0), c(c0), p(p0)
{
    if (this->c <= -1) throw std::invalid_argument("Parameter c must be greater than -1 for power compromise function");
    if (this->p <= 0) throw std::invalid_argument("Number of initial guesses must be positive");

    this->estimate();
    e.attach(this);
}

/// Setters
void Estimate::setSigma(double new_sigma) {
    if (new_sigma <= 1e-15) throw std::invalid_argument("Sigma must be positive");
    this->sigma = new_sigma;
    this->estimate();
}
void Estimate::setC(double new_c) {
    if (new_c <= -1) throw std::invalid_argument("Parameter c must be greater than -1 for power compromise function");
    this->c = new_c;
    this->estimate();
}
void Estimate::setP(int new_p) {
    if (new_p <= 0) throw std::invalid_argument("Number of initial guesses must be positive");
    this->p = new_p;
    this->estimate();
}
