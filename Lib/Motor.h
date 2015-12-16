/**
 * @file
 *
 * @brief Class for controlling hobby ESCs
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 11, 2015
 *
 * This class abstracts the low-level PWM interface used by hobby electronic
 * speed controllers (ESCs). It allows for simple initialization by specifying
 * a compatible GPIO pin to the constructor.
 *
 * The Motor::setSpeed function takes care of calculating the necessary pulse
 * width and calls routines in the PwmTimer class for processor-specific
 * operations.
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup Motor
 *  @{
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "PwmTimer.h"
#include "config.h"

/**
 * Defines for the throttle range, specified in ms.
 *
 * NOTE: ESC must be re-calibrated each time these are changed to allow full
 * throttle range.
 */
#define FULL_SPEED_WIDTH 2.0f
#define ZERO_SPEED_WIDTH 1.0f

/**
 * @brief Electronic Speed Controller (ESC) Interface
 *
 * Hobby ESCs take a PWM input with a frequency of 50 Hz. 1 ms positive pulse
 * width corresponds to zero throttle, and 2 ms positive pulse width corresponds
 * to full throttle.
 *
 * This class provides a simple interface for setting the speed of a motor. It
 * allows for simple initialization by specifying a compatible GPIO pin to the
 * constructor. The Motor::setSpeed() function takes care of calculating the
 * necessary pulse width and calls routines in the PwmTimer class for processor-
 * specific operations.
 */
class Motor {
private:
	PwmTimer *pwm;	///< PwmTimer object to handle hardware-specifics
	float speed;	///< Speed/throttle of the motor. 0.0 <= speed <= 1.0

	float mapSpeedToPulseW(float s);

public:

	Motor();
	Motor(TimerPin p);

	void setSpeed(float s);

	float getSpeed(void);
};

#endif /* MOTOR_H_ */

/** @} Close Motor group */
/** @} Close Peripherals Group */
