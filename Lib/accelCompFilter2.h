/**
 * @file accelCompFilter2.h
 *
 * @brief Class for low-pass filtering angles calculated using just the accelerometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 4, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup COMPFILTER
 *  @{
 */

#ifndef ACCELCOMPFILTER2_H_
#define ACCELCOMPFILTER2_H_

/**
 * @brief 2nd order complementary low-pass filter for accelerometer
 *
 * This class implements a complementary low-pass filter designed for the
 * angle calculated using the accelerometer. It has the transfer function:
 * 		\f[
 * 			H_a(s) = \frac{2\tau s + 1}{(\tau s + 1)^2}
 * 		\f]
 * corresponding z-transform:
 * 		\f[
 * 			H_a(z) = \frac{(2\tau + 1) + 2z^{-1} + (-2\tau + 1)z^{-2}}
 * 				{(\tau^2 + 2\tau + 1) + (-2\tau^2 + 2)z^{-1} + (\tau^2 - 2\tau + 1)z^{-2}}
 * 		\f]
 * which has difference equation:
 * 		\f[
 * 			y_a(n) = \frac{(2\tau+1)x_a(n) + 2x_a(n-1) + (-2\tau+1)x_a(n-2) - (-2\tau^2 + 2)y_a(n-1) - (\tau^2-2\tau+1)y_a(n-2)}
 * 				{\tau^2 + 2\tau + 1}
 * 		\f]
 *
 * Each sample is processed one at a time.
 */
class accelCompFilter2 {
private:
	float tau;		///< Complementary filter time constant
	float x1;		///< Previous input value, \f$ x_a(n-1) \f$
	float x2;		///< 2nd previous input value, \f$ x_a(n-2) \f$
	float y1;		///< Previous output value, \f$ y_a(n-1) \f$
	float y2;		///< 2nd previous input value, \f$ y_a(n-2) \f$

public:
	accelCompFilter2(float tau);

	float filterSample(float x);
};

#endif

/** @} Close COMPFILTER group */
/** @} Close Control Group */
