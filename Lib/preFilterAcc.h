/**
 * @file preFilterAcc.h
 *
 * @brief Class for low-pass filtering raw accelerometer data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

#ifndef PREFILTERACC_H_
#define PREFILTERACC_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

//#define PREFILTER_TAU (500.0f)	// Defined in preFilter.h

class preFilterAcc {
private:
	arm_biquad_cascade_df2T_instance_f32 f;
	float32_t *state;
	float32_t g;

public:
	preFilterAcc();

	float32_t filterSample(float32_t *x);
};

#endif
