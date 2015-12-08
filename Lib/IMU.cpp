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

//#define PI (3.1415927f)

IMU::IMU()
	: barometer(), gyro(), accel(),
	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
	log = logger::instance();
}

IMU::IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig)
	: barometer(), gyro(gyroConfig), accel(accelConfig),
	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
	log = logger::instance();
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
	gyro.getYFiltered();
//	gx_f = gyro.getX();

	// Calculate roll angle based on accelerometer data
	float angle_x;
	angle_x = atan2f(ax_f, sqrtf(ay_f*ay_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff[100];
	sprintf(buff, "P %f\n\r",angle_x);
	log->log(buff);
#endif

//	angle_pitch = COMPLEMENTARY_TAU * (angle_pitch + gx_f * gyro.getDT()) + (1.0f - COMPLEMENTARY_TAU) * (angle_x);

	// Complementary filter the accelerometer calculated angle
	float angle_x_f;
	angle_x_f = aFilter_x.filterSample(angle_x);

	// Complementary filter the gyroscope data
	float gyro_x_f;
	gyro_x_f = gFilter_x.filterSample(gx_f);

	// Sum the two
	float pitch = angle_x_f + gyro_x_f;

	return pitch;
//	return angle_pitch;
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
	gyro.getXFiltered();
	gy_f = gyro.getYFiltered();
//	gy_f = gyro.getY();

	// Calculate pitch angle based on accelerometer data
	float angle_y;
	angle_y = atan2f(ay_f, sqrtf(ax_f*ax_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff[100];
	sprintf(buff, "R %f\n\r",angle_y);
	log->log(buff);
#endif

//	angle_roll = COMPLEMENTARY_TAU * (angle_roll + gy_f * gyro.getDT()) + (1.0f-COMPLEMENTARY_TAU)*angle_y;

	// Complementary filter the accelerometer calculated angle
	float angle_y_f;
	angle_y_f = aFilter_y.filterSample(angle_y);

	// Complementary filter the gyroscope data
	float gyro_y_f;
	gyro_y_f = gFilter_y.filterSample(gy_f);

	// Sum the two
	float roll = angle_y_f + gyro_y_f;

	return roll;
//	return angle_roll;
}

void IMU::getRollPitch(float *roll, float*pitch) {
	// Initiate a new read
	accel.read();
	gyro.read();

	// Fetch the pre-filtered accelerometer data [g]
	float ax_f, ay_f, az_f;
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();

	// Fetch the pre-filtered gyroscope data [deg/s]
	float gx_f, gy_f;
	gx_f = gyro.getXFiltered();
	gy_f = gyro.getYFiltered();

	// Calculate pitch & roll angles based on accelerometer data
	float angle_x, angle_y;
	angle_x = atan2f(ax_f, sqrtf(ay_f*ay_f + az_f*az_f)) * 180.0f / PI;
	angle_y = atan2f(ay_f, sqrtf(ax_f*ax_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff1[100];
	sprintf(buff1, "P %f\n\rR %f\n\r",angle_x,angle_y);
	log->log(buff1);
#endif

//	*pitch = COMPLEMENTARY_TAU * (angle_pitch + gx_f * gyro.getDT()) + (1.0f - COMPLEMENTARY_TAU) * (angle_x);
//	*roll  = COMPLEMENTARY_TAU * (angle_roll + gy_f * gyro.getDT()) + (1.0f-COMPLEMENTARY_TAU)*angle_y;

	// Complementary filter the accelerometer calculated angle
	float angle_x_f, angle_y_f;
	angle_x_f = aFilter_x.filterSample(angle_x);
	angle_y_f = aFilter_y.filterSample(angle_y);

	// Complementary filter the gyroscope data
	float gyro_x_f, gyro_y_f;
	gyro_x_f = gFilter_x.filterSample(gx_f);
	gyro_y_f = gFilter_y.filterSample(gy_f);

#ifdef LOG_COMP_FILTERED
	char buff2[100];
	sprintf(buff2, "Pf %f\n\rRf %f\n\rPdf %f\n\rRdf %f\n\r", angle_x_f, angle_y_f, gyro_x_f, gyro_y_f);
	log->log(buff2);
#endif

	// Sum the two
	*pitch = angle_x_f + gyro_x_f;
	*roll  = angle_y_f + gyro_y_f;

#ifdef LOG_OUTPUT
	char buff3[100];
	sprintf(buff3, "PT %f\n\rRT %f\n\r", *pitch, *roll);
	log->log(buff3);
#endif
}

#endif



