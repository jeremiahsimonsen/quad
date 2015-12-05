/**
 * @file gyroCompFilter2.h
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 4, 2015
 *
 * This class implements a 2nd order low-pass filter of the transfer function:
 * 		H(s) = (tau^2*s) / (tau*s + 1)^2
 * which has difference equation:
 * 		y(n) = [tau^2*x(n) - tau^2*x(n-2) - (-2*tau+2)*y(n-1) - (tau^2-2*tau+1)*y(n-2)] / (tau^2+2*tau+1)
 *
 * One single sample is processed at a time
 *
 */

#ifndef GYROCOMPFILTER2_H_
#define GYROCOMPFILTER2_H_

class gyroCompFilter2 {
private:
	float tau;
	float x1, x2;
	float y1, y2;

public:
	gyroCompFilter2(float tau);

	float filterSample(float x);
};

#endif
