/**
 * @file
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
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

/** @addtogroup COMPFILTER
 *  @{
 */

#include "gyroCompFilter.h"

/**
 * @brief Construct a gyroCompFilter object with the default time constant
 */
gyroCompFilter::gyroCompFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

/**
 * @brief Construct a gyroCompFilter object with the given time constant
 * @param t Time constant
 */
gyroCompFilter::gyroCompFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

/**
 * @brief  Calculate the filter output
 * @param  x The current sample input
 * @return The corresponding filter output
 */
float gyroCompFilter::filterSample(float x) {
	float y = ( x*tau + x_prev*tau - y_prev*(1-tau) ) / (tau+1);
	x_prev = x;
	y_prev = y;

	return y;
}

/** @} Close COMPFILTER group */
/** @} Close Control Group */


