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

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup IMU
 *  @{
 */

#ifndef IMU_H_
#define IMU_H_

#include "logger.h"

#include "LPS25H.h"
#include "L3GD20H.h"
#include "LSM303D.h"
#include "accelCompFilter.h"
#include "gyroCompFilter.h"
#include "accelCompFilter2.h"
#include "gyroCompFilter2.h"

// Complementary filter time constant
#define COMPLEMENTARY_TAU (0.8f)

/**
 * @brief Class for calculating orientation
 *
 * This class is composed of the three (3) chip classes that the Pololu
 * AltIMU-10 v4 carries. Each chip class handles the register reads/writes
 * via i2c. This class is responsible for calculating orientation.
 */
class IMU {
private:
	LPS25H  barometer;				///< Altimeter object
	L3GD20H gyro;					///< Gyroscope object
	LSM303D accel;					///< Accelerometer/Magnetometer object

	logger *log;					///< Logger for gathering data

	accelCompFilter aFilter_x;		///< Complementary filter for x-angle measured by accelerometer
	accelCompFilter aFilter_y;		///< Complementary filter for y-angle measured by accelerometer
	gyroCompFilter gFilter_x;		///< Complementary filter for x-rate measured by gyroscope
	gyroCompFilter gFilter_y;		///< Complementary filter for y-rate measured by gyroscope

	float rate_roll;				///< The angular roll rate [deg/s]
	float rate_pitch;				///< The angular pitch rate [deg/s]
	float angle_roll;				///< The roll angle
	float angle_pitch;				///< The pitch angle

public:
	IMU();
	IMU(L3GD20H_InitStruct gyroConfig, LSM303D_InitStruct accelConfig);

	float getDT(void);

	float getRoll(void);
	float getPitch(void);

	void getRollPitch(float *roll, float*pitch);
};

#endif

/** @} Close IMU group */
/** @} Close Sensors Group */
