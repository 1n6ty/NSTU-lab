/**
 * @file pirson.h
 * @author Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Header file for Pirson-VII distribution structures and functions.
 */

#ifndef PIRSON_H
#define PIRSON_H

/**
 * @struct Pirson_VII
 * @version 1
 * 
 * @brief The structure for Pirson-VII distribution.
 * @details This Pirson-VII structure provides functions
 *          to compute density, mathematical expectation, dispersion,
 *          skewness and excess coefficient for pirson-VII distribution.
 * 
 * @author Artyom Eroshchenko
 */
struct Pirson_VII{
    double compute_density(double x);
    double compute_mat_expectation(double x);
};

#endif