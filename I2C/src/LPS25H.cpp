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
 * Based on lps-arduino library from Pololu Corp. See license.txt for details
 *
 */

#include "I2C.h"
#include "LPS25H.h"

LPS25H::LPS25H(void) {
	address = 0b10111010;
	// TODO: Init i2c somehow...
	enable();
}

void LPS25H::enable(void) {
	uint8_t buff = 0xB4;

	i2c->memWrite(address, (uint8_t)LPS25H_Reg::CTRL_REG1, &buff, 1);
}

float LPS25H::readPressureInchesHg(void) {

}

int32_t LPS25H::readPressureRaw(void) {

}

float LPS25H::readTemperatureF(void) {

}

float LPS25H::reatTemperatureRaw(void) {

}

float LPS25H::pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg) {

}





