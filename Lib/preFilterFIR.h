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

#ifndef PREFILTERFIR_H_
#define PREFILTERFIR_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

/**
 * @brief Arbitrary FIR filter
 *
 * This class implements an arbitrary FIR filter. The filter coefficients are
 * defined in the constructor. The ARM CMSIS DSP filtering routines are used
 * for maximum performance.
 *
 * Each sample is processed one at a time.
 */
class preFilterFIR {
private:
	arm_fir_instance_f32 f;		///< ARM FIR filter structure
	float32_t *state;			///< State buffer used by ARM routine

public:
	preFilterFIR();

	float32_t filterSample(float32_t *x);
};

#endif

/** @} Close PREFILTER group */
/** @} Close Control Group */
