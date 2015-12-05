/**
 * @file accelCompFilter2.cpp
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 4, 2015
 *
 * This class implements a 2nd order low-pass filter of the transfer function:
 * 		H(s) = (2*tau*s + 1) / (tau*s + 1)^2
 * which has difference equation:
 * 		y(n) = [(2*tau + 1)*x(n) + 2*x(n-1) + (-2*tau + 1)*x(n-2) - (-2*tau^2 + 2)*y(n-1) - (tau^2-2*tau+1)*y(n-2)] /
 * 							(tau^2 + 2*tau + 1)
 *
 * One single sample is processed at a time
 *
 */

#include "accelCompFilter2.h"

accelCompFilter2::accelCompFilter2(float t) {
	x1 = x2 = 0.0f;
	y1 = y2 = 0.0f;
	tau = t;
}

float accelCompFilter2::filterSample(float x) {
	float y = ( (2*tau+1)*x + 2*x1 + (-2*tau+1)*x2 - (-2*tau*tau+2)*y1 - (tau*tau-2*tau+1)*y2 ) / (tau*tau + 2*tau + 1);

	x2 = x1;
	x1 = x;
	y2 = y1;
	y1 = y;

	return y;
}




