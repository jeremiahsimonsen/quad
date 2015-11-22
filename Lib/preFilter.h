/**
 * @file preFilter.h
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
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

#ifndef PREFILTER_H_
#define PREFILTER_H_

#define PREFILTER_TAU (10.0f)

class preFilter {
private:
	float tau;
	float x_prev;
	float y_prev;

public:
	preFilter();
	preFilter(float tau);

	float filterSample(float x);
};

#endif
