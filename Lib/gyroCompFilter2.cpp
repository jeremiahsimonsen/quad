/**
 * @file
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
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

#include "gyroCompFilter2.h"

/**
 * @brief Construct a gyroCompFilter2 object with given time constant
 * @param t Time constant
 */
gyroCompFilter2::gyroCompFilter2(float t) {
	x1 = x2 = 0.0f;
	y1 = y2 = 0.0f;
	tau = t;
}

/**
 * @brief Calculate the filter output
 * @param x The current sample input
 * @return The corresponding filter output
 */
float gyroCompFilter2::filterSample(float x) {
	float y = ( tau*tau*x - tau*tau*x2 - (-2*tau*tau+2)*y1 - (tau*tau-2*tau+1)*y2 ) / (tau*tau+2*tau+1);

	x2 = x1;
	x1 = x;
	y2 = y1;
	y1 = y;

	return y;
}

/** @} Close COMPFILTER group */
/** @} Close Control Group */


