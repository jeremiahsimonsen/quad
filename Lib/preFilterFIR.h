/**
 * @file preFilterFIR.h
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 6, 2015
 *
 */

#ifndef PREFILTERFIR_H_
#define PREFILTERFIR_H_

#include <stdlib.h>
#include "stm32f407xx.h"
#include "arm_math.h"

//#define PREFILTER_TAU (500.0f)	// Defined in preFilter.h

class preFilterFIR {
private:
	arm_fir_instance_f32 f;
	float32_t *state;

public:
	preFilterFIR();

	float32_t filterSample(float32_t *x);
};

#endif
