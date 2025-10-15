/**
 * @file    alt_pirson.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for altered Pirson-VII distribution 
 *          (shear-scale transformed) structures and functions.
 */

#ifndef ALT_PIRSON_H
#define ALT_PIRSON_H

#include "pirson.h"

/**
 * @enum Alt_Pirson_Status
 * @version 1.0
 * 
 * @brief Object with all status codes that could be produced by functions
 */
enum Alt_Pirson_Status {
    ALT_PIRSON_INVALID_PARAMETER = -1,
    ALT_PIRSON_SUCCESS = 0
};

/**
 * @struct  Alt_Pirson_p
 * @version 1.0
 * 
 * @brief   The structure of parameters for altered Pirson-VII distribution.
 * 
 * @param   dp base Pirson-VII distribution parameters
 * @param   u shear parameter
 * @param   a scale parameter   
 */
struct Alt_Pirson_p{
    Pirson_p *dp;
    double u;
    double a;
};

/**
 * @version 1.0
 * 
 * @brief   Constructor of the structure for altered Pirson-VII distribution.
 * 
 * @param   dp base Pirson-VII distribution parameters
 * @param   u shear parameter
 * @param   a scale parameter   
 * @return  pointer to new altered Pirson structure
 */
Alt_Pirson_p *new_Alt_Pirson_p(Pirson_p *dp, double u, double a);

/**
 * @version 1.0
 * 
 * @brief   Destructor of the structure for altered Pirson-VII distribution.
 * 
 * @param   p pointer to be freed  
 */
void del_Alt_Pirson_p(Alt_Pirson_p *p);

/**
 * @version 1.0
 * 
 * @section Altered Pirson-VII
 *  
 *          Functions to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for altered pirson-VII distribution.
 *          Also function to generate random variable under this distribution.
 */
double pirson_compute_density(double x, Alt_Pirson_p *p, Alt_Pirson_Status *status);
double pirson_compute_mat_expectation(Alt_Pirson_p *p, Alt_Pirson_Status *status);
double pirson_compute_dispersion(Alt_Pirson_p *p, Alt_Pirson_Status *status);
double pirson_compute_skewness(Alt_Pirson_p *p, Alt_Pirson_Status *status);
double pirson_compute_excess(Alt_Pirson_p *p, Alt_Pirson_Status *status);
double pirson_generate_x(Alt_Pirson_p *p, Alt_Pirson_Status *status);

#endif