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

#include "IMU.h"
#include <math.h>

#define PI 3.1415927f

IMU::IMU()
	: barometer(), gyro(), accel()
{
}

IMU::IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig)
	: barometer(), gyro(gyroConfig), accel(accelConfig)
{
}

void IMU::enable() {
// TODO: Was going to set up input capture for gyro rate...leads to problem w/PCB...
}

float IMU::getDT(void) {
	return gyro.getDT();
}

float IMU::getRoll(void) {
	accel.read();
	gyro.read();

	float accX, accY, accZ;
	accX = accel.getAccX();
	accY = accel.getAccY();
	accZ = accel.getAccZ();

	float accY2 = accY * accY;
	float accZ2 = accZ * accZ;

	float denom = sqrtf(accY2 + accZ2);
	float result = atan2f(accX, denom) * 180.0f / PI;

	return result;
}

float IMU::getPitch(void) {
	accel.read();
	gyro.read();

	float accX, accY, accZ;
	accX = accel.getAccX();
	accY = accel.getAccY();
	accZ = accel.getAccZ();

	float accX2 = accX * accX;
	float accZ2 = accZ * accZ;

	float denom = sqrtf(accX2 + accZ2);
	float result = atan2f(accY, denom) * 180.0f / PI;

	return result;
}
