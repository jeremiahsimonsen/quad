/**
 * @file preFilter.cpp
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 * This class implements a low-pass filter of the transfer function:
 * 		H(z) = (1+z^(-1)) / ((tau+1)+(1-tau)z^(-1))
 * which has difference equation:
 * 		y(n) = (x(n) + x(n-1) - y(n-1)(1-tau)) / (tau+1)
 *
 * One single sample is processed at a time
 *
 */

#include "preFilter.h"

preFilter::preFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

preFilter::preFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

float preFilter::filterSample(float *x) {
	float xn = *x;
	float y = ( xn + x_prev - y_prev*(1-tau) ) / (tau + 1);
	x_prev = xn;
	y_prev = y;

	return y;
}




