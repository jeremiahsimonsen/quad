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

#define PI (3.1415927f)

IMU::IMU()
	: barometer(), gyro(), accel()//,
//	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
//	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
}

IMU::IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig)
	: barometer(), gyro(gyroConfig), accel(accelConfig)//,
//	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
//	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
}

float IMU::getDT(void) {
	return gyro.getDT();
}

#if 0 // 4pcb code
float IMU::getRoll(void) {
	// Initiate a new read
	accel.read();
	gyro.read();

	rate_roll = gyro.getX();
	angle_roll = COMPLEMENTARY_TAU * (angle_roll + rate_roll * gyro.getDT());
	angle_roll += (1.0f - COMPLEMENTARY_TAU) * (accel.getAccX());

	return angle_roll;
}

float IMU::getPitch(void) {
	// Initiate a new read
	accel.read();
	gyro.read();

	rate_pitch = gyro.getY();
	angle_pitch = COMPLEMENTARY_TAU * (angle_pitch + rate_pitch * gyro.getDT());
	angle_pitch += (1.0f - COMPLEMENTARY_TAU) * (accel.getAccY());

	return angle_pitch;
}
#endif

#if 1
float IMU::getPitch(void) {
	// Initiate a new read
	accel.read();
	gyro.read();

	// Fetch the pre-filtered accelerometer data [g]
	float ax_f, ay_f, az_f;
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();
//	ax_f = accel.getAccX();
//	ay_f = accel.getAccY();
//	az_f = accel.getAccZ();

	// Fetch the pre-filtered gyroscope data [deg/s]
	float gx_f;
	gx_f = gyro.getXFiltered();
//	gx_f = gyro.getX();

	// Calculate roll angle based on accelerometer data
	float angle_x;
	angle_x = atan2f(ax_f, sqrtf(ay_f*ay_f + az_f*az_f)) * 180.0f / PI;

	angle_roll = COMPLEMENTARY_TAU * (angle_roll + gx_f * gyro.getDT()) + (1.0f - COMPLEMENTARY_TAU) * (angle_x);

	// Complementary filter the accelerometer calculated angle
//	float angle_x_f;
//	angle_x_f = aFilter_x.filterSample(angle_x);

	// Complementary filter the gyroscope data
//	float gyro_x_f;
//	gyro_x_f = gFilter_x.filterSample(gx_f);

	// Sum the two
//	float roll = angle_x_f + gyro_x_f;

	return angle_roll;
}

float IMU::getRoll(void) {
	// Initiate a new read
	accel.read();
	gyro.read();

	// Fetch the pre-filtered accelerometer data [g]
	float ax_f, ay_f, az_f;
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();
//	ax_f = accel.getAccX();
//	ay_f = accel.getAccY();
//	az_f = accel.getAccZ();

	// Fetch the pre-filtered gyroscope data [deg/s]
	float gy_f;
	gy_f = gyro.getYFiltered();
//	gy_f = gyro.getY();

	// Calculate pitch angle based on accelerometer data
	float angle_y;
	angle_y = atan2f(ay_f, sqrtf(ax_f*ax_f + az_f*az_f)) * 180.0f / PI;

	angle_pitch = COMPLEMENTARY_TAU * (angle_pitch + gy_f * gyro.getDT()) + (1.0f-COMPLEMENTARY_TAU)*angle_y;

	// Complementary filter the accelerometer calculated angle
//	float angle_y_f;
//	angle_y_f = aFilter_y.filterSample(angle_y);

	// Complementary filter the gyroscope data
//	float gyro_y_f;
//	gyro_y_f = gFilter_y.filterSample(gy_f);

	// Sum the two
//	float pitch = angle_y_f + gyro_y_f;

	return angle_pitch;
}
#endif



