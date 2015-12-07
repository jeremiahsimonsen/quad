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

#include "pass05_stop4v2.h"

float pass05__b_coef[4][3] = {{1, 1.29237465749, 1},{1, -0.538943597193, 1},{1, -1.17959762145, 1},{1, -1.37268562748, 1}};
float pass05__a_coef[4][3] = {{1, -1.91959747632, 0.921837935858},{1, -1.92733026321, 0.933743480607},{1, -1.94325704798, 0.955622928744},{1, -1.96756848519, 0.984286535147}};
float pass05__g = 1.18614571697e-09;
int pass05__num_stages = 4;