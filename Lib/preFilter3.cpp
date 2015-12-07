/**
 * @file preFilter3.cpp
 *
 * @brief Class for low-pass filtering raw accelerometer and gyro data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 6, 2015
 *
 */

#include "preFilter3.h"

preFilter3::preFilter3() {
	// IIR filter coefficient array
 	// {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22}

	// Sample frequency 500 Hz, Pass up to 5 Hz, stop above 25 Hz
//	static float32_t coef[10] = {
//		1.0f, -1.8256f, 1.0f,	// b10, b11, b12
//		1.94551f, -0.946985f,	// a11, a12
//		1.0f, -1.9673f, 1.0f,	// b20, b21, b22
//		1.97569f, -0.979843f	// a21, a22
//	};
//	g = 0.00101447;
//	int num_sections = 2;

	// Fs = 1550 Hz, Fpass = 1 Hz, Fstop = 5 Hz
	// Apass = 0.01 dB, Astop = -110 dB
//	static float32_t coef[15] = {
//		1.0f, -1.99544, 1.0f,	// b10, b11, b12
//		1.9959f, -0.995891f,	// a11, a12
//		1.0f, -1.99938f, 1.0f,	// b20, b21, b22
//		1.99349f, -0.993518f,	// a21, a22
//		1.0f, -1.99966f, 1.0f,	// b30, b31, b32
//		2.00024f, -1.00027f		// a31, a32
//	};
//	g = 3.15028e-06f;
//	int num_sections = 3;

	// Fs = 800 Hz, Fpass = 1 Hz, Fstop = 5 Hz, Apass = 0.1 dB, Astop = -80 dB
//	static float coef[15] = {
//		1.0f, 1.0f, 0.0f,
//		0.995626, 0.0f,
//		1.0f, -1.99833f, 1.0f,
//		1.99317f, -0.993208f,
//		1.0f, -1.99934f, 1.0f,
//		1.99749f, -0.997558f
//	};
//	g = 5.9143e-06f;
//	int num_sections = 3;

	// Fs = 800 Hz, Fpass = 0.5 Hz, Fstop = 4 Hz, Apass = 0.1 dB, Astop = -200 dB
//	static float coef[20] = {
//		1, -1.98195, 1,
//		1.9991, -0.998764,
//		1, -1.99776, 0.999999,
//		1.97252, -0.972875,
//		1, -1.999, 1,
//		2.02416, -1.02449,
//		1, -1.99927, 0.999996,
//		1.99759, -0.99794
//	};
//	g = 1.00221e-10;
//	int num_sections = 4;

	// Fs = 25 Hz, Fpass = 0.5 Hz, Fstop = 4 Hz, Apass = 0.1 dB, Astop = -200 dB
//	static float coef[20] = {
//		1, 1.29237465749, 1,
//		1.91959747632, -0.921837935858,
//		1, -0.538943597193, 1,
//		1.92733026321, -0.933743480607,
//		1, -1.17959762145, 1,
//		1.94325704798, -0.955622928744,
//		1, -1.37268562748, 1,
//		1.96756848519, -0.984286535147
//	};
//	g = 1.18614571697e-09;
//	int num_sections = 4;

	// Fs = 25 Hz, Fpass = 0.5 Hz, Fstop = 3 Hz, Apass = 0.1dB, Astop = -400 dB
	static float32_t coef[40] = {
		1, -1.53827732741, 1,
		1.9736770358, -0.988465011822,
		1, -1.46835640208, 1,
		1.96864488667, -0.981258720075,
		1, -1.33634156748, 1,
		1.96493352976, -0.974725242506,
		1, -1.09028678073, 1,
		1.96237114802, -0.969109500058,
		1, -0.611754501065, 1,
		1.96073432731, -0.964645755383,
		1, 0.332264013216, 1,
		1.95979755697, -0.961539366484,
		1, 1.6981210746, 1,
		1.95937953194, -0.959944614614,
		1, -1.56885562701, 1,
		1.98010320081, -0.996098893256,
	};
	g = 9.79310676897e-19;
	int num_sections = 8;

	// state buffer used by arm routine of size 2*NUM_SECTIONS
	state = (float32_t *)malloc(sizeof(float32_t)*(2*num_sections));

	// arm biquad structure initialization
 	arm_biquad_cascade_df2T_init_f32(&f,num_sections,&coef[0],state);
}

float32_t preFilter3::filterSample(float32_t *x) {
	float32_t y = 0.0f;

	arm_biquad_cascade_df2T_f32(&f, (float32_t *)x, &y, 1);

	return y*g;
}




