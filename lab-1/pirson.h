/**
 * @file    pirson.h
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for Pirson-VII distribution structures and functions.
 */

#ifndef PIRSON_H
#define PIRSON_H

#include <string>

/**
 * @struct  Pirson_VII
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   The structure for Pirson-VII distribution.
 * @details This Pirson-VII structure provides functions
 *          to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for pirson-VII distribution.
 *          Also function to generate random variable under this distribution.
 */
struct Pirson_VII{
    double compute_density(double x, struct Pirson_p *p);
    double generate_x(struct Pirson_p *p);
    double compute_mat_expectation(struct Pirson_p *p);
    double compute_dispersion(struct Pirson_p *p);
    double compute_skewness(struct Pirson_p *p);
    double compute_excess(struct Pirson_p *p);
};

/**
 * @struct  Pirson_VII
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   The structure of parameters for Pirson-VII distribution.
 */
struct Pirson_p{
    double v;
};

#endif