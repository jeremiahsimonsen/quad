/**
 * @file
 *
 * @brief Class for interfacing with the ST LSM303D 3-axis accelerometer and magnetometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 9, 2015
 *
 * Based on lsm303-arduino library from Pololu Corp. See license.txt for details
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup IMU
 *  @{
 */

/** @addtogroup LSM303D
 *  @{
 */

#include "LSM303D.h"
#include "errDC9000.h"

/**
 * @brief Instantiates sensor with default configuration
 */
LSM303D::LSM303D()
	: ax(PREFILTER_TAU), ay(PREFILTER_TAU), az(PREFILTER_TAU)
//	: ax(), ay(), az()
{
	// Get a pointer to the logger
	log = logger::instance();

	// Save the i2c slave address of the sensor
	address = 0b00111010;

	// Initialize members
	accXOffset = accYOffset = accZOffset = 0.0f;

	// Initialize the I2C pointer
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Default settings
	LSM303D_InitStruct init;
	init.aodr_config = LSM_AODR_Config::EIGHT;		// 200 Hz Accelerometer output data rate
	init.abw_config  = LSM_ABW_Config::ONE;			// 773 Hz Acceleration anti-alias filter bandwidth
	init.afs_config  = LSM_AFS_Config::FOUR;		// +/-4g Acceleration full-scale
	init.modr_config = LSM_MODR_Config::FIVE;		// 50 Hz Magnetometer output data rate
	init.mres_config = LSM_MRES_Config::HIGH;		// Magnetometer high-resolution mode
	init.mfs_config  = LSM_MFS_Config::FOUR;		// +/- 4 gauss Magnetometer full-scale
	init.md_config   = LSM_MD_Config::CONTINUOUS;	// Magnetic sensor continuous mode
	accResolution    = 0.122e-3f;
	magResolution	 = 0.160e-3f;

	// Perform i2c writes to setup the sensor
	enable(init);
}

/**
 * @brief Instantiates sensor with given configuration parameters
 * @param init Sensor configuration parameters
 */
LSM303D::LSM303D(LSM303D_InitStruct init)
	: ax(PREFILTER_TAU), ay(PREFILTER_TAU), az(PREFILTER_TAU)
//	: ax(), ay(), az()
{
	// Get a pointer to the logger
	log = logger::instance();

	// Save the i2c slave address of the sensor
	address = 0b00111010;

	// Initialize members
	accXOffset = accYOffset = accZOffset = 0.0f;

	// Initialize the I2C pointer
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Determine the appropriate resolutions
	switch(init.afs_config) {
	case LSM_AFS_Config::TWO	: accResolution = 0.061e-3f; break;
	case LSM_AFS_Config::FOUR	: accResolution = 0.122e-3f; break;
	case LSM_AFS_Config::SIX	: accResolution = 0.183e-3f; break;
	case LSM_AFS_Config::EIGHT	: accResolution = 0.244e-3f; break;
	case LSM_AFS_Config::SIXTEEN: accResolution = 0.732e-3f; break;
	default: break;
	}

	switch(init.mfs_config) {
	case LSM_MFS_Config::TWO	: magResolution = 0.080e-3f; break;
	case LSM_MFS_Config::FOUR	: magResolution = 0.160e-3f; break;
	case LSM_MFS_Config::EIGHT	: magResolution = 0.320e-3f; break;
	case LSM_MFS_Config::TWELVE	: magResolution = 0.479e-3f; break;
	default: break;
	}

	// Perform i2c writes to setup the sensor
	enable(init);
}

/**
 * @brief Configure the sensor operation
 * @param init Sensor operation parameters
 */
void LSM303D::enable(LSM303D_InitStruct init) {
	uint8_t buf;

	// Set Accelerometer output data rate; Enable 3 axis acc operation
	buf = LSM303D_CTRL1_AODR(init.aodr_config)
			| LSM303D_CTRL1_BDU_MASK
			| LSM303D_CTRL1_AXEN_MASK
			| LSM303D_CTRL1_AYEN_MASK
			| LSM303D_CTRL1_AZEN_MASK;
	if ( i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL1, &buf, 1) < 0) {
		Error_Handler(errDC9000::LSM_INIT_ERROR);
	}

	// Set accelerometer anti-alias filter bandwidth and full-scale
	buf = (uint8_t)(LSM303D_CTRL2_ABW(init.abw_config)
			| LSM303D_CTRL2_AFS(init.afs_config));
	if ( i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL2, &buf, 1) < 0) {
		Error_Handler(errDC9000::LSM_INIT_ERROR);
	}

	// Set magnetometer resolution and output data rate
	buf = (uint8_t)(LSM303D_CTRL5_M_RES(init.mres_config)
			| LSM303D_CTRL5_M_ODR(init.modr_config));
	if ( i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL5, &buf, 1) < 0) {
		Error_Handler(errDC9000::LSM_INIT_ERROR);
	}

	// Set magnetic full-scale
	buf = LSM303D_CTRL6_MFS(init.mfs_config);
	if ( i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL6, &buf, 1) < 0) {
		Error_Handler(errDC9000::LSM_INIT_ERROR);
	}

	// Set magnetic sensor mode
	buf = (uint8_t)(LSM303D_CTRL7_MD(init.md_config));
//			| LSM303D_CTRL7_AFDS_MASK);
	if ( i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL7, &buf, 1) < 0) {
		Error_Handler(errDC9000::LSM_INIT_ERROR);
	}

	// Initialize PA5 for externally measuring sampling frequency
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.Pin 	= GPIO_PIN_5;
//	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull	= GPIO_NOPULL;
//	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Calibrate the sensor to eliminate offset error
	accCalibrate();
}

/**
 * @brief Basic calibration to eliminate offset errors
 *
 * Initializes the offset members so offset error can be reduced/eliminated.
 * The sensor must remain stationary during calibration.
 */
void LSM303D::accCalibrate(void) {
	uint8_t samples = 128;
	float ax_offset = 0.0f, ay_offset = 0.0f, az_offset = 0.0f;

	readAcc();	// Initial read so values are correct
	// Take some readings and accumulate
	for (int i = 0; i < samples; i++) {
		readAcc();
		ax_offset += getAccXFiltered();
		ay_offset += getAccYFiltered();
		az_offset += getAccZFiltered();
		HAL_Delay(20);
	}

	// Average the offsets
	ax_offset = ax_offset / (float)samples;
	ay_offset = ay_offset / (float)samples;
	az_offset = (az_offset / (float)samples) + 1.0f;	// Subtract gravity

	// Save the offsets
	accXOffset = ax_offset;
	accYOffset = ay_offset;
	accZOffset = az_offset;
}

/**
 * @brief Initiates a read of the accelerometer and magnetometer
 */
void LSM303D::read(void) {
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	readAcc();
	readMag();
}

/**
 * @brief Initiates a read of the accelerometer data registers
 * @note  Calls Error_Handler() on error
 */
void LSM303D::readAcc(void) {
	// Read from the accelerometer registers
	if ( i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_A | (1<<7) ), accBuff, 6) < 0) {
		Error_Handler(errDC9000::LSM_IO_ERROR);
	}
}

/**
 * @brief  Function to get the raw acceleration on the x axis
 * @return Raw x axis acceleration (register values)
 */
int16_t LSM303D::getAccXRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw X acceleration
	return -(int16_t)(accBuff[1]<<8 | accBuff[0]);
}

/**
 * @brief  Function to get the acceleration on the x axis
 * @return X axis acceleration (g)
 */
float LSM303D::getAccX() {
	float x = (float)getAccXRaw() * accResolution - accXOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Ax %f\n\r", x);
	log->log(buff);
#endif
	return x;
}

/**
 * @brief  Function to get the filtered acceleration in the x direction
 * @return Filtered X acceleration (g)
 */
float LSM303D::getAccXFiltered() {
	float32_t x = getAccX();
	float xf = ax.filterSample(&x);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Axf %f\n\r", xf);
	log->log(buff);
#endif
	return xf;
}

/**
 * @brief  Function to get the raw acceleration on the y axis
 * @return Raw y axis acceleration (register values)
 */
int16_t LSM303D::getAccYRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw Y acceleration
	return -(int16_t)(accBuff[3]<<8 | accBuff[2]);
}

/**
 * @brief  Function to get the acceleration on the y axis
 * @return Y axis acceleration (g)
 */
float LSM303D::getAccY() {
	float y = (float)getAccYRaw() * accResolution - accYOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Ay %f\n\r", y);
	log->log(buff);
#endif
	return y;
}

/**
 * @brief  Function to get the filtered acceleration on the y axis
 * @return Filtered Y accleration (g)
 */
float LSM303D::getAccYFiltered() {
	float32_t y = getAccY();
	float yf = ay.filterSample(&y);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Ayf %f\n\r", yf);
	log->log(buff);
#endif
	return yf;
}

/**
 * @brief  Function to get the raw acceleration on the z axis
 * @return Raw z axis acceleration (register values)
 */
int16_t LSM303D::getAccZRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw Z acceleration
	return (int16_t)(accBuff[5]<<8 | accBuff[4]);
}

/**
 * @brief  Function to get the acceleration on the z axis
 * @return Z axis acceleration (g)
 */
float LSM303D::getAccZ() {
	float z = (float)getAccZRaw() * accResolution - accZOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Az %f\n\r", z);
	log->log(buff);
#endif
	return z;
}

/**
 * @brief  Function to get the filtered acceleration on the z-axis
 * @return Filtered Z acceleration (g)
 */
float LSM303D::getAccZFiltered() {
	float32_t z = getAccZ();
	float zf = az.filterSample(&z);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Azf %f\n\r", zf);
	log->log(buff);
#endif
	return zf;
}

/**
 * @brief Initiates a read of all 3 magnetometer axes
 * @note  Calls Error_Handler() on error
 */
void LSM303D::readMag(void) {
	if ( i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_M | (1<<7) ), magBuff, 6) < 0) {
		Error_Handler(errDC9000::LSM_IO_ERROR);
	}

}

/**
 * @brief  Function to get the magnetic field strength on the x axis
 * @return Raw x axis magnetic field (register values)
 */
int16_t LSM303D::getMagXRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw x magnetic reading
	return (int16_t)(magBuff[1]<<8 | magBuff[0]);
}

/**
 * @brief  Function to get the magnetic field strength on the x axis
 * @return X axis magnetic field (gauss)
 */
float LSM303D::getMagX() {
	float retVal = (float)getMagXRaw() * magResolution;
	return retVal;
}

/**
 * @brief  Function to get the magnetic field strength on the y axis
 * @return Raw y axis magnetic field (register values)
 */
int16_t LSM303D::getMagYRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw y magnetic reading
	return (int16_t)(magBuff[3]<<8 | magBuff[2]);
}

/**
 * @brief  Function to get the magnetic field strength on the y axis
 * @return Y axis magnetic field (gauss)
 */
float LSM303D::getMagY() {
	float retVal = (float)getMagYRaw() * magResolution;
	return retVal;
}

/**
 * @brief  Function to get the magnetic field strength on the z axis
 * @return Raw z axis magnetic field (register values)
 */
int16_t LSM303D::getMagZRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw z magnetic reading
	return (int16_t)(magBuff[5]<<8 | magBuff[4]);
}

/**
 * @brief  Function to get the magnetic field strength on the z axis
 * @return Z axis magnetic field (gauss)
 */
float LSM303D::getMagZ() {
	float retVal = (float)getMagZRaw() * magResolution;
	return retVal;
}

/** @} Close LSM303D group */
/** @} Close IMU group */
/** @} Close Sensors Group */
