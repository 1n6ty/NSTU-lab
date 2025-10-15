/**
 * @file    empirical.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for empirical distribution 
 *          structures and functions.
 */

#ifndef EMPIRICAL_H
#define EMPIRICAL_H

/**
 * @enum Empirical_Status
 * @version 1.0
 * 
 * @brief Object with all status codes that could be produced by functions
 */
enum Empirical_Status {
    EMPIRICAL_SUCCESS = 0
};

/**
 * @struct  Empirical_p
 * @version 1.0
 * 
 * @brief   The structure of parameters for empirical distribution.
 * 
 * @param   n count of points @p x
 * @param   x array of points
 * @param   freq relative frequences
 * @param   k count of relative frequences
 * @param   min_x minimum of x
 * @param   max_x maximum of x
 */
struct Empirical_p{
    unsigned int n;
    double R; 
    double *x;
    unsigned int *freq;
    unsigned int k;
    double min_x;
    double max_x;
};

/**
 * @version 1.0
 * 
 * @brief   Constructor of the structure for empirical distribution.
 * 
 * @param   n count of points @p x
 * @param   x array of points
 * @return  pointer to new empirical structure
 */
Empirical_p *new_Empirical_p(double *x, unsigned int n);

/**
 * @version 1.0
 * 
 * @brief   Destructor of the structure for empirical distribution.
 * 
 * @param   p pointer to be freed  
 */
void del_Empirical_p(Empirical_p *p);

/**
 * @version 1.0
 * 
 * @section Empirical
 *  
 *          Functions to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for empirical distribution.
 *          Also function to generate random variable under this distribution.
 */
double empirical_compute_density(double x, Empirical_p *p, Empirical_Status *status);
double empirical_compute_mat_expectation(Empirical_p *p, Empirical_Status *status);
double empirical_compute_dispersion(Empirical_p *p, Empirical_Status *status);
double empirical_compute_skewness(Empirical_p *p, Empirical_Status *status);
double empirical_compute_excess(Empirical_p *p, Empirical_Status *status);
double empirical_generate_x(Empirical_p *p, Empirical_Status *status);

#endif