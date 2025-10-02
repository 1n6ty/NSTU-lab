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

#include "alt_pirson.h"
#include <stdlib.h>

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Constructor of the structure for altered Pirson-VII distribution.
 * 
 * @details Also checks if @p a is not `0`
 *          If conditions doesn't met - returns `nullptr`
 * 
 * @param   dp base Pirson-VII distribution parameters
 * @param   u shear parameter
 * @param   a scale parameter   
 * @return  pointer to new altered Pirson structure
 */
Alt_Pirson_p *new_Alt_Pirson_p(Pirson_p *dp, double u, double a){
    if(a <= 0) return nullptr;

    Alt_Pirson_p *p = (Alt_Pirson_p *) malloc(sizeof(Alt_Pirson_p));
    if(p == NULL) return nullptr;
    p->dp = dp;
    p->u = u;
    p->a = a;

    return p;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Destructor of the structure for altered Pirson-VII distribution.
 * 
 * @param   p pointer to be freed  
 */
void del_Alt_Pirson_p(Alt_Pirson_p *p){
    del_Pirson_p(p->dp);
    free(p);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes density of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   x point at which the density will be computed
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double pirson_compute_density(double x, Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    if(p->a == 0){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    Pirson_Status ps;
    double alt_density = pirson_compute_density((x - p->u) / p->a, p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return (1. / p->a) * alt_density;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Mathematical Expectation
 */
double pirson_compute_mat_expectation(Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    Pirson_Status ps;
    pirson_compute_mat_expectation(p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return p->u;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Dispersion of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Dispersion
 */
double pirson_compute_dispersion(Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    Pirson_Status ps;
    double alt_disp = pirson_compute_dispersion(p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return p->a * p->a * alt_disp;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Skewness of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double pirson_compute_skewness(Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    Pirson_Status ps;
    double alt_skew = pirson_compute_skewness(p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return alt_skew;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Excess of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Excess
 */
double pirson_compute_excess(Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    Pirson_Status ps;
    double alt_excess = pirson_compute_excess(p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return alt_excess;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Generates x out of altered pirson-VII distribution.
 * 
 * @details If PIRSON_INVALID_PARAMETER status occured sets ALT_PIRSON_INVALID_PARAMETER
 *          error in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  generated x
 */
double pirson_generate_x(Alt_Pirson_p *p, Alt_Pirson_Status *status){
    *status = ALT_PIRSON_SUCCESS;

    Pirson_Status ps;
    double alt_gen = pirson_generate_x(p->dp, &ps);
    if(ps == PIRSON_INVALID_PARAMETER){
        *status = ALT_PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return p->u + p->a * alt_gen;
}
