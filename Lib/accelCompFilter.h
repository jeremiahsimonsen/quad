/**
 * @file accelCompFilter.h
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 * This class implements a low-pass filter of the transfer function:
 * 		H(z) = (1+z^(-1)) / ((tau+1)+(1-tau)z^(-1))
 * which has difference equation:
 * 		y(n) = (x(n) + x(n-1) - y(n-1)(1-tau)) / (tau+1)
 *
 * One single sample is processed at a time
 *
 */

#ifndef ACCELCOMPFILTER_H_
#define ACCELCOMPFILTER_H_

class accelCompFilter {
private:
	float tau;
	float x_prev;
	float y_prev;

public:
	accelCompFilter();
	accelCompFilter(float tau);

	float filterSample(float x);
};

#endif
