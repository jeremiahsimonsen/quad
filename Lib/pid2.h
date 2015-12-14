/**
 * @file
 *
 * @brief Class for PID feedback control
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 8, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PID
 *  @{
 */

#ifndef PID2_H_
#define PID2_H_

#define INTEGRAL_SATURATION 5.0f
#define ERROR_DEADBAND 1.0f

/**
 * @brief Alternate PID implementation
 *
 * This class implements PID control in a more traditional manner. The sample
 * time is measured and the error integral & derivative are numerically
 * calculated.
 */
class pid2 {
private:
	float kp;			///< The proportional gain constant
	float ki;			///< The integral gain constant
	float kd;			///< The derivative gain constant
	float integral;		///< The error integral
	float e1;			///< The previous error for calculating the derivative

public:
	pid2(float kp, float ki, float kd);

	float calculate(float e, float dt);
};

#endif

/** @} Close PID group */
/** @} Close Control Group */
