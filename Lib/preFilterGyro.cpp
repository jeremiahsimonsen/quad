/**
 * @file preFilterGyro.cpp
 *
 * @brief Class for low-pass filtering raw gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */
#include "preFilterGyro.h"

preFilterGyro::preFilterGyro() {
	// IIR filter coefficient array
 	// {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22}

	// Fs = 50 Hz, Fpass = 1 Hz, Fstop = 2 Hz, Apass = 0.1 dB, Astop = -80 dB
	static float32_t coef[20] = {
		1, -1.95527693158, 1,
		1.9671534848, -0.983957988485,
		1, -1.93562845047, 1,
		1.93697323893, -0.949448544642,
		1, -1.81557763356, 1,
		1.90840490853, -0.914482014726,
		1, 1, 0,
		0.947588073493, 0
	};
	g = 6.28832087242e-05;
	int num_sections = 4;

	// state buffer used by arm routine of size 2*NUM_SECTIONS
	state = (float32_t *)malloc(sizeof(float32_t)*(2*num_sections));

	// arm biquad structure initialization
 	arm_biquad_cascade_df2T_init_f32(&f,num_sections,&coef[0],state);
}

float32_t preFilterGyro::filterSample(float32_t *x) {
	float32_t y = 0.0f;

	arm_biquad_cascade_df2T_f32(&f, (float32_t *)x, &y, 1);

	return y*g;
}




