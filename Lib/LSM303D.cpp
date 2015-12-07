/**
 * @file LSM303D.cpp
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

#include "LSM303D.h"

/**
 * @brief Instantiates sensor with default configuration
 */
LSM303D::LSM303D()
	: ax(PREFILTER_TAU), ay(PREFILTER_TAU), az(PREFILTER_TAU)
{
	// Save the i2c slave address of the sensor
	address = 0b00111010;

	accXOffset = accYOffset = accZOffset = 0.0f;

	// Initialize the I2C pointer
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Default settings
	LSM303D_InitStruct init;
	init.aodr_config = LSM_AODR_Config::EIGHT;		// 200 Hz Accelerometer output data rate
	init.abw_config  = LSM_ABW_Config::ONE;			// 773 Hz Acceleration anti-alias filter bandwidth
	init.afs_config  = LSM_AFS_Config::FOUR;		// ±4g Acceleration full-scale
	init.modr_config = LSM_MODR_Config::FIVE;		// 50 Hz Magnetometer output data rate
	init.mres_config = LSM_MRES_Config::HIGH;		// Magnetometer high-resolution mode
	init.mfs_config  = LSM_MFS_Config::FOUR;		// ± 4 gauss Magnetometer full-scale
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
{
	// Save the i2c slave address of the sensor
	address = 0b00111010;

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
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL1, &buf, 1);

	// Set accelerometer anti-alias filter bandwidth and full-scale
	buf = (uint8_t)(LSM303D_CTRL2_ABW(init.abw_config)
			| LSM303D_CTRL2_AFS(init.afs_config));
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL2, &buf, 1);

	// Set magnetometer resolution and output data rate
	buf = (uint8_t)(LSM303D_CTRL5_M_RES(init.mres_config)
			| LSM303D_CTRL5_M_ODR(init.modr_config));
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL5, &buf, 1);

	// Set magnetic full-scale
	buf = LSM303D_CTRL6_MFS(init.mfs_config);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL6, &buf, 1);

	// Set magnetic sensor mode
	buf = (uint8_t)(LSM303D_CTRL7_MD(init.md_config));
//			| LSM303D_CTRL7_AFDS_MASK);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL7, &buf, 1);

	accCalibrate();
}

void LSM303D::accCalibrate(void) {
	uint8_t samples = 32;
	float ax_offset = 0.0f, ay_offset = 0.0f, az_offset = 0.0f;

	readAcc();	// Initial read so values are correct
	// Take some readings and accumulate
	for (int i = 0; i < samples; i++) {
		readAcc();
		ax_offset += getAccX();
		ay_offset += getAccY();
		az_offset += getAccZ();
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
	readAcc();
	readMag();
}

/**
 * @brief  Initiates a read of the accelerometer data registers
 * @return  The return value of I2C::memRead()
 * 			DOUBLE BUFFERING: 1 or 2 means which buffer is readable, -1 means HAL_ERROR
 * 			ELSE: 0 on success, -1 on HAL_ERROR
 * 			Updates data in accBuff
 */
uint8_t LSM303D::readAcc(void) {
#if USE_DOUBLE_BUFFERING
	accBuffIndicator = i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_A | (1<<7) ), accBuff1, accBuff2, 6);
	return accBuffIndicator;
#else
	return i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_A | (1<<7) ), accBuff, 6);
#endif
}

/**
 * @brief  Function to get the acceleration on the x axis
 * @return Raw x axis acceleration (register values)
 */
int16_t LSM303D::getAccXRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (accBuffIndicator == 1) {
		return (accBuff1[1]<<8 | accBuff1[0]);
	}
	else if (accBuffIndicator == 2) {
		return accBuff2[1]<<8 | accBuff2[0];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return -(int16_t)(accBuff[1]<<8 | accBuff[0]);
#endif
}

/**
 * @brief  Function to get the acceleration on the x axis
 * @return X axis acceleration (g)
 */
float LSM303D::getAccX() {
	float x = (float)getAccXRaw() * accResolution - accXOffset;

	return x;
}

float LSM303D::getAccXFiltered() {
	float xf = ax.filterSample(getAccX());
	return xf;
}

/**
 * @brief  Function to get the acceleration on the y axis
 * @return Raw y axis acceleration (register values)
 */
int16_t LSM303D::getAccYRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (accBuffIndicator == 1) {
		return (accBuff1[3]<<8 | accBuff1[2]);
	}
	else if (accBuffIndicator == 2) {
		return accBuff2[3]<<8 | accBuff2[2];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return -(int16_t)(accBuff[3]<<8 | accBuff[2]);
#endif
}

/**
 * @brief  Function to get the acceleration on the y axis
 * @return Y axis acceleration (g)
 */
float LSM303D::getAccY() {
	float y = (float)getAccYRaw() * accResolution - accYOffset;

	return y;
}

float LSM303D::getAccYFiltered() {
	float yf = ay.filterSample(getAccY());
	return yf;
}

/**
 * @brief  Function to get the acceleration on the z axis
 * @return Raw z axis acceleration (register values)
 */
int16_t LSM303D::getAccZRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (accBuffIndicator == 1) {
		return accBuff1[5]<<8 | accBuff1[4];
	}
	else if (accBuffIndicator == 2) {
		return accBuff2[5]<<8 | accBuff2[4];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(accBuff[5]<<8 | accBuff[4]);
#endif
}

/**
 * @brief  Function to get the acceleration on the z axis
 * @return Z axis acceleration (g)
 */
float LSM303D::getAccZ() {
	float z = (float)getAccZRaw() * accResolution - accZOffset;

	return z;
}

float LSM303D::getAccZFiltered() {
	float zf = az.filterSample(getAccZ());
	return zf;
}

uint8_t LSM303D::readMag(void) {
#if USE_DOUBLE_BUFFERING
	magBuffIndicator = i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_M | (1<<7) ), magBuff1, magBuff2, 6);
	return magBuffIndicator;
#else
	return i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_M | (1<<7) ), magBuff, 6);
#endif
}

/**
 * @brief  Function to get the magnetic field strength on the x axis
 * @return Raw x axis magnetic field (register values)
 */
int16_t LSM303D::getMagXRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (magBuffIndicator == 1) {
		return magBuff1[1]<<8 | magBuff1[0];
	}
	else if (magBuffIndicator == 2) {
		return magBuff2[1]<<8 | magBuff2[0];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(magBuff[1]<<8 | magBuff[0]);
#endif
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
#if USE_DOUBLE_BUFFERING
	if (magBuffIndicator == 1) {
		return magBuff1[3]<<8 | magBuff1[2];
	}
	else if (magBuffIndicator == 2) {
		return magBuff2[3]<<8 | magBuff2[2];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(magBuff[3]<<8 | magBuff[2]);
#endif
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
#if USE_DOUBLE_BUFFERING
	if (magBuffIndicator == 1) {
		return magBuff1[5]<<8 | magBuff1[4];
	}
	else if (magBuffIndicator == 2) {
		return magBuff2[5]<<8 | magBuff2[4];
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(magBuff[5]<<8 | magBuff[4]);
#endif
}

/**
 * @brief  Function to get the magnetic field strength on the z axis
 * @return Z axis magnetic field (gauss)
 */
float LSM303D::getMagZ() {
	float retVal = (float)getMagZRaw() * magResolution;
	return retVal;
}


