/**
 * @file
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

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PREFILTER
 *  @{
 */

#ifndef PREFILTER2_H_
#define PREFILTER2_H_

/**
 * @brief 2nd order low-pass filter
 *
 * This class implements a low-pass filter of the transfer function:
 * 		\f[ \frac{2\tau s + 1}{(\tau s + 1)^2} \f]
 * corresponding z-transform:
 * 		\f[
 * 			H(z) = \frac{(2\tau + 1) + 2z^{-1} + (-2\tau + 1)z^{-2}}
 * 				{(\tau^2 + 2\tau + 1) + (-2\tau^2 + 2)z^{-1} + (\tau^2 - 2\tau + 1)z^{-2}}
 * 		\f]
 * which has difference equation:
 * 		\f[
 * 			y(n) = \frac{(2\tau + 1)x(n) + 2x(n-1) + (-2\tau + 1)x(n-2) - (-2\tau^2 + 2)y(n-1) - (\tau^2-2\tau+1)y(n-2)}
 * 				{\tau^2 + 2\tau + 1}
 * 		\f]
 *
 * Each sample is processed one at a time.
 */
class preFilter2 {
private:
	float tau;		///< Filter time constant
	float x1;		///< Previous input value, \f$ x(n-1) \f$
	float x2;		///< 2nd previous input value, \f$ x(n-2) \f$
	float y1;		///< Previous output value, \f$ y(n-1) \f$
	float y2;		///< 2nd previous output value, \f$ y(n-2) \f$

public:
	preFilter2(float tau);

	float filterSample(float *x);
};

#endif

/** @} Close PREFILTER group */
/** @} Close Control Group */
