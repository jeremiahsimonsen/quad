/**
 * @file LPS25H.h
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

#ifndef LPS25H_H_
#define LPS25H_H_

#include "I2C.h"

enum class LPS25H_Reg {
	REF_P_XL 		=	0x08,
	REF_P_L			=	0x09,
	REF_P_H			= 	0x0A,

	WHO_AM_I 		= 	0x0F,

	RES_CONF		=	0x10,

	CTRL_REG1		=	0x20,
	CTRL_REG2		=	0x21,
	CTRL_REG3		=	0x22,
	CTRL_REG4		=	0x23,

	INT_CFG			=	0x24,
	INT_SOURCE		=	0x25,

	STATUS_REG		=	0x27,

	PRESS_OUT_XL	=	0x28,
	PRESS_OUT_L		=	0x29,
	PRESS_OUT_H		= 	0x2A,

	TEMP_OUT_L		=	0x2B,
	TEMP_OUT_H		=	0x2C,

	FIFO_CTRL		=	0x2E,
	FIFO_STATUS		=	0x2F,

	THS_P_L			= 	0x30,
	THS_P_H			= 	0x31,

	RPDS_L			=	0x39,
	RPDS_H			=	0x3A
};

class LPS25H {
private:
	I2C *i2c;
	uint8_t address;

public:
	LPS25H(void);

	void enable(void);

	float readPressureInchesHg(void);
	int32_t readPressureRaw(void);

	float readTemperatureF(void);
	float reatTemperatureRaw(void);

	float pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg = 29.9213);
};

#endif


