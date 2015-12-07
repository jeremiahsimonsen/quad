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

#include "pass1_stop5v2.h"

float pass1_s_b_coef[3][3] = {{1, 1, 0},{1, -1.99833, 1},{1, -1.99934, 1}};
float pass1_s_a_coef[3][3] = {{1, -0.995626, 0},{1, -1.99317, 0.993208},{1, -1.99749, 0.997558}};
float pass1_s_g = 5.9143e-06;
int pass1_s_num_stages = 3;