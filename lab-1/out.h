/**
 * @file    out.h
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @section DESCRIPTION
 *
 *          Header file for functions to print/write-into-file distributions.
 */

#ifndef OUT_H
#define OUT_H

#include <string>
#include "pirson.h"

/**
 * @author  Artyom Eroshchenko <eroshhenko.2023@stud.nstu.ru>
 * @version 1.0
 * 
 * @brief   Writes into file
 * 
 * @param   fname file-name
 * @param   x array of function arguments
 * @param   s size of @p x
 * @param   p Pirson-parameters structure
 */
void pwrite_csv(std::string fname, double *x, size_t s, Pirson_p *p);

#endif