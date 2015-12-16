/**
 * @file
 *
 * @brief Class for interacting with the inertial measurement unit
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 17, 2015
 *
 */

/** @defgroup Sensors Macro peripherals (sensors)
 *  @brief Peripherals external to the MCU
 *
 *  The Death Chopper 9000 uses several sensors/devices external to the MCU,
 *  including:
 *  	- Pololu @ref IMU "AltIMU10-v4"
 *  	-
 *
 *  @{
 */

/** @defgroup IMU Inertial Measurement Unit (IMU)
 *  @brief Module for calculating orientation using Pololu AltIMU10
 *
 *  Quadcopters utilize a 3-axis acclerometer and a 3-axis gyroscope for
 *  calculating pitch and roll angles. A 3-axis magnetometer is used for
 *  measuring the absolute yaw angle relative to magnetic north.
 *
 *  @{
 */

#include "IMU.h"
#include <math.h>

// Define for whether or not pre-filtered sensor data should be used for calculations
#define USE_PREFILTERED

/**
 * @brief Create an IMU object with default sensor configurations
 *
 * Initializes each of the sensors and the required filters
 */
IMU::IMU()
	: barometer(), gyro(), accel(),
	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	// Initialize members
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
	log = logger::instance();
}

/**
 * @brief Create an IMU object with a given sensor configuration
 * @param gyroConfig  Configuration options for the gyro
 * @param accelConfig Configuration options for the accelerometer/magnetometer
 */
IMU::IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig)
	: barometer(), gyro(gyroConfig), accel(accelConfig),
	  aFilter_x(COMPLEMENTARY_TAU), aFilter_y(COMPLEMENTARY_TAU),
	  gFilter_x(COMPLEMENTARY_TAU), gFilter_y(COMPLEMENTARY_TAU)
{
	// Initialize members
	rate_roll = rate_pitch = angle_roll = angle_pitch = 0.0f;
	log = logger::instance();
}

/**
 * @brief  Sample rate measurement
 * @return The sample time
 */
float IMU::getDT(void) {
	return gyro.getDT();
}

/**
 * @brief  Calculate the pitch angle
 * @return The pitch angle [deg]
 */
float IMU::getPitch(void) {
	// Initiate new reads
	accel.read();
	gyro.read();

	// Fetch the pre-filtered accelerometer data [g]
	float ax_f, ay_f, az_f;
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();

	// Fetch the pre-filtered gyroscope data [deg/s]
	float gx_f;
	gx_f = gyro.getXFiltered();
	gyro.getYFiltered();

	// Calculate pitch angle based on accelerometer data
	float angle_x;
	angle_x = atan2f(ax_f, sqrtf(ay_f*ay_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff[100];
	sprintf(buff, "P %f\n\r",angle_x);
	log->log(buff);
#endif

	// Alternate implementation
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

/**
 * @brief  Calculate the roll angle
 * @return The roll angle [deg]
 */
float IMU::getRoll(void) {
	// Initiate new reads
	accel.read();
	gyro.read();

	// Fetch the pre-filtered accelerometer data [g]
	float ax_f, ay_f, az_f;
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();

	// Fetch the pre-filtered gyroscope data [deg/s]
	float gy_f;
	gyro.getXFiltered();
	gy_f = gyro.getYFiltered();

	// Calculate pitch angle based on accelerometer data
	float angle_y;
	angle_y = atan2f(ay_f, sqrtf(ax_f*ax_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff[100];
	sprintf(buff, "R %f\n\r",angle_y);
	log->log(buff);
#endif

	// Alternate implementation
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

/**
 * @brief Calculate the roll and pitch simultaneously
 * @param roll  [out] The roll angle [deg]
 * @param pitch [out] The pitch angle [deg]
 */
void IMU::getRollPitch(float *roll, float*pitch) {
	// Initiate new reads
	accel.read();
	gyro.read();

	float ax_f, ay_f, az_f;
	float gx_f, gy_f;
#ifdef USE_PREFILTERED
	// Fetch the pre-filtered accelerometer data [g]
	ax_f = accel.getAccXFiltered();
	ay_f = accel.getAccYFiltered();
	az_f = accel.getAccZFiltered();

	// Fetch the pre-filtered gyroscope data [deg/s]
	gx_f = gyro.getXFiltered();
	gy_f = gyro.getYFiltered();
#else
	// Fetch the unfiltered accelerometer data [g]
	ax_f = accel.getAccX();
	ay_f = accel.getAccY();
	az_f = accel.getAccZ();

	// Fetch the unfiltered gyroscope data [deg/s]
	gx_f = gyro.getX();
	gy_f = gyro.getY();
#endif

	// Calculate pitch & roll angles based on accelerometer data
	float angle_x, angle_y;
	angle_x = atan2f(ax_f, sqrtf(ay_f*ay_f + az_f*az_f)) * 180.0f / PI;
	angle_y = atan2f(ay_f, sqrtf(ax_f*ax_f + az_f*az_f)) * 180.0f / PI;

#ifdef LOG_ACC_ANGLE
	char buff1[100];
	sprintf(buff1, "P %f\n\rR %f\n\r",angle_x,angle_y);
	log->log(buff1);
#endif

	angle_pitch = COMPLEMENTARY_TAU * (angle_pitch + gx_f * gyro.getDT()) + (1.0f - COMPLEMENTARY_TAU) * (angle_x);
	angle_roll  = COMPLEMENTARY_TAU * (angle_roll + gy_f * gyro.getDT()) + (1.0f-COMPLEMENTARY_TAU)*angle_y;

	// Complementary filter the accelerometer calculated angle
//	float angle_x_f, angle_y_f;
//	angle_x_f = aFilter_x.filterSample(angle_x);
//	angle_y_f = aFilter_y.filterSample(angle_y);

	// Complementary filter the gyroscope data
//	float gyro_x_f, gyro_y_f;
//	gyro_x_f = gFilter_x.filterSample(gx_f);
//	gyro_y_f = gFilter_y.filterSample(gy_f);

#ifdef LOG_COMP_FILTERED
	char buff2[100];
	sprintf(buff2, "Pf %f\n\rRf %f\n\rPdf %f\n\rRdf %f\n\r", angle_x_f, angle_y_f, gyro_x_f, gyro_y_f);
	log->log(buff2);
#endif

	// Sum the two
//	*pitch = angle_x_f + gyro_x_f;
//	*roll  = angle_y_f + gyro_y_f;
	*pitch = angle_pitch;
	*roll  = angle_roll;

#ifdef LOG_OUTPUT
	char buff3[100];
	sprintf(buff3, "PT %f\n\rRT %f\n\r", *pitch, *roll);
	log->log(buff3);
#endif
}

/** @} Close IMU group */
/** @} Close Peripherals Group */

