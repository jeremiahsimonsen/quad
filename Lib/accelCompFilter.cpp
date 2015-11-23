/**
 * @file accelCompFilter.cpp
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
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

#include "accelCompFilter.h"

accelCompFilter::accelCompFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

accelCompFilter::accelCompFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

float accelCompFilter::filterSample(float x) {
	float y = ( x + x_prev - y_prev*(1-tau) ) / (tau + 1);
	x_prev = x;
	y_prev = y;

	return y;
}




