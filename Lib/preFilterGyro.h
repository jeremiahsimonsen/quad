/**
 * @file preFilterGyro.h
 *
 * @brief Class for low-pass filtering raw gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

#ifndef PREFILTERGYRO_H_
#define PREFILTERGYRO_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

class preFilterGyro {
private:
	arm_biquad_cascade_df2T_instance_f32 f;
	float32_t *state;
	float32_t g;

public:
	preFilterGyro();

	float32_t filterSample(float32_t *x);
};

#endif
