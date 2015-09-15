/*
 * Motor.h
 *
 *  Created on: Sep 11, 2015
 *      Author: Jeremiah
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "PwmTimer.h"

#define FULL_SPEED_WIDTH 2.5f
#define ZERO_SPEED_WIDTH 0.5f

class Motor {
private:
	PwmTimer *pwm;
	float speed;

	float mapSpeedToPulseW(float s);

public:
	Motor();
	Motor(TimerPin p);

	void setSpeed(float s);
	float getSpeed(void);
};

#endif /* MOTOR_H_ */
