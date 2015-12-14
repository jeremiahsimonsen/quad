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

#ifndef PID_H_
#define PID_H_

// Option to take Kp, Ki, Kd or K1, K2, K3
#define TAKE_PID 1

/**
 * @brief PID control
 *
 * This class implements PID control based on a z-transform. The continuous-time
 * transfer function of a PID controller is
 * 		\f[ G(s) = K_p + \frac{K_i}{s} + K_ds\f]
 * Which has a z-transform of
 * 		\f[ G(z) = \frac{K_1 - K_2z^{-1} + K_3z^{-2}}{1-z^{-1}}\f]
 * and difference equation:
 * 		\f[ u(n) = u(n-1) + K_1e(n) - K_2e(n-1) + K_3e(n-2)\f]
 */
class pid {
private:
	float k1;		///< The 1st gain constant. \f$ K_1 = K_p + K_i + K_d\f$
	float k2;		///< The 2nd gain constant. \f$ K_2 = -K_p + 2K_d\f$
	float k3;		///< The 3rd gain constant. \f$ K_3 = K_d\f$
	float u1;		///< The previous output \f$ u(n-1)\f$
	float e1;		///< The previous error \f$ e(n-1)\f$
	float e2;		///< The 2nd previous error \f$ e(n-2)\f$

public:
	pid(float kp, float ki, float kd);

	float calculate(float e);
};

#endif

/** @} Close PID group */
/** @} Close Control Group */
