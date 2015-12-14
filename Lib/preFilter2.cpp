/**
 * @file
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
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

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PREFILTER
 *  @{
 */

#include "preFilter2.h"

/**
 * @brief Construct a preFilter2 object with the given time constant
 * @param t Time constant
 */
preFilter2::preFilter2(float t) {
	x1 = x2 = 0.0f;
	y1 = y2 = 0.0f;
	tau = t;
}

/**
 * @brief Calculate the filter output
 * @param x The current sample input
 * @return  The corresponding filter output
 *
 * Calculates the filtered value, according to the transfer function.
 */
float preFilter2::filterSample(float *x) {
	float xn = *x;
	float y = ( (2*tau+1)*xn + 2*x1 + (-2*tau+1)*x2 - (-2*tau*tau+2)*y1 - (tau*tau-2*tau+1)*y2 ) / (tau*tau + 2*tau + 1);

	x2 = x1;
	x1 = xn;
	y2 = y1;
	y1 = y;

	return y;
}

/** @} Close PREFILTER group */
/** @} Close Control Group */


