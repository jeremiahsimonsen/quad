/**
 * @file
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 */

/** @defgroup Control Signal Processing and Feedback Control
 *  @brief Sensor pre- and complementary filtering. Feedback control.
 *  @{
 */

/** @defgroup COMPFILTER Complementary filters for sensor fusion
 *  @brief Complementary filtering of pre-filtered data for sensor fusion
 *
 *  Pre-filtering the raw accelerometer and gyroscope sensor outputs helps
 *  reduce the noise. However, accelerometers are poorly-suited to measuring
 *  rapidly changing angles. There is no way to distinguish between tilt and
 *  lateral acceleration, since using an accelerometer to calculate an angle
 *  relies on the gravitational acceleration vector. Instead, it can measure
 *  a long-term average orientation, filtering out the high frequency changes
 *  in orientation.
 *
 *  On the other hand, gyroscopes have no way to measure a static angle, since
 *  they measure a rate of change in an angle. If the quadcopter is completely
 *  still, the gyroscope should measure all zero rotation, which contains no
 *  information about the orientation. As such, the gyroscope reading only
 *  contains high frequency information.
 *
 *  To add to the confusion, the gyroscope measures a derivative. This means
 *  that if the gyro output [deg/s] is low-pass filtered, we are essentially
 *  high-pass filtering the angle measurement [deg]. This is how low-pass
 *  filtering both the accelerometer based angle and the gyroscope angular
 *  velocity results in a complementary filter structure.
 *
 *  @{
 */

#include "accelCompFilter.h"

/**
 * @brief Construct an accelCompFilter object with default time constant
 */
accelCompFilter::accelCompFilter() {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = 1.0f;
}

/**
 * @brief Construct an accelCompFilter object with the given time constant
 * @param t Time constant
 */
accelCompFilter::accelCompFilter(float t) {
	x_prev = 0.0f;
	y_prev = 0.0f;
	tau = t;
}

/**
 * @brief  Calculate the filter output
 * @param  x The current sample input
 * @return The corresponding filter output
 */
float accelCompFilter::filterSample(float x) {
	float y = ( x + x_prev - y_prev*(1-tau) ) / (tau + 1);
	x_prev = x;
	y_prev = y;

	return y;
}

/** @} Close PREFILTER group */
/** @} Close Control Group */


