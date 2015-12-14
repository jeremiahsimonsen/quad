/**
 * @file
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 4, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup COMPFILTER
 *  @{
 */

#include "accelCompFilter2.h"

/**
 * @brief Construct an accelCompFilter2 object with given time constant
 * @param t Time constant
 */
accelCompFilter2::accelCompFilter2(float t) {
	x1 = x2 = 0.0f;
	y1 = y2 = 0.0f;
	tau = t;
}

/**
 * @brief Calculate the filter output
 * @param x The current sample input
 * @return The corresponding filter output
 */
float accelCompFilter2::filterSample(float x) {
	float y = ( (2*tau+1)*x + 2*x1 + (-2*tau+1)*x2 - (-2*tau*tau+2)*y1 - (tau*tau-2*tau+1)*y2 ) / (tau*tau + 2*tau + 1);

	x2 = x1;
	x1 = x;
	y2 = y1;
	y1 = y;

	return y;
}

/** @} Close COMPFILTER group */
/** @} Close Control Group */


