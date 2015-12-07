/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 4 - Source file containing 
 * definitions of filter coefficients for external use.
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 *
 * @date Mar 22, 2015
 *
 * This file contains the definitions of filter coefficients
 * for external use. The coefficients are the output of the 
 * zp2biquad() function. This implementation allows for the
 * testing of new filters simply by re-running the zp2biquad()
 * function and then re-compiling.
 *
 */

#include "pass05_stop4.h"

float pass05__b_coef[4][3] = {{1, -1.98195, 1},{1, -1.99776, 0.999999},{1, -1.999, 1},{1, -1.99927, 0.999996}};
float pass05__a_coef[4][3] = {{1, -1.9991, 0.998764},{1, -1.97252, 0.972875},{1, -2.02416, 1.02449},{1, -1.99759, 0.99794}};
float pass05__g = 1.00221e-10;
int pass05__num_stages = 4;