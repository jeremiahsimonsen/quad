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

#include "pass05_stop3.h"

float pass05__b_coef[8][3] = {{1, 1.6981210746, 1},{1, 0.332264013216, 1},{1, -0.611754501065, 1},{1, -1.09028678073, 1},{1, -1.33634156747, 0.999999999998},{1, -1.46835640212, 1.00000000001},{1, -1.53827732733, 0.999999999987},{1, -1.56885562705, 1.00000000001}};
float pass05__a_coef[8][3] = {{1, -1.72239309577, 0.741231451976},{1, -2.34790137249, 1.38093921449},{1, -1.68938471822, 0.720127957568},{1, -2.2665943881, 1.30529554099},{1, -1.74554832728, 0.783698615874},{1, -2.13143181753, 1.17669509313},{1, -1.84574888388, 0.890544513087},{1, -1.98063861402, 1.02841374802}};
float pass05__g = 9.79310676897e-19;
int pass05__num_stages = 8;