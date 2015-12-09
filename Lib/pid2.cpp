/**
 * @file pid2.cpp
 *
 * @brief Class for PID feedback control
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 24, 2015
 *
 * This class implements a feedback controller with transfer function:
 *		G(s) = Kp + Ki/s + Kd*s
 * Or z-transform:
 * 		G(z) = (K1 - K2*z^-1 + K3*z^-2) / (1-z^-1)
 * and difference equation:
 * 		u(n) = u(n-1) + K1*e(n) - K2*e(n-1) + K3*e(n-2)
 *
 */

#include "pid2.h"

pid2::pid2(float p, float i, float d) {
	kp = p;
	ki = i;
	kd = d;
	integral = 0.0f;
	e1 = 0.0f;
}

float pid2::calculate(float e, float dt) {
	integral += e * dt;
	if (integral > INTEGRAL_SATURATION) integral = INTEGRAL_SATURATION;
	if (integral < -INTEGRAL_SATURATION) integral = -INTEGRAL_SATURATION;

	float derivative = (e - e1) / dt;

	float u = kp*e + ki*integral + kd*derivative;

	e1 = e;

	return u;
}


