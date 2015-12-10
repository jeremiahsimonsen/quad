/**
 * @file pid2.h
 *
 * @brief Class for PID feedback control
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 8, 2015
 *
 * This class implements a feedback controller with transfer function:
 *		G(s) = Kp + Ki/s + Kd*s
 * Or z-transform:
 * 		G(z) = (K1 - K2*z^-1 + K3*z^-2) / (1-z^-1)
 * and difference equation:
 * 		u(n) = u(n-1) + K1*e(n) - K2*e(n-1) + K3*e(n-2)
 *
 */

#ifndef PID2_H_
#define PID2_H_

#define INTEGRAL_SATURATION 5.0f
#define ERROR_DEADBAND 1.0f

class pid2 {
private:
	float kp, ki, kd;
	float integral;
	float e1;

public:
	pid2(float kp, float ki, float kd);

	float calculate(float e, float dt);
};

#endif
