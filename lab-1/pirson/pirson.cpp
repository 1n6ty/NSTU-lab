/**
 * @file    pirson.cpp
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          This file initializes structures and functions for Pirson-VII 
 *          distribution. For more see @ref pirson.h "Pirson-VII header file".
 */

#include "pirson.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Constructor of the structure for Pirson-VII distribution.
 * 
 * @param   v shape parameter
 * @return  pointer to new Pirson structure
 */
Pirson_p *new_Pirson_p(double v){
    Pirson_p *p = (Pirson_p *) malloc(sizeof(Pirson_p));
    if(p == NULL) return nullptr;
    p->v = v;

    return p;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Destructor of the structure for Pirson-VII distribution.
 * 
 * @param   p pointer to be freed
 */
void del_Pirson_p(Pirson_p *p){
    free(p);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes density of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `0.5` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   x point at which the density will be computed
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  probability density at @p x point
 */
double pirson_compute_density(double x, Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;
    
    if(p->v <= 0.5){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }
    
    return pow(1 + x * x, -p->v) / std::beta(p->v - 0.5, 0.5);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Mathematical Expectation of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `1` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Mathematical Expectation
 */
double pirson_compute_mat_expectation(Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;
    
    if(p->v <= 1){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }
    
    return 0;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Dispersion of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `1.5` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Dispersion
 */
double pirson_compute_dispersion(Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;

    if(p->v <= 1.5){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }
    
    return 1 / (2 * p->v - 3);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Skewness of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `1.5` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Skewness
 */
double pirson_compute_skewness(Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;
    
    if(p->v <= 1.5){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }
    
    return 0;
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Computes Excess of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `2.5` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  Excess
 */
double pirson_compute_excess(Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;
    
    if(p->v <= 2.5){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }
    
    return 6 / (2 * p->v - 5);
}

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Generates out of pirson-VII distribution.
 * 
 * @details If `p->v` is less or equal `0.5` then PIRSON_INVALID_PARAMETERS
 *          error sets in status
 * 
 * @param   p pointer to parameters-structure
 * @param   status pointer to a variable where to put status codes
 * @return  generated x
 */
double pirson_generate_x(Pirson_p *p, Pirson_Status *status){
    *status = PIRSON_SUCCESS;
    
    if(p->v <= 0.5){
        *status = PIRSON_INVALID_PARAMETER;
        return -1;
    }

    return std::sqrt(
        std::pow((double)rand() / RAND_MAX, -1. / (p->v - 0.5)) - 1
    ) * cos(2. * M_PI * rand() / RAND_MAX);
}
