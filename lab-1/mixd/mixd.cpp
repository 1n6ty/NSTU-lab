/**
 * @file    mixd.cpp
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for Mix of two pirson-VII distributions
 *          (shear-scale transformed). For more see @ref mixh.h "Mix of two Pirson-VII header file".
 */

#include "mixd.h"
#include <stdlib.h>
#include <cmath>

#define EPS 1e-15

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Constructor of the structure for Mix distribution.
 * 
 * @details Also checks if @p coefs are in `[0, 1]` and their sum is equal to `1`
 *          If conditions doesn't met - returns `nullptr`
 * 
 * @param   params altered Pirson-VII distribution parameters array
 * @param   coefs mix parameters
 * @param   size size of @p params and @p coefs
 * @return  pointer to new Mix structure
 */
Mix_p *new_Mix_p(Alt_Pirson_p* params, double *coefs, unsigned int size){
    double s = 0;
    for(unsigned int i = 0; i < size; i++){
        if(coefs[i] < 0 || coefs[i] > 1) return nullptr;
        s += coefs[i];
    }
    if(s - 1 > EPS) return nullptr;
    
    Mix_p *m = (Mix_p *) malloc(sizeof(Mix_p));
    if(m == NULL) return nullptr;
    m->params = params;
    m->coefs = coefs;
    m->size = size;

    return m;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Destructor of the structure for Mix distribution.
 * 
 * @param   p Mix distribution parameters 
 */
void del_Mix_p(Mix_p *m){
    for(unsigned int i = 0; i < m->size; i++){
        del_Alt_Pirson_p(m->params + i);
        free(m->coefs + i);
    }
    free(m);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes density of mix distribution.
 * 
 * @details If ALT_PIRSON_INVALID_PARAMETER status occured sets MIX_INVALID_PARAMETER
 *          error in status
 * 
 * @param   x point at which the density will be computed
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double mix_compute_density(double x, Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    double cum_dens = 0;
    for(unsigned int i = 0; i < m->size; i++){
        cum_dens += m->coefs[i] * pirson_compute_density(x, m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
    }
    
    return cum_dens;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of mix distribution.
 * 
 * @details If ALT_PIRSON_INVALID_PARAMETER status occured sets MIX_INVALID_PARAMETER
 *          error in status
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Mathematical Expectation
 */
double mix_compute_mat_expectation(Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    double cum_mat_exp = 0;
    for(unsigned int i = 0; i < m->size; i++){
        cum_mat_exp += m->coefs[i] * pirson_compute_mat_expectation(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
    }
    
    return cum_mat_exp;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Dispersion of mix distribution.
 * 
 * @details If ALT_PIRSON_INVALID_PARAMETER status occured sets MIX_INVALID_PARAMETER
 *          error in status
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Dispersion
 */
double mix_compute_dispersion(Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    double cum_md = 0, buf_m = 0, buf_d = 0;
    for(unsigned int i = 0; i < m->size; i++){
        buf_m = pirson_compute_mat_expectation(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_d = pirson_compute_dispersion(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        cum_md += m->coefs[i] * (buf_m * buf_m + buf_d);
    }

    Mix_Status ms;
    buf_m = mix_compute_mat_expectation(m, &ms);
    if(ms == MIX_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    cum_md -= buf_m * buf_m;

    return cum_md;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Skewness of mix distribution.
 * 
 * @details If ALT_PIRSON_INVALID_PARAMETER status occured sets MIX_INVALID_PARAMETER
 *          error in status
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double mix_compute_skewness(Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    Mix_Status ms;
    double buf_mix_m = mix_compute_mat_expectation(m, &ms);
    if(ms == MIX_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    double buf_mix_d = mix_compute_dispersion(m, &ms);
    if(ms == MIX_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    double cum_skew = 0, buf_m = 0, buf_d = 0, buf_sk = 0;
    for(unsigned int i = 0; i < m->size; i++){
        buf_m = pirson_compute_mat_expectation(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_d = pirson_compute_dispersion(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_sk = pirson_compute_skewness(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        cum_skew += m->coefs[i] * (std::pow(buf_m - buf_mix_m, 3) + 3 * (buf_m - buf_mix_m) * buf_d + std::pow(buf_d, 1.5) * buf_sk);
    }

    return cum_skew / std::pow(buf_mix_d, 1.5);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Excess of mix distribution.
 * 
 * @details If ALT_PIRSON_INVALID_PARAMETER status occured sets MIX_INVALID_PARAMETER
 *          error in status
 * 
 * @param   m pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Excess
 */
double mix_compute_excess(Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    Mix_Status ms;
    double buf_mix_m = mix_compute_mat_expectation(m, &ms);
    if(ms == MIX_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    double buf_mix_d = mix_compute_dispersion(m, &ms);
    if(ms == MIX_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    double cum_skew = 0, buf_m = 0, buf_d = 0, buf_ex = 0, buf_sk = 0;
    for(unsigned int i = 0; i < m->size; i++){
        buf_m = pirson_compute_mat_expectation(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_d = pirson_compute_dispersion(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_sk = pirson_compute_skewness(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        buf_ex = pirson_compute_excess(m->params + i, &ps);
        if(ps == ALT_PIRSON_INVALID_PARAMETER){
            *status = MIX_INVALID_PARAMETER;
            return -1;
        }
        cum_skew += m->coefs[i] * (std::pow(buf_m - buf_mix_m, 4) + 6 * std::pow(buf_m - buf_mix_m, 2) * buf_d + 4 * (buf_m - buf_mix_m) * std::pow(buf_d, 1.5) * buf_sk + buf_d * buf_d * (buf_ex + 3));
    }

    return cum_skew / std::pow(buf_mix_d, 2) - 3;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Generates x out of mix distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double mix_generate_x(Mix_p *m, Mix_Status *status){
    *status = MIX_SUCCESS;

    Alt_Pirson_Status ps;
    double alt_gen = 0, r = (double)rand() / RAND_MAX, cum_p = 0;
    for(unsigned int i = 0; i < m->size; i++){
        cum_p += m->coefs[i];
        if(r <= cum_p + EPS){
            alt_gen = pirson_generate_x(m->params + i, &ps);
            if(ps == ALT_PIRSON_INVALID_PARAMETER){
                *status = MIX_INVALID_PARAMETER;
                return -1;
            }
            return alt_gen;
        }
    }

    alt_gen = pirson_generate_x(m->params + m->size - 1, &ps);
    if(ps == ALT_PIRSON_INVALID_PARAMETER){
        *status = MIX_INVALID_PARAMETER;
        return -1;
    }
    return alt_gen;
}
