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

	// Fs = 100 Hz, Fpass = 5 Hz, Fstop = 8 Hz, Apass = 0.1 dB, Astop = -80 dB
//	static float32_t coef[20] = {
//		1, -1.75666833722, 1,
//		1.83382611119, -0.915365604501,
//		1, -1.54774841395, 1,
//		1.79622106803, -0.844470699344,
//		1, -0.0640337980066, 1,
//		1.76796044753, -0.78659156693,
//		1, -1.80681823605, 1,
//		1.87276285114, -0.973992570455
//	};
//	g = 0.000178224074641;
//	int num_sections = 4;

	// Fs = 100 Hz, Fpass = 6 Hz, Fstop = 8 Hz, Apass = 0.1 dB, Astop = -100 dB
//	static float32_t coef[25] = {
//		1, -1.7206089427, 1,
//		1.8151227321, -0.939964858765,
//		1, -1.61482002848, 1,
//		1.7964606786, -0.888009064336,
//		1, -1.25387574785, 1,
//		1.78020524333, -0.829872992942,
//		1, 0.564231736618, 1,
//		1.76955193066, -0.787650130071,
//		1, -1.75485269648, 1,
//		1.83838103862, -0.981342330232
//	};
//	g = 2.87652675854e-05;
//	int num_sections = 5;

	// Fs = 100 Hz, Fpass = 6.25 Hz, Fstop = 8 Hz, Apass = 0.1 dB, Astop = -80 dB
//	static float32_t coef[25] = {
//		1, -1.76102027226, 1,
//		1.82317309624, -0.978493231909,
//		1, -1.72166168124, 1,
//		1.79394642255, -0.927636823416,
//		1, -1.58323951337, 1,
//		1.76433264674, -0.857023136127,
//		1, -0.95358729133, 1,
//		1.73508772275, -0.778562952482,
//		1, 1, 0,
//		0.860755376203, -0
//	};
//	g = 0.000200830599328;
//	int num_sections = 5;

	// Fs = 100 Hz, Fpass = 6.25 Hz, Fstop = 8 Hz, Apass = 0.1 dB, Astop = -40 dB
//	static float32_t coef[15] = {
//		1, -1.67171299109, 1,
//		1.71022092712, -0.833197919132,
//		1, -0.631952192663, 1,
//		1.58706817569, -0.648599546625,
//		1, -1.77493127647, 1,
//		1.79913121367, -0.958726718288
//	};
//	g = 0.0118105736583;
//	int num_sections = 3;

	// Fs = 100 Hz, Fpass = 12.5 Hz, Fstop = 15 Hz, Apass = 0.1 dB, Astop = -40 dB
	static float32_t coef[15] = {
		1, -0.882441011183, 1,
		1.2617494564, -0.708807988091,
		1, 0.770732765855, 1,
		1.19566959055, -0.410588782579,
		1, -1.17828082288, 1,
		1.32666260013, -0.925616102885
	};
	g = 0.0223585778671;
	int num_sections = 3;

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




