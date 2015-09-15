/*
 * Motor.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: Jeremiah
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

	if (s <= 0.0) speed = 0.0;
	else if (s >= 1.0) speed = 1.0;
	else speed = s;

	w = mapSpeedToPulseW(speed);
	pwm->setWidth(w);
}

float Motor::getSpeed(void) {
	return 0.0;
}

float Motor::mapSpeedToPulseW(float s) {
	return (FULL_SPEED_WIDTH - ZERO_SPEED_WIDTH) * s + ZERO_SPEED_WIDTH;
}
