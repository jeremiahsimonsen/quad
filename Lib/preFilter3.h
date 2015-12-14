/**
 * @file
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 6, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PREFILTER
 *  @{
 */

#ifndef PREFILTER3_H_
#define PREFILTER3_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

/**
 * @brief Arbitrary IIR filter
 *
 * This class implements an arbitrary IIR filter. The filter coefficients are
 * defined in the constructor. The ARM CMSIS DSP filtering routines are used
 * for maximum performance.
 *
 * Each sample is processed one at a time.
 */
class preFilter3 {
private:
	arm_biquad_cascade_df2T_instance_f32 f;		///< ARM IIR Direct-Form II Transpose filter structure
	float32_t *state;							///< State buffer used by ARM routine
	float32_t g;								///< Filter gain factor

public:
	preFilter3();

	float32_t filterSample(float32_t *x);
};

#endif

/** @} Close PREFILTER group */
/** @} Close Control Group */
