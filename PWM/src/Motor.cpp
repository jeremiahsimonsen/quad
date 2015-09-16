/**
 * @file Motor.cpp
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

#include "Motor.h"
#include "PwmTimer.h"

Motor::Motor() {
	float w;

	pwm = new PwmTimer();

	setSpeed(0.0);
}

Motor::Motor(TimerPin p) {
	float w;

	pwm = new PwmTimer(50.0, p);

	setSpeed(0.0);
}

void Motor::setSpeed(float s) {
	float w;

	// Only allow speeds between 0.0 and 1.0
	if (s < 0.0) speed = 0.0;
	else if (s > 1.0) speed = 1.0;
	else speed = s;

	// Convert speed to pulse width and configure TIM for new width
	w = mapSpeedToPulseW(speed);
	pwm->setWidth(w);
}

float Motor::getSpeed(void) {
	return 0.0;
}

float Motor::mapSpeedToPulseW(float s) {
	return (FULL_SPEED_WIDTH - ZERO_SPEED_WIDTH) * s + ZERO_SPEED_WIDTH;
}
