/**
 * @file    alt_pirson.cpp
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Constructor of the structure for empirical distribution.
 * 
 * @param   n count of points @p x
 * @param   x array of points
 * @return  pointer to new empirical structure
 */
Empirical_p *new_Empirical_p(double *x, unsigned int n){
    if(n == 0) return nullptr;

    Empirical_p *ep = (Empirical_p *) malloc(sizeof(Empirical_p));
    if(ep == NULL) return nullptr;

    ep->x = x;
    ep->n = n;

    ep->min_x = __DBL_MAX__;
    ep->max_x = -__DBL_MAX__;
    for(unsigned int i = 0; i < n; i++){
        if(x[i] < ep->min_x) ep->min_x = x[i];
        if(x[i] > ep->max_x) ep->max_x = x[i];
    }

    ep->k = (unsigned int)std::log2(n) + 1;
    double R = (ep->max_x - ep->min_x) / ep->k;

    unsigned int *freq = (unsigned int *) malloc(sizeof(unsigned int) * ep->k);
    if(freq == NULL) return nullptr;

    double prev = ep->min_x;
    double next = prev + R, buf;
    for(unsigned int i = 0; i < ep->k; i++){
        if(i == ep->k - 1){
            next = ep->max_x;
        }

        buf = 0;
        for(unsigned int xi = 0; xi < n; xi++){
            if(x[xi] <= next and prev < x[xi]) buf ++;
        }
        freq[i] = buf;

        prev = next;
        next += R;
    }
    freq[0] ++;
    ep->freq = freq;

    return ep;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes density of empirical distribution.
 * 
 * @param   x point at which the density will be computed
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double empirical_compute_density(double x, Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    if(x <= p->min_x || x > p->max_x) return 0;

    double R = (p->max_x - p->min_x) / p->k;
    double prev = p->min_x;
    double next = prev + R;
    for(unsigned int i = 0; i < p->k; i++){
        if(i == p->k - 1){
            next = p->max_x;
        }

        if(x <= next and prev < x) return p->freq[i] * p->k / p->n / (p->max_x - p->min_x);

        prev = next;
        next += R;
    }

    return 0;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
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
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Generates x out of empirical distribution.
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  generated x
 */
double empirical_generate_x(Empirical_p *p, Empirical_Status *status){
    *status = EMPIRICAL_SUCCESS;

    double r = (double)rand() / RAND_MAX, cum_p = 0, R = (p->max_x - p->min_x) / p->k;;
    for(unsigned int i = 0; i < p->k; i++){
        cum_p += p->freq[i] / p->n;
        if(cum_p + EPS >= r) return p->min_x + R * (i + (double)rand() / RAND_MAX);
    }

    return p->min_x + R * (p->k - 1 + (double)rand() / RAND_MAX);
}
