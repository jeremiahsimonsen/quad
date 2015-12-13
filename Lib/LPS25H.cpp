/**
 * @file
 *
 * @brief Class for interfacing with the LPS25H pressure sensor
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 5, 2015
 *
 * Based in part on lps-arduino library from Pololu Corp. See license.txt for details
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup IMU
 *  @{
 */

/** @addtogroup LPS25H
 *  @{
 */

#include "I2C.h"
#include "LPS25H.h"

/**
 * @brief Instantiates an object using default I2C pins
 */
LPS25H::LPS25H(void) {
	// Initialize member
	address = 0b10111010;

	// Get a pointer to the I2C instance
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Enable and configure the altimeter
	enable();
}

/**
 * @brief Powers on the sensor and sets ODR to 12.5 Hz with BDU enabled
 */
void LPS25H::enable(void) {
	uint8_t buff = 0xB4;

	i2c->memWrite(address, (uint8_t)LPS25H_Reg::CTRL_REG1, &buff, 1);
}

/**
 * @brief  Calculates pressure
 * @return Pressure in millibars (mbar)
 */
float LPS25H::readPressureMillibars(void) {
	return (float)readPressureRaw() / 4096.0f;
}

/**
 * @brief  Initiates a new pressure read
 * @return The latest complete raw pressure reading or -1 on error
 */
int32_t LPS25H::readPressureRaw(void) {
	// Initiate the i2c read
	int8_t retVal = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::PRESS_OUT_XL | (1<<7) ), pressureBuff, 3);

	// Check for errors
	if (retVal < 0) {
		return retVal;
	}

	// Wait for the read to complete
	i2c->readyWait();

	// Return the raw pressure value
	return pressureBuff[2] << 16 | pressureBuff[1] << 8 | pressureBuff[0];
}

/**
 * @brief  Calculates temperature
 * @return Temperature in degrees Fahrenheit
 */
float LPS25H::readTemperatureF(void) {
	// Formula from the datasheet
	return 108.5f + (float)readTemperatureRaw() / 480.0f * 1.8f;
}

/**
 * @brief  Initiates a new temperature read
 * @return The latest complete raw temperature reading or -1 on error
 */
int16_t LPS25H::readTemperatureRaw(void) {
	// Initiate the i2c read
	int8_t retVal = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::TEMP_OUT_L | (1<<7) ), temperatureBuff, 2);

	// Check for errors
	if (retVal < 0) {
		return retVal;
	}

	// Wait for the read to complete
	i2c->readyWait();

	// Return the raw temperature value
	return (int16_t) (temperatureBuff[1] << 8 | temperatureBuff[0]);
}

/** @} Close LPS25H group */
/** @} Close IMU group */
/** @} Close Sensors Group */



