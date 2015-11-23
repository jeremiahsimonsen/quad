/**
 * @file gyroCompFilter.h
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 * This class implements a low-pass filter of the transfer function:
 * 		H(z) = (tau + tau*z^(-1)) / ((tau+1) + (1-tau)z^(-1))
 * which has difference equation:
 * 		y(n) = ( (x(n)tau + x(n-1)tau - y(n-1)(1-tau) ) / (tau + 1)
 *
 * One single sample is processed at a time
 *
 */

#ifndef GYROCOMPFILTER_H_
#define GYROCOMPFILTER_H_

class gyroCompFilter {
private:
	float tau;
	float x_prev;
	float y_prev;

public:
	gyroCompFilter();
	gyroCompFilter(float tau);

	float filterSample(float x);
};

#endif
