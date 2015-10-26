/**
 * @file LPS25H.cpp
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

#include "I2C.h"
#include "LPS25H.h"

/**
 * @brief Instantiates an object using default I2C pins
 */
LPS25H::LPS25H(void) {
	address = 0b10111010;

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
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
#if USE_DOUBLE_BUFFERING
	uint8_t buffIndicator;

	buffIndicator = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::PRESS_OUT_XL | (1<<7) ), pressureBuff1, pressureBuff2, 3);

	if (buffIndicator == 1)
		return pressureBuff1[2] << 16 | pressureBuff1[1] << 8 | pressureBuff1[0];
	else if (buffIndicator == 2)
		return pressureBuff2[2] << 16 | pressureBuff2[1] << 8 | pressureBuff2[0];
	else
		return -1;
#else
	int8_t retVal = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::PRESS_OUT_XL | (1<<7) ), pressureBuff, 3);
	if (retVal < 0) {
		return retVal;
	}
	i2c->readyWait();
	return pressureBuff[2] << 16 | pressureBuff[1] << 8 | pressureBuff[0];
#endif
}

/**
 * @brief  Calculates temperature
 * @return Temperature in degrees Fahrenheit
 */
float LPS25H::readTemperatureF(void) {
	return 108.5f + (float)readTemperatureRaw() / 480.0f * 1.8f;
}

/**
 * @brief  Initiates a new temperature read
 * @return The latest complete raw temperature reading or -1 on error
 */
int16_t LPS25H::readTemperatureRaw(void) {
#if USE_DOUBLE_BUFFERING
	uint8_t buffIndicator;

	buffIndicator = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::TEMP_OUT_L | (1<<7) ), temperatureBuff1, temperatureBuff2, 2);

	if (buffIndicator == 1)
		return temperatureBuff1[1] << 8 | temperatureBuff1[0];
	else if (buffIndicator == 2)
		return temperatureBuff2[1] << 8 | temperatureBuff2[0];
	else
		return -1;
#else
	int8_t retVal = i2c->memRead(address, ( (uint8_t)LPS25H_Reg::TEMP_OUT_L | (1<<7) ), temperatureBuff, 2);
	if (retVal < 0) {
		return retVal;
	}
	i2c->readyWait();
	return (int16_t) (temperatureBuff[1] << 8 | temperatureBuff[0]);
#endif
}

float LPS25H::pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg) {
	// TODO: Implement
	return 0.0;
}





