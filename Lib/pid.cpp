/**
 * @file pid.cpp
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

#include "pid.h"

#if TAKE_PID
pid::pid(float kp, float ki, float kd) {
	k1 = kp + ki + kd;
	k2 = -kp - 2*kd;
	k3 = kd;
	u1 = e1 = e2 = 0.0f;
}
#else
pid::pid(float k1, float k2, float k3) {
	this->k1 = k1;
	this->k2 = k2;
	this->k3 = k3;
	u1 = e1 = e2 = 0.0f;
}
#endif

float pid::calculate(float e) {
	float u = u1 + k1*e + k2*e1 + k3*e2;

	u1 = u;
	e2 = e1;
	e1 = e;

	return u;
}


