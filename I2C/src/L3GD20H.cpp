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
 * @brief Instantiates an object using default I2C pins
 */
L3GD20H::L3GD20H(void) {
	address = 0b11010110;

	i2c = I2C::Instance(i2cPin::PB6, i2cPin::PB9);
	enable();
}

/**
 * @brief Enables the sensor and sets default settings
 */
void L3GD20H::enable(void) {
	uint8_t buf = 0x0;

	// Low speed ODR disabled
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::LOW_ODR, &buf, 1);

	// Full-scale of 500 dps
	buf = 0x10;
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL4, &buf, 1);

	// Set data rate to 200 Hz (DR=01)
	// 	   bandwidth to 12.5 Hz (BW=00)
	//     enable sensor (PD = 1)
	//     enable all axes (Zen = Yen = Xen = 1)
	buf = 0b01001111;
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL1, &buf, 1);
}

/**
 * @brief Initiates a read of all 3 axes
 */
void L3GD20H::read(void) {
	buffIndicator = i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff1, gyroBuff2, 6);
}

/**
 * @brief  Function to get the raw angular rate about the x axis
 * @return Raw x-axis gyro rate
 */
int16_t L3GD20H::getX(void) {
	if (buffIndicator == 1)
		return (int16_t) (gyroBuff1[1]<<8 | gyroBuff1[0]);
	else if (buffIndicator == 2) {
		return (int16_t) (gyroBuff2[1]<<8 | gyroBuff2[0]);
	} else {
		return 0;
	}
}

/**
 * @brief  Function to get the raw angular rate about the y axis
 * @return Raw y-axis gyro rate
 */
int16_t L3GD20H::getY(void) {
	if (buffIndicator == 1)
		return (int16_t) (gyroBuff1[3]<<8 | gyroBuff1[2]);
	else if (buffIndicator == 2) {
		return (int16_t) (gyroBuff2[3]<<8 | gyroBuff2[2]);
	} else {
		return 0;
	}
}

/**
 * @brief  Function to get the raw angular rate about the z axis
 * @return Raw z-axis gyro rate
 */
int16_t L3GD20H::getZ(void) {
	if (buffIndicator == 1)
		return (int16_t) (gyroBuff1[5]<<8 | gyroBuff1[4]);
	else if (buffIndicator == 2) {
		return (int16_t) (gyroBuff2[5]<<8 | gyroBuff2[4]);
	} else {
		return 0;
	}
}


