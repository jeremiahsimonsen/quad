/**
 * @file L3GD20H.cpp
 *
 * @brief Class for interfacing with the ST L3GD20H 3-axis gyroscope
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 8, 2015
 *
 * Based on lps-arduino library from Pololu Corp. See license.txt for details
 *
 */

#include "L3GD20H.h"

/**
 * @brief Instantiates an object using default I2C pins and configures the sensor to default
 */
L3GD20H::L3GD20H(void) {
	address = 0b11010110;

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	L3GD20H_InitStruct init;
	init.odr_bw_config = L3GD_ODR_BW_Config::EIGHT;
	init.fs_config = L3GD_FS_Config::MEDIUM;
	resolution = 17.50e-3f;
	enable(init);
}

/**
 * @brief Configures the sensor according to the parameters
 * @param init Sensor configuration parameters
 */
L3GD20H::L3GD20H(L3GD20H_InitStruct init) {
	address = 0b11010110;

	switch(init.fs_config) {
	case L3GD_FS_Config::LOW	: resolution = 8.75e-3f;  break;
	case L3GD_FS_Config::MEDIUM	: resolution = 17.50e-3f; break;
	case L3GD_FS_Config::HIGH	: resolution = 70.00e-3f; break;
	default: break;
	}

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
	enable(init);
}

/**
 * @brief Configures the sensor operation
 */
void L3GD20H::enable(L3GD20H_InitStruct init) {
	// Set data rate and bandwidth; Enable sensor for 3-axis operation
	uint8_t buf = (uint8_t)(L3GD_CTRL1_DR(L3GD_ODR_BW_Config_DR(init.odr_bw_config))
					| L3GD_CTRL1_BW(L3GD_ODR_BW_Config_BW(init.odr_bw_config))
					| L3GD_CTRL1_PD_MASK
					| L3GD_CTRL1_ZEN_MASK
					| L3GD_CTRL1_YEN_MASK
					| L3GD_CTRL1_XEN_MASK);
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL1, &buf, 1);

	// Set low output data rate configuration
	buf = L3GD_LOW_ODR_Low_ODR(L3GD_ODR_BW_Config_LOW_ODR(init.odr_bw_config));
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::LOW_ODR, &buf, 1);

	// Set full scale
	buf = (uint8_t)(L3GD_CTRL4_FS(init.fs_config)
			| L3GD_CTRL4_BDU_MASK);
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL4, &buf, 1);
}

/**
 * @brief Initiates a read of all 3 axes
 * @return The return value of I2C::memRead()
 * 			DOUBLE BUFFERING: 1 or 2 means which buffer is readable, -1 means HAL_ERROR
 * 			ELSE: 0 on success, -1 on HAL_ERROR
 * 			Updates data in gyroBuff
 */
int8_t L3GD20H::read(void) {
#if USE_DOUBLE_BUFFERING
	buffIndicator = i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff1, gyroBuff2, 6);
	return buffIndicator;
#else
	return i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff, 6);
#endif
}

/**
 * @brief  Function to get the rate of angular rotation about the x axis (roll)
 * @return Raw rate of angular rotation about the x axis (roll) (register values)
 */
int16_t L3GD20H::getXRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (buffIndicator == 1)
		return (gyroBuff1[1]<<8 | gyroBuff1[0]);
	else if (buffIndicator == 2) {
		return (gyroBuff2[1]<<8 | gyroBuff2[0]);
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(gyroBuff[1]<<8 | gyroBuff[0]);
#endif
}

/**
 * @brief  Function to get the rate of angular rotation about the x axis (roll)
 * @return Rate of angular rotation about the x axis (roll) in degrees per second (dps)
 */
float L3GD20H::getX() {
	float retVal = (float)getXRaw() * resolution;
	return retVal;
}

/**
 * @brief  Function to get the rate of angular rotation about the y axis (pitch)
 * @return Raw rate of angular rotation about the y axis (pitch) (register values)
 */
int16_t L3GD20H::getYRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (buffIndicator == 1)
		return (gyroBuff1[3]<<8 | gyroBuff1[2]);
	else if (buffIndicator == 2) {
		return (gyroBuff2[3]<<8 | gyroBuff2[2]);
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(gyroBuff[3]<<8 | gyroBuff[2]);
#endif
}

/**
 * @brief  Function to get the rate of angular rotation about the y axis (pitch)
 * @return Rate of angular rotation about the y axis (pitch) in degrees per second (dps)
 */
float L3GD20H::getY() {
	float retVal = (float)getYRaw() * resolution;
	return retVal;
}

/**
 * @brief  Function to get the rate of angular rotation about the z axis (yaw)
 * @return Raw rate of angular rotation about the z axis (yaw) (register values)
 */
int16_t L3GD20H::getZRaw(void) {
#if USE_DOUBLE_BUFFERING
	if (buffIndicator == 1)
		return (gyroBuff1[5]<<8 | gyroBuff1[4]);
	else if (buffIndicator == 2) {
		return (gyroBuff2[5]<<8 | gyroBuff2[4]);
	} else {
		return 0;
	}
#else
	i2c->readyWait();
	return (int16_t)(gyroBuff[5]<<8 | gyroBuff[4]);
#endif
}

/**
 * @brief  Function to get the rate of angular rotation about the z axis (yaw)
 * @return Rate of angular rotation about the z axis (yaw) in degrees per second (dps)
 */
float L3GD20H::getZ() {
	float retVal = (float)getZRaw() * resolution;
	return retVal;
}
