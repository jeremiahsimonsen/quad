/**
 * @file
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
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

/** @addtogroup PREFILTER
 *  @{
 */

#ifndef PREFILTER_H_
#define PREFILTER_H_

// Pre-filter time constant
#define PREFILTER_TAU (150.0f)

/**
 * @brief 1st order low-pass filter
 *
 * This class implements a low-pass filter of the transfer function:
 * 		\f[ \frac{1}{\tau s + 1} \f]
 * corresponding z-transform:
 * 		\f[ H(z) = \frac{1 + z^{-1}}{(\tau + 1) + (1 - \tau)z^{-1}} \f]
 * which has difference equation:
 * 		\f[ y(n) = \frac{x(n) + x(n-1) - y(n-1)(1-\tau)}{\tau + 1} \f]
 *
 * Each sample is processed one at a time.
 */
class preFilter {
private:
	float tau;		///< Filter time-constant
	float x_prev;	///< Previous input value, \f$ x(n-1) \f$
	float y_prev;	///< Previous output value, \f$ y(n-1) \f$

public:
	preFilter();
	preFilter(float tau);

	float filterSample(float *x);
};

#endif

/** @} Close PREFILTER group */
/** @} Close Control Group */
