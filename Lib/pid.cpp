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

/** @defgroup PID PID Control
 *  @brief Proportional-Integral-Derivative (PID) Feedback Control
 *
 *  A quadcopter is a dynamic system. In order to fly, it is necessary to
 *  maintain a fixed orientation. Feedback control is used to reject outside
 *  disturbances and track a desired input.
 *
 *  @{
 */

#include "pid.h"

#if TAKE_PID
/**
 * @brief Construct a pid object with the given gains
 * @param kp Proportional gain
 * @param ki Integral gain
 * @param kd Derivative gain
 */
pid::pid(float kp, float ki, float kd) {
	// Initialize members
	k1 = kp + ki + kd;
	k2 = -kp - 2*kd;
	k3 = kd;
	u1 = e1 = e2 = 0.0f;
}
#else
/**
 * @brief Construct a pid object with the given gains
 * @param k1 The 1st gain constant
 * @param k2 The 2nd gain constant
 * @param k3 The 3rd gain constant
 */
pid::pid(float k1, float k2, float k3) {
	// Initialize members
	this->k1 = k1;
	this->k2 = k2;
	this->k3 = k3;
	u1 = e1 = e2 = 0.0f;
}
#endif

/**
 * @brief Calculate the controller output
 * @param e The current error
 * @return The controller output \f$ u(n)\f$
 */
float pid::calculate(float e) {
	float u = u1 + k1*e + k2*e1 + k3*e2;

	u1 = u;
	e2 = e1;
	e1 = e;

	return u;
}

/** @} Close PID group */
/** @} Close Control Group */
