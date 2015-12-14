/**
 * @file
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @defgroup PREFILTER Filters for raw sensor data
 *  @brief Low-pass filtering of sensor data
 *
 *  The motors and propellers of a quadcopter spin at tremendous speeds.
 *  This introduces a significant amount of mechanical vibration, causing the
 *  accelerometer and gyroscope sensor output to be noisy. To calculate the
 *  pitch and roll angles reliably, the sensor outputs must be low-pass
 *  filtered prior to being used in the orientation calculation.
 *
 *  @{
 */

#include "preFilter.h"

/**
 * @brief Construct a preFilter object with default time constant
 */
preFilter::preFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

/**
 * @brief Construct a preFilter object with the given time constant
 * @param t Time constant
 */
preFilter::preFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

/**
 * @brief  Calculate the filter output
 * @param  x The current sample input
 * @return The corresponding filter output
 *
 * Calculates the filtered value, according to the transfer function.
 */
float preFilter::filterSample(float *x) {
	float xn = *x;
	float y = ( xn + x_prev - y_prev*(1-tau) ) / (tau + 1);
	x_prev = xn;
	y_prev = y;

	return y;
}

/** @} Close PREFILTER group */
/** @} Close Control Group */


