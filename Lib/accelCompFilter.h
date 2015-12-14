/**
 * @file
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
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

#ifndef ACCELCOMPFILTER_H_
#define ACCELCOMPFILTER_H_

/**
 * @brief 1st order complementary low-pass filter for accelerometer
 *
 * This class implements a complementary low-pass filter designed for the
 * angle calculated using the accelerometer. It has the transfer function:
 * 		\f[
 * 			H_a(s) = \frac{1}{\tau s + 1}
 * 		\f]
 * corresponding z-transform:
 * 		\f[
 * 			H_a(z) = \frac{1 + z^{-1}}{(\tau + 1) + (1 - \tau)z^{-1}}
 * 		\f]
 * which has difference equation:
 * 		\f[
 * 			y_a(n) = \frac{x_a(n) + x_a(n-1)\tau - y_a(n-1)(1 - \tau)}{\tau + 1}
 * 		\f]
 *
 * Each sample is processed one at a time.
 */
class accelCompFilter {
private:
	float tau;			///< Complementary filter time constant
	float x_prev;		///< Previous input value, \f$ x_a(n-1) \f$
	float y_prev;		///< Previous output value, \f$ y_a(n-1) \f$

public:
	accelCompFilter();
	accelCompFilter(float tau);

	float filterSample(float x);
};

#endif

/** @} Close COMPFILTER group */
/** @} Close Control Group */
