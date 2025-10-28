/**
 * @file    mixd.h
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Header file for mix of two Pearson-VII distributions
 *          structures and functions.
 */

#ifndef MIXD_H
#define MIXD_H

#include "../pirson/alt_pirson.h"
#include <vector>
#include <stdexcept>

/**
 * @enum Mix_Status
 * @version 2.0
 * 
 * @brief Object with all status codes that could be produced by functions
 */
enum Mix_Status {
    MIX_INVALID_PARAMETER = -1,
    MIX_SUCCESS = 0
};

/**
 * @struct  Mix_p
 * @version 2.0
 * 
 * @brief   The structure of parameters for Mix distribution.
 * 
 * @param   distributions vector of TransformedPearson distributions
 * @param   coefficients mix coefficients
 */
struct Mix_p{
    std::vector<TransformedPearson> distributions;
    std::vector<double> coefficients;
};

/**
 * @version 2.0
 * 
 * @brief   Constructor of the structure for Mix distribution.
 * 
 * @param   distributions vector of TransformedPearson distributions
 * @param   coefficients mix coefficients
 * @return  pointer to new Mix structure
 */
Mix_p *new_Mix_p(const std::vector<TransformedPearson>& distributions, 
                 const std::vector<double>& coefficients);

/**
 * @version 2.0
 * 
 * @brief   Destructor of the structure for Mix distribution.
 * 
 * @param   m Mix distribution parameters 
 */
void del_Mix_p(Mix_p *m);

/**
 * @version 2.0
 * 
 * @section Mix distribution
 *  
 *          Functions to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for mix distribution.
 *          Also function to generate random variable under this distribution.
 */
double mix_compute_density(double x, Mix_p *m, Mix_Status *status);
double mix_compute_mat_expectation(Mix_p *m, Mix_Status *status);
double mix_compute_dispersion(Mix_p *m, Mix_Status *status);
double mix_compute_skewness(Mix_p *m, Mix_Status *status);
double mix_compute_excess(Mix_p *m, Mix_Status *status);
double mix_generate_x(Mix_p *m, Mix_Status *status);

// Исключения для смеси
class MixException : public std::runtime_error {
public:
    explicit MixException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif