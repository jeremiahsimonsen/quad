/**
 * @file
 *
 * @brief Class for PID feedback control
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 24, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PID
 *  @{
 */

#include "pid2.h"

/**
 * @brief Construct a pid2 object with the given gains
 * @param p Proportional gain
 * @param i Integral gain
 * @param d Derivative gain
 */
pid2::pid2(float p, float i, float d) {
	// Initialize members
	kp = p;
	ki = i;
	kd = d;
	integral = 0.0f;
	e1 = 0.0f;
}

/**
 * @brief Calculate the controller output
 * @param e  The current error
 * @param dt The sample time [s]
 * @return The controller output, \f$ u(n) \f$
 */
float pid2::calculate(float e, float dt) {
	/* Rejecting error since it doesn't matter too much if the angle is off by
	 * a small margin. This should help reject sensor noise.
	 */
	if (e > -ERROR_DEADBAND && e < ERROR_DEADBAND) {
		e = 0.0f;
	}

	/* Calculate the error integral. Don't allow it to grow too large so it
	 * doesn't dominate the controller output
	 */
	integral += e * dt;
	if (integral > INTEGRAL_SATURATION) integral = INTEGRAL_SATURATION;
	if (integral < -INTEGRAL_SATURATION) integral = -INTEGRAL_SATURATION;

	// Calculate the derivative of the error
	float derivative = (e - e1) / dt;

	// Calculate the controller output
	float u = kp*e + ki*integral + kd*derivative;

	// Store the error
	e1 = e;

	return u;
}

/** @} Close PID group */
/** @} Close Control Group */
