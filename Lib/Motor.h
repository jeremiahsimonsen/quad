/**
 * @file Motor.h
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

#ifndef MOTOR_H_
#define MOTOR_H_

#include "PwmTimer.h"

/**
 * Defines for the throttle range, specified in ms.
 *
 * NOTE: ESC must be re-calibrated each time these are changed to allow full
 * throttle range.
 */
#define FULL_SPEED_WIDTH 2.5f
#define ZERO_SPEED_WIDTH 0.5f

class Motor {
private:
	PwmTimer *pwm;	///< PwmTimer object to handle hardware-specifics
	float speed;	///< Speed/throttle of the motor. 0.0 <= speed <= 1.0

	/**
	 * Helper function to convert a speed level (0.0 to 1.0) to the required
	 * PWM positive pulse width.
	 * @param s Speed/throttle level from 0.0 to 1.0
	 * @return PWM positive pulse width corresponding to the speed in ms
	 */
	float mapSpeedToPulseW(float s);

public:
	/**
	 * Default constructor. Initializes a motor on the default PwmTimer pin
	 */
	Motor();
	/**
	 * Initializes a motor on the specified pin, p
	 * @param p The pin to initialize for the motor control
	 */
	Motor(TimerPin p);

	/**
	 * Sets the speed of the ESC/motor to the specified throttle level, s
	 * @param s Speed/throttle level from 0.0 to 1.0
	 */
	void setSpeed(float s);

	/**
	 * Retrieves the speed of the motor. Only relevant for Group B's ESC
	 *
	 * @todo Implement this function
	 * @return The speed of the motor in rpm
	 */
	float getSpeed(void);
};

#endif /* MOTOR_H_ */
