/**
 * @file IMU.cpp
 *
 * @brief Class for interacting with the inertial measurement unit
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 17, 2015
 *
 */

#ifndef IMU_H_
#define IMU_H_

#include "LPS25H.h"
#include "L3GD20H.h"
#include "LSM303D.h"
#include "accelCompFilter.h"
#include "gyroCompFilter.h"

#define COMPLEMENTARY_TAU (0.95f)

class IMU {
private:
	LPS25H  barometer;
	L3GD20H gyro;
	LSM303D accel;

	accelCompFilter aFilter_x, aFilter_y;
	gyroCompFilter gFilter_x, gFilter_y;

	float rate_roll, rate_pitch, angle_roll, angle_pitch;

public:
	IMU();
	IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig);

	float getDT(void);

	float getRoll(void);
	float getPitch(void);
};

#endif
