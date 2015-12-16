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

/** @defgroup Motor ESC (Electronic Speed Controller) Interface
 *  @brief Abstraction building on PwmTimer to interface with hobby ESCs
 *
 *  Hobby ESCs take a PWM input with a frequency of 50 Hz. 1 ms positive pulse
 *  width corresponds to zero throttle, and 2 ms positive pulse width corresponds
 *  to full throttle. This class provides a simple interface for setting the
 *  speed of a motor
 *
 *  @{
 */

#include "Motor.h"
#include "PwmTimer.h"

/**
 * @brief Default constructor. Initializes a motor on the default PwmTimer pin
 */
Motor::Motor() {
	pwm = new PwmTimer();

	setSpeed(0.0);
}

/**
 * @brief Initializes a motor on the specified pin, p
 * @param p The pin to initialize for the motor control
 */
Motor::Motor(TimerPin p) {
	pwm = new PwmTimer(50.0, p);

	setSpeed(0.0);
}

/**
 * @brief Sets the speed of the ESC/motor to the specified throttle level, s
 * @param s Speed/throttle level from 0.0 to 1.0
 */
void Motor::setSpeed(float s) {
	float w;

	// Only allow speeds between 0.0 and 1.0
	if (s < 0.0) speed = 0.0;
	else if (s > MAX_SPEED) speed = MAX_SPEED;
	else speed = s;

	// Convert speed to pulse width and configure TIM for new width
	w = mapSpeedToPulseW(speed);
	pwm->setWidth(w);
}

/**
 * @brief Retrieves the speed of the motor. Only relevant for Group B's ESC
 *
 * @todo Implement this function
 * @return The speed of the motor in rpm
 */
float Motor::getSpeed(void) {
	return speed;
}

/**
 * @brief Helper function to convert a speed level (0.0 to 1.0) to the required
 * PWM positive pulse width.
 * @param s Speed/throttle level from 0.0 to 1.0
 * @return PWM positive pulse width corresponding to the speed in ms
 */
float Motor::mapSpeedToPulseW(float s) {
	return (FULL_SPEED_WIDTH - ZERO_SPEED_WIDTH) * s + ZERO_SPEED_WIDTH;
}

/** @} Close Motor group */
/** @} Close Peripherals Group */
