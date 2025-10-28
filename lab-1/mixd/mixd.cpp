/**
 * @file    mixd.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for Mix of two Pearson-VII distributions
 *          (shear-scale transformed). For more see @ref mixd.h "Mix of two Pearson-VII header file".
 */

#include "mixd.h"
#include <stdlib.h>
#include <cmath>
#include <random>

#define EPS 1e-15

/**
 * @version 2.0
 * 
 * @brief   Constructor of the structure for Mix distribution.
 * 
 * @details Also checks if coefficients are in `[0, 1]` and their sum is equal to `1`
 *          If conditions doesn't met - returns `nullptr`
 * 
 * @param   distributions vector of TransformedPearson distributions
 * @param   coefficients mix coefficients
 * @return  pointer to new Mix structure
 */
Mix_p *new_Mix_p(const std::vector<TransformedPearson>& distributions, 
                 const std::vector<double>& coefficients) {
    
    if (distributions.size() != coefficients.size()) {
        return nullptr;
    }
    
    double sum = 0;
    for (double coef : coefficients) {
        if (coef < 0 || coef > 1) return nullptr;
        sum += coef;
    }
    
    if (std::abs(sum - 1.0) > EPS) return nullptr;
    
    Mix_p *m = new Mix_p;
    m->distributions = distributions;
    m->coefficients = coefficients;
    
    return m;
}

/**
 * @version 2.0
 * 
 * @brief   Destructor of the structure for Mix distribution.
 * 
 * @param   m Mix distribution parameters 
 */
void del_Mix_p(Mix_p *m) {
    delete m;
}

/**
 * @version 2.0
 * 
 * @brief   Computes density of mix distribution.
 * 
 * @param   x point at which the density will be computed
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double mix_compute_density(double x, Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        double cum_dens = 0;
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            cum_dens += m->coefficients[i] * m->distributions[i].computeDensity(x);
        }
        return cum_dens;
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}

/**
 * @version 2.0
 * 
 * @brief   Computes Mathematical Expectation of mix distribution.
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Mathematical Expectation
 */
double mix_compute_mat_expectation(Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        double cum_mat_exp = 0;
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            cum_mat_exp += m->coefficients[i] * m->distributions[i].computeExpectation();
        }
        return cum_mat_exp;
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}

/**
 * @version 2.0
 * 
 * @brief   Computes Dispersion of mix distribution.
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Dispersion
 */
double mix_compute_dispersion(Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        double cum_md = 0;
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            double expectation = m->distributions[i].computeExpectation();
            double variance = m->distributions[i].computeVariance();
            cum_md += m->coefficients[i] * (expectation * expectation + variance);
        }

        double mix_expectation = mix_compute_mat_expectation(m, status);
        if (*status != MIX_SUCCESS) {
            return -1;
        }
        
        cum_md -= mix_expectation * mix_expectation;
        return cum_md;
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}

/**
 * @version 2.0
 * 
 * @brief   Computes Skewness of mix distribution.
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double mix_compute_skewness(Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        double mix_expectation = mix_compute_mat_expectation(m, status);
        if (*status != MIX_SUCCESS) {
            return -1;
        }
        
        double mix_variance = mix_compute_dispersion(m, status);
        if (*status != MIX_SUCCESS) {
            return -1;
        }
        
        double cum_skew = 0;
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            
            double expectation = m->distributions[i].computeExpectation();
            double variance = m->distributions[i].computeVariance();
            double skewness = m->distributions[i].computeSkewness();
            
            double term1 = std::pow(expectation - mix_expectation, 3);
            double term2 = 3 * (expectation - mix_expectation) * variance;
            double term3 = std::pow(variance, 1.5) * skewness;
            
            cum_skew += m->coefficients[i] * (term1 + term2 + term3);
        }

        return cum_skew / std::pow(mix_variance, 1.5);
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}

/**
 * @version 2.0
 * 
 * @brief   Computes Excess of mix distribution.
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Excess
 */
double mix_compute_excess(Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        double mix_expectation = mix_compute_mat_expectation(m, status);
        if (*status != MIX_SUCCESS) {
            return -1;
        }
        
        double mix_variance = mix_compute_dispersion(m, status);
        if (*status != MIX_SUCCESS) {
            return -1;
        }
        
        double cum_excess = 0;
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            
            double expectation = m->distributions[i].computeExpectation();
            double variance = m->distributions[i].computeVariance();
            double skewness = m->distributions[i].computeSkewness();
            double kurtosis = m->distributions[i].computeKurtosis();
            
            double term1 = std::pow(expectation - mix_expectation, 4);
            double term2 = 6 * std::pow(expectation - mix_expectation, 2) * variance;
            double term3 = 4 * (expectation - mix_expectation) * std::pow(variance, 1.5) * skewness;
            double term4 = variance * variance * (kurtosis + 3);
            
            cum_excess += m->coefficients[i] * (term1 + term2 + term3 + term4);
        }

        return cum_excess / std::pow(mix_variance, 2) - 3;
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}

/**
 * @version 2.0
 * 
 * @brief   Generates x out of mix distribution.
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  generated x
 */
double mix_generate_x(Mix_p *m, Mix_Status *status) {
    *status = MIX_SUCCESS;

    try {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        
        double r = dis(gen);
        double cum_p = 0;
        
        for (size_t i = 0; i < m->distributions.size(); i++) {
            if (!m->distributions[i].isValid()) {
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            
            cum_p += m->coefficients[i];
            if (r <= cum_p + EPS) {
                return m->distributions[i].generateRandom();
            }
        }
        
        // Если из-за ошибок округления не выбрали распределение, возвращаем последнее
        return m->distributions.back().generateRandom();
    } catch (const TransformedPearsonException& e) {
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
}