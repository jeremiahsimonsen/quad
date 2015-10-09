/**
 * @file L3GD20H.h
 *
 * @brief Class for interfacing with the ST L3GD20H 3-axis gyroscope
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 8, 2015
 *
 * Based on l3g-arduino library from Pololu Corp. See license.txt for details
 *
 */

#ifndef L3GD20H_H_
#define L3GD20H_H_

#include "I2C.h"

enum class L3GD20H_Reg {
	WHO_AM_I	=	0x0F,

	CTRL1		= 	0x20,
	CTRL2		=	0x21,
	CTRL3		=	0x22,
	CTRL4		=	0x23,
	CTRL5		=	0x24,

	REFERENCE	=	0x25,

	OUT_TEMP	=	0x26,

	STATUS		= 	0x27,

	OUT_X_L		=	0x28,
	OUT_X_H		=	0x29,

	OUT_Y_L		=	0x2A,
	OUT_Y_H		=	0x2B,

	OUT_Z_L		=	0x2C,
	OUT_Z_H		=	0x2D,

	FIFO_CTRL	=	0x2E,
	FIFO_SRC	=	0x2F,

	IG_CFG		=	0x30,
	IG_SRC		=	0x31,
	IG_THS_XH	=	0x32,
	IG_THS_XL	=	0x33,
	IG_THS_YH	=	0x34,
	IG_THS_YL	=	0x35,
	IG_THS_ZH	=	0x36,
	IG_THS_ZL	=	0x37,
	IG_DURATION	=	0x38,

	LOW_ODR		=	0x39
};

class L3GD20H {
private:
	I2C *i2c;
	uint8_t address;

	uint8_t buffIndicator;	// Indicates which buffer is readable
	uint8_t gyroBuff1[6];	// Gyro angular velocity buffer 1
	uint8_t gyroBuff2[6];	// Gyro angular velocity buffer 2

public:
	L3GD20H(void);

	void enable(void);

	void read(void);
	int16_t getX(void);
	int16_t getY(void);
	int16_t getZ(void);
};

#endif
