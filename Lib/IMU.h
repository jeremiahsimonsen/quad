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

class IMU {
private:
	LPS25H  barometer;
	L3GD20H gyro;
	LSM303D accel;

public:
	IMU();

	float getRoll(void);
	float getPitch(void);
};

#endif
