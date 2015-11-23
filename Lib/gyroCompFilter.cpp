/**
 * @file gyroCompFilter.cpp
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 * This class implements a low-pass filter of the transfer function:
 * 		H(z) = (tau + tau*z^(-1)) / ((tau+1) + (1-tau)z^(-1))
 * which has difference equation:
 * 		y(n) = ( (x(n)tau + x(n-1)tau - y(n-1)(1-tau) ) / (tau + 1)
 *
 * One single sample is processed at a time
 *
 */

#include "gyroCompFilter.h"

gyroCompFilter::gyroCompFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

gyroCompFilter::gyroCompFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

float gyroCompFilter::filterSample(float x) {
	float y = ( x*tau + x_prev*tau - y_prev*(1-tau) ) / (tau+1);
	x_prev = x;
	y_prev = y;

	return y;
}




