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

#include "pass5_stop25.h"

float pass5_s_b_coef[2][3] = {{1, -1.8256, 1},{1, -1.9673, 1}};
float pass5_s_a_coef[2][3] = {{1, -1.94551, 0.946985},{1, -1.97569, 0.979843}};
float pass5_s_g = 0.00101447;
int pass5_s_num_stages = 2;