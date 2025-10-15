/**
 * @file    pirson.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for Pirson-VII distribution structures and functions.
 */

#ifndef PIRSON_H
#define PIRSON_H

/**
 * @enum Pirson_Status
 * @version 1.0
 * 
 * @brief Object with all status codes that could be produced by functions
 */
enum Pirson_Status {
    PIRSON_INVALID_PARAMETER = -1,
    PIRSON_SUCCESS = 0
};

/**
 * @struct  Pirson_p
 * @version 1.0
 * 
 * @brief   The structure of parameters for Pirson-VII distribution.
 * 
 * @param   v shape parameter
 */
struct Pirson_p{
    double v;
};

/**
 * @version 1.0
 * 
 * @brief   Constructor of the structure for Pirson-VII distribution.
 * 
 * @param   v shape parameter
 * @return  pointer to new Pirson structure
 */
Pirson_p *new_Pirson_p(double v);

/**
 * @version 1.0
 * 
 * @brief   Destructor of the structure for Pirson-VII distribution.
 * 
 * @param   p pointer to be freed
 */
void del_Pirson_p(Pirson_p *p);

/**
 * @version 1.0
 * 
 * @section Pirson-VII
 *  
 *          Functions to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for pirson-VII distribution.
 *          Also function to generate random variable under this distribution.
 */
double pirson_compute_density(double x, Pirson_p *p, Pirson_Status *status);
double pirson_compute_mat_expectation(Pirson_p *p, Pirson_Status *status);
double pirson_compute_dispersion(Pirson_p *p, Pirson_Status *status);
double pirson_compute_skewness(Pirson_p *p, Pirson_Status *status);
double pirson_compute_excess(Pirson_p *p, Pirson_Status *status);
double pirson_generate_x(Pirson_p *p, Pirson_Status *status);

#endif