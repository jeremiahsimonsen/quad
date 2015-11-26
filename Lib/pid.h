/**
 * @file pid.h
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

#ifndef PID_H_
#define PID_H_

class pid {
private:
	float k1, k2, k3;
	float u1;
	float e1, e2;

public:
	pid(float kp, float ki, float kd);

	float calculate(float e);
};

#endif
