/**
 * @file preFilterAcc.cpp
 *
 * @brief Class for low-pass filtering raw accelerometer data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

#include "preFilterAcc.h"

preFilterAcc::preFilterAcc() {
	// IIR filter coefficient array
 	// {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22}

	// Fs = 50 Hz, Fpass = 0.15 Hz, Fstop = .5 Hz, Apass = 0.1dB, Astop = -80 dB
	static float32_t coef[15] = {
		1, -1.99621347193, 1,
		1.99373085273, -0.994150990679,
		1, -1.99039477622, 1,
		1.98354165316, -0.98377487276,
		1, 1, 0,
		0.989534474304, 0
	};
	g = 1.40974100238e-05;
	int num_sections = 3;

	// state buffer used by arm routine of size 2*NUM_SECTIONS
	state = (float32_t *)malloc(sizeof(float32_t)*(2*num_sections));

	// arm biquad structure initialization
 	arm_biquad_cascade_df2T_init_f32(&f,num_sections,&coef[0],state);
}

float32_t preFilterAcc::filterSample(float32_t *x) {
	float32_t y = 0.0f;

	arm_biquad_cascade_df2T_f32(&f, (float32_t *)x, &y, 1);

	return y*g;
}




