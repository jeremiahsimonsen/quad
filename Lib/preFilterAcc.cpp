/**
 * @file
 *
 * @brief Class for low-pass filtering raw accelerometer data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

/** @addtogroup Control
 *  @{
 */

/** @addtogroup PREFILTER
 *  @{
 */

#include "preFilterAcc.h"

/**
 * @brief Construct a preFilterAcc object
 *
 * Constructs an arbitrary IIR filter object and initializes the ARM structs
 * and filter coefficients.
 */
preFilterAcc::preFilterAcc() {
	/* Variables for several filters are below. Uncomment the one for the
	 * desired filter transfer function.
	 */

	// IIR filter coefficient array
 	// {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22}

	// Fs = 50 Hz, Fpass = 0.15 Hz, Fstop = .5 Hz, Apass = 0.1dB, Astop = -80 dB
//	static float32_t coef[15] = {
//		1, -1.99621347193, 1,
//		1.99373085273, -0.994150990679,
//		1, -1.99039477622, 1,
//		1.98354165316, -0.98377487276,
//		1, 1, 0,
//		0.989534474304, 0
//	};
//	g = 1.40974100238e-05;
//	int num_sections = 3;

	// Fs = 100 Hz, Fpass = 2 Hz, Fstop = 4 Hz, Apass = 0.1 dB, Astop = -80 dB
//	static float32_t coef[20] = {
//		1, -1.95527693158, 1,
//		1.9671534848, -0.983957988485,
//		1, -1.93562845047, 1,
//		1.93697323893, -0.949448544642,
//		1, -1.81557763356, 1,
//		1.90840490853, -0.914482014726,
//		1, 1, 0,
//		0.947588073493, 0
//	};
//	g = 6.28832087242e-05;
//	int num_sections = 4;

	// Fs = 100 Hz, Fpass = 2 Hz, Fstop = 3 Hz, Apass = 0.1 dB, Astop = -80 dB
	static float32_t coef[20] = {
		1, -1.95953138525, 1,
		1.95144255108, -0.964882197289,
		1, -1.92112902732, 1,
		1.92640902728, -0.934482799651,
		1, -1.48432259432, 1,
		1.90586457236, -0.909019680291,
		1, -1.96822547213, 1,
		1.97290627125, -0.989358918125
	};
	g = 0.000106468611206;
	int num_sections = 4;

	// Fs = 100 Hz, Fpass = 2 Hz, Fstop = 2.5 Hz, Apass = 0.1 dB, Astop = -100 dB
//	static float32_t coef[30] = {
//		1, -1.97608908926, 1,
//		1.97938907666, -0.995441533138,
//		1, -1.97359684625, 1,
//		1.97024840267, -0.984922682837,
//		1, -1.96623938279, 1,
//		1.95865566304, -0.970487577651,
//		1, -1.94323175055, 1,
//		1.94397169161, -0.951623765262,
//		1, -1.82015664773, 1,
//		1.9296486596, -0.933026767942,
//		1, 1, 0,
//		0.961670531286, -0
//	};
//	g = 6.34521966566e-06;
//	int num_sections = 6;

	// state buffer used by arm routine of size 2*NUM_SECTIONS
	state = (float32_t *)malloc(sizeof(float32_t)*(2*num_sections));

	// arm biquad structure initialization
 	arm_biquad_cascade_df2T_init_f32(&f,num_sections,&coef[0],state);
}

/**
 * @brief   Calculate the filter output
 * @param x The current sample input
 * @return  The corresponding filter output
 *
 * Calculates the filtered value using the IIR filter coefficients from the
 * constructor.
 */
float32_t preFilterAcc::filterSample(float32_t *x) {
	float32_t y = 0.0f;

	// Calculate output using the ARM routine
	arm_biquad_cascade_df2T_f32(&f, (float32_t *)x, &y, 1);

	// Scale by the gain
	return y*g;
}

/** @} Close PREFILTER group */
/** @} Close Control Group */


