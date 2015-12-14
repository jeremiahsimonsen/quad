/**
 * @file
 *
 * @brief Class for low-pass filtering pre-filtered gyro data
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

#ifndef GYROCOMPFILTER2_H_
#define GYROCOMPFILTER2_H_

/**
 * @brief 2nd order complementary low-pass filter for the gyroscope
 *
 * This class implements a complementary low-pass filter designed for the
 * gyroscope angular velocity. It has the transfer function:
 * 		\f[
 * 			H_g(s) = \frac{\tau^2}{(\tau s + 1)^2}
 * 		\f]
 * corresponding z-transform:
 * 		\f[
 * 			H_g(z) = \frac{\tau^2(z-1)(z+1)}{\tau^2(z^2-2z+1) + 2\tau(z^2-1) + (z^2+2z+1)}
 * 		\f]
 * which has difference equation:
 * 		\f[
 * 			y_g(n) = \frac{\tau^2x_g(n) - \tau^2x_g(n-2) - (-2\tau^2+2)y_g(n-1) - (\tau^2-2\tau+1)y_g(n-2)}
 * 				{\tau^2 + 2\tau + 1}
 * 		\f]
 *
 * Each sample is processed one at a time.
 */
class gyroCompFilter2 {
private:
	float tau;		///< Complementary filter time constant
	float x1;		///< Previous input value, \f$ x_g(n-1) \f$
	float x2;		///< 2nd previous input value, \f$ x_g(n-2) \f$
	float y1;		///< Previous output value, \f$ y_g(n-1) \f$
	float y2;		///< 2nd previous input value, \f$ y_g(n-2) \f$

public:
	gyroCompFilter2(float tau);

	float filterSample(float x);
};

#endif

/** @} Close COMPFILTER group */
/** @} Close Control Group */
