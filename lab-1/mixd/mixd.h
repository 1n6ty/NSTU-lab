/**
 * @file    mixd.h
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for mix of two Pirson-VII distributions
 *          structures and functions.
 */

#ifndef MIXD_H
#define MIXD_H

#include "../pirson/alt_pirson.h"

/**
 * @enum Mix_Status
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief Object with all status codes that could be produced by functions
 */
enum Mix_Status {
    MIX_INVALID_PARAMETER = -1,
    MIX_SUCCESS = 0
};

/**
 * @struct  Mix_p
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   The structure of parameters for Mix distribution.
 * 
 * @param   params altered Pirson-VII distribution parameters array
 * @param   coefs mix parameters
 * @param   size size of @p params and @p coefs
 */
struct Mix_p{
    Alt_Pirson_p* params;
    double *coefs;
    unsigned int size;
};

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Constructor of the structure for Mix distribution.
 * 
 * @param   params altered Pirson-VII distribution parameters array
 * @param   coefs mix parameters
 * @param   size size of @p params and @p coefs
 * @return  pointer to new Mix structure
 */
Mix_p *new_Mix_p(Alt_Pirson_p* params, double *coefs, unsigned int size);

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Destructor of the structure for Mix distribution.
 * 
 * @param   m Mix distribution parameters 
 */
void del_Mix_p(Mix_p *m);

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
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

#endif