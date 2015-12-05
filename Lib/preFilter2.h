/**
 * @file preFilter2.h
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 4, 2015
 *
 * This class implements a 2nd order low-pass filter of the transfer function:
 * 		H(s) = (2*tau*s + 1) / (tau*s + 1)^2
 * which has difference equation:
 * 		y(n) = [(2*tau + 1)*x(n) + 2*x(n-1) + (-2*tau + 1)*x(n-2) - (-2*tau^2 + 2)*y(n-1) - (tau^2-2*tau+1)*y(n-2)] /
 * 							(tau^2 + 2*tau + 1)
 *
 * One single sample is processed at a time
 *
 */

#ifndef PREFILTER2_H_
#define PREFILTER2_H_

//#define PREFILTER_TAU (500.0f)	// Defined in preFilter.h

class preFilter2 {
private:
	float tau;
	float x1, x2;
	float y1, y2;

public:
	preFilter2(float tau);

	float filterSample(float x);
};

#endif
