/**
 * @file    alt_pirson.cpp
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for altered Pirson-VII distribution 
 *          (shear-scale transformed). For more see @ref alt_pirson.h "Altered Pirson-VII header file".
 */

#include "empirical.h"
#include <stdlib.h>
#include <cmath>

#define EPS 1e-15

/**
 * @version 1.0
 * 
 * @brief   Constructor of the structure for empirical distribution.
 * 
 * @param   n count of points @p x
 * @param   x array of points
 * @return  pointer to new empirical structure
 */
Empirical_p *new_Empirical_p(double *x, unsigned int n) {
    if (n == 0) return nullptr;

    Empirical_p *ep = (Empirical_p *)malloc(sizeof(Empirical_p));
    if (!ep) return nullptr;

    ep->x = x;
    ep->n = n;

    ep->min_x = __DBL_MAX__;
    ep->max_x = -__DBL_MAX__;
    for (unsigned int i = 0; i < n; i++) {
        if (x[i] < ep->min_x) ep->min_x = x[i];
        if (x[i] > ep->max_x) ep->max_x = x[i];
    }

    ep->k = (unsigned int)std::log2(n) + 1;
    ep->R = (ep->max_x - ep->min_x) / ep->k;

    unsigned int *freq = (unsigned int *)calloc(ep->k, sizeof(unsigned int));
    if (!freq) return nullptr;

    for (unsigned int i = 0; i < n; i++) {
        int bin = (int)((x[i] - ep->min_x) / ep->R);
        if (bin >= (int)ep->k) bin = ep->k - 1;
        freq[bin]++;
    }

    ep->freq = freq;
    return ep;
}

/**
 * @version 1.0
 * 
 * @brief   Destructor of the structure for empirical distribution.
 * 
 * @param   p pointer to be freed  
 */
void del_Empirical_p(Empirical_p *p){
    free(p->x);
    free(p->freq);
    free(p);
}

/**
 * @version 1.0
 * 
 * @brief   Computes density of empirical distribution.
 * 
 * @param   x point at which the density will be computed
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double empirical_compute_density(double x, Empirical_p *p, Empirical_Status *status) {
    *status = EMPIRICAL_SUCCESS;
    if (x < p->min_x || x > p->max_x) return 0;

    int bin = (int)((x - p->min_x) / p->R);
    if (bin >= (int)p->k) bin = p->k - 1;

    double density = (double)p->freq[bin] / (p->n * p->R);
    return density;
}

/**
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Mathematical Expectation
 */
double empirical_compute_mat_expectation(Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    double cum_sum = 0;
    for(unsigned int i = 0; i < p->n; i++) cum_sum += p->x[i];

    return cum_sum / p->n;
}

/**
 * @version 1.0
 * 
 * @brief   Computes Dispersion of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Dispersion
 */
double empirical_compute_dispersion(Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    Empirical_Status es;
    double mat_exp = empirical_compute_mat_expectation(p, &es), cum_sum = 0;
    for(unsigned int i = 0; i < p->n; i++) cum_sum += std::pow(p->x[i] - mat_exp, 2);

    return cum_sum / p->n;
}

/**
 * @version 1.0
 * 
 * @brief   Computes Skewness of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double empirical_compute_skewness(Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    Empirical_Status es;
    double mat_exp = empirical_compute_mat_expectation(p, &es), cum_sum = 0;
    for(unsigned int i = 0; i < p->n; i++) cum_sum += std::pow(p->x[i] - mat_exp, 3);

    return cum_sum / p->n / std::pow(empirical_compute_dispersion(p, &es), 1.5);
}

/**
 * @version 1.0
 * 
 * @brief   Computes Excess of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Excess
 */
double empirical_compute_excess(Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    Empirical_Status es;
    double mat_exp = empirical_compute_mat_expectation(p, &es), cum_sum = 0;
    for(unsigned int i = 0; i < p->n; i++) cum_sum += std::pow(p->x[i] - mat_exp, 4);

    return cum_sum / p->n / std::pow(empirical_compute_dispersion(p, &es), 2) - 3;
}

/**
 * @version 1.0
 * 
 * @brief   Generates x out of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  generated x
 */
double empirical_generate_x(Empirical_p *p, Empirical_Status *status) {
    *status = EMPIRICAL_SUCCESS;

    double r = (double)rand() / (RAND_MAX + 1.0);
    double cum_p = 0.0;

    for(unsigned int i = 0; i < p->k; i++){
        double bin_prob = (double)p->freq[i] / p->n;
        cum_p += bin_prob;

        if(r <= cum_p){
            double u = (double)rand() / (RAND_MAX + 1.0);
            double x = p->min_x + p->R * (i + u);
            return x;
        }
    }

    return p->max_x - p->R * ((double)rand() / (RAND_MAX + 1.0));
}
