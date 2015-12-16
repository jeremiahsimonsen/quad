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

/** @addtogroup LPS25H LPS25H altimeter chip-specific class
 *  @brief Module for interfacing with the LPS25H
 *  @{
 */

#ifndef LPS25H_H_
#define LPS25H_H_

#include "I2C.h"

/**
 * @brief LPS25H Pressure sensor device registers
 */
enum class LPS25H_Reg {
	REF_P_XL 		=	0x08,   //!< REF_P_XL
	REF_P_L			=	0x09,    //!< REF_P_L
	REF_P_H			= 	0x0A,   //!< REF_P_H

	WHO_AM_I 		= 	0x0F,  //!< WHO_AM_I

	RES_CONF		=	0x10,    //!< RES_CONF

	CTRL_REG1		=	0x20,   //!< CTRL_REG1
	CTRL_REG2		=	0x21,   //!< CTRL_REG2
	CTRL_REG3		=	0x22,   //!< CTRL_REG3
	CTRL_REG4		=	0x23,   //!< CTRL_REG4

	INT_CFG			=	0x24,    //!< INT_CFG
	INT_SOURCE		=	0x25,  //!< INT_SOURCE

	STATUS_REG		=	0x27,  //!< STATUS_REG

	PRESS_OUT_XL	=	0x28, //!< PRESS_OUT_XL
	PRESS_OUT_L		=	0x29, //!< PRESS_OUT_L
	PRESS_OUT_H		= 	0x2A,//!< PRESS_OUT_H

	TEMP_OUT_L		=	0x2B,  //!< TEMP_OUT_L
	TEMP_OUT_H		=	0x2C,  //!< TEMP_OUT_H

	FIFO_CTRL		=	0x2E,   //!< FIFO_CTRL
	FIFO_STATUS		=	0x2F, //!< FIFO_STATUS

	THS_P_L			= 	0x30,   //!< THS_P_L
	THS_P_H			= 	0x31,   //!< THS_P_H

	RPDS_L			=	0x39,     //!< RPDS_L
	RPDS_H			=	0x3A      //!< RPDS_H
};

/**
 * @brief Class for interfacing with the LPS25H pressure sensor
 *
 * This class is responsible for handling the i2c register reads/writes
 * necessary to communicate with the LPS25H pressure sensor. All I/O is
 * via i2c.
 *
 * The pressure and temperature can be measured using this class.
 */
class LPS25H {
private:
	I2C *i2c;						///< I2C global instance
	uint8_t address;				///< Slave address of the chip

	uint8_t pressureBuff[3];		///< Buffer to store pressure reading bytes
	uint8_t temperatureBuff[2];		///< Buffer to store temperature reading bytes

	void enable(void);

public:
	LPS25H(void);

	float readPressureMillibars(void);
	int32_t readPressureRaw(void);

	float readTemperatureF(void);
	int16_t readTemperatureRaw(void);
};

#endif

/** @} Close LPS25H group */
/** @} Close IMU group */
/** @} Close Sensors Group */
