/**
 * @file preFilter3.h
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 6, 2015
 *
 */

#ifndef PREFILTER3_H_
#define PREFILTER3_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

//#define PREFILTER_TAU (500.0f)	// Defined in preFilter.h

class preFilter3 {
private:
	arm_biquad_cascade_df2T_instance_f32 f;
	float32_t *state;
	float32_t g;

public:
	preFilter3();

	float32_t filterSample(float32_t *x);
};

#endif
