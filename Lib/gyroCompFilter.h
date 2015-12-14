/**
 * @file
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 22, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup COMPFILTER
 *  @{
 */

#ifndef GYROCOMPFILTER_H_
#define GYROCOMPFILTER_H_

/**
 * @brief 1st order complementary low-pass filter for the gyroscope
 *
 * This class implements a complementary low-pass filter designed for the
 * gyroscope angular velocity. It has the transfer function:
 * 		\f[
 * 			H_g(s) = \frac{\tau}{\tau s + 1}
 * 		\f]
 * corresponding z-transform:
 * 		\f[
 * 			H_g(z) = \frac{\tau + \tau z^{-1}}{(\tau + 1) + (1-\tau)z^{-1}}
 * 		\f]
 * which has difference equation:
 * 		\f[
 * 			y_g(n) = \frac{x_g(n)\tau + x_g(n-1)\tau - y_g(n-1)(1-\tau)}{\tau+1}
 * 		\f]
 *
 * Each sample is processed one at a time.
 */
class gyroCompFilter {
private:
	float tau;			///< Complementary filter time constant
	float x_prev;		///< Previous input value, \f$ x_g(n-1) \f$
	float y_prev;		///< Previous output value, \f$ y_g(n-1) \f$

public:
	gyroCompFilter();
	gyroCompFilter(float tau);

	float filterSample(float x);
};

#endif

/** @} Close COMPFILTER group */
/** @} Close Control Group */
