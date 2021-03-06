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

/** @defgroup LSM303D LSM303D accelerometer/magnetometer chip-specific class
 *  @brief Module for interfacing with the LSM303D *
 *  @{
 */

#ifndef LSM303D_H_
#define LSM303D_H_

#include "I2C.h"
#include "logger.h"

#include "preFilter.h"
#include "preFilter2.h"
#include "preFilter3.h"
#include "preFilterFIR.h"
#include "preFilterAcc.h"

/**
 * @brief Register enumerations for the ST LSM303D accelerometer/magnetometer
 */
enum class LSM303D_Reg {
	TEMP_OUT_L		= 0x05,  //!< TEMP_OUT_L
	TEMP_OUT_H		= 0x06,  //!< TEMP_OUT_H

	STATUS_M		= 0x07,    //!< STATUS_M

	OUT_X_L_M 		= 0x08,  //!< OUT_X_L_M
	OUT_X_H_M 		= 0x09,  //!< OUT_X_H_M
	OUT_Y_L_M 		= 0x0A,  //!< OUT_Y_L_M
	OUT_Y_H_M 		= 0x0B,  //!< OUT_Y_H_M
	OUT_Z_L_M 		= 0x0C,  //!< OUT_Z_L_M
	OUT_Z_H_M 		= 0x0D,  //!< OUT_Z_H_M

	WHO_AM_I 		= 0x0F,   //!< WHO_AM_I

	INT_CTRL_M 		= 0x12, //!< INT_CTRL_M
	INT_SRC_M 		= 0x13,  //!< INT_SRC_M
	INT_THS_L_M 	= 0x14, //!< INT_THS_L_M
	INT_THS_H_M 	= 0x15, //!< INT_THS_H_M

	OFFSET_X_L_M 	= 0x16,//!< OFFSET_X_L_M
	OFFSET_X_H_M 	= 0x17,//!< OFFSET_X_H_M
	OFFSET_Y_L_M 	= 0x18,//!< OFFSET_Y_L_M
	OFFSET_Y_H_M 	= 0x19,//!< OFFSET_Y_H_M
	OFFSET_Z_L_M 	= 0x1A,//!< OFFSET_Z_L_M
	OFFSET_Z_H_M 	= 0x1B,//!< OFFSET_Z_H_M

	REFERENCE_X 	= 0x1C, //!< REFERENCE_X
	REFERENCE_Y 	= 0x1D, //!< REFERENCE_Y
	REFERENCE_Z 	= 0x1E, //!< REFERENCE_Z

	CTRL0 			= 0x1F,     //!< CTRL0
	CTRL1 			= 0x20,     //!< CTRL1
	CTRL2 			= 0x21,     //!< CTRL2
	CTRL3 			= 0x22,     //!< CTRL3
	CTRL4 			= 0x23,     //!< CTRL4
	CTRL5 			= 0x24,     //!< CTRL5
	CTRL6 			= 0x25,     //!< CTRL6
	CTRL7 			= 0x26,     //!< CTRL7

	STATUS_A 		= 0x27,   //!< STATUS_A

	OUT_X_L_A 		= 0x28,  //!< OUT_X_L_A
	OUT_X_H_A 		= 0x29,  //!< OUT_X_H_A
	OUT_Y_L_A 		= 0x2A,  //!< OUT_Y_L_A
	OUT_Y_H_A 		= 0x2B,  //!< OUT_Y_H_A
	OUT_Z_L_A 		= 0x2C,  //!< OUT_Z_L_A
	OUT_Z_H_A 		= 0x2D,  //!< OUT_Z_H_A

	FIFO_CTRL 		= 0x2E,  //!< FIFO_CTRL
	FIFO_SRC 		= 0x2F,   //!< FIFO_SRC

	IG_CFG1 		= 0x30,    //!< IG_CFG1
	IG_SRC1 		= 0x31,    //!< IG_SRC1
	IG_THS1 		= 0x32,    //!< IG_THS1
	IG_DUR1 		= 0x33,    //!< IG_DUR1
	IG_CFG2 		= 0x34,    //!< IG_CFG2
	IG_SRC2			= 0x35,    //!< IG_SRC2
	IG_THS2 		= 0x36,    //!< IG_THS2
	IG_DUR2 		= 0x37,    //!< IG_DUR2

	CLICK_CFG 		= 0x38,  //!< CLICK_CFG
	CLICK_SRC 		= 0x39,  //!< CLICK_SRC
	CLICK_THS 		= 0x3A,  //!< CLICK_THS

	TIME_LIMIT 		= 0x3B, //!< TIME_LIMIT
	TIME_LATENCY 	= 0x3C,//!< TIME_LATENCY
	TIME_WINDOW 	= 0x3D, //!< TIME_WINDOW

	Act_THS 		= 0x3E,    //!< Act_THS
	Act_DUR 		= 0x3F     //!< Act_DUR
};

/** @addtogroup LSM303D_Macros Register Access Macros
 *  @brief Macros for setting fields in registers
 *  @{
 */

/**
 * @brief BitField macros for CTRL0
 */
#if 1 // LSM303D CTRL0 BitFields
#define LSM303D_CTRL0_HPIS2_MASK		0x01u
#define LSM303D_CTRL0_HPIS2_SHIFT		0
#define LSM303D_CTRL0_HPIS2_WIDTH		1
#define LSM303D_CTRL0_HPIS2(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_HPIS2_SHIFT))&LSM303D_CTRL0_HPIS2_MASK)
#define LSM303D_CTRL0_HPIS1_MASK		0x02u
#define LSM303D_CTRL0_HPIS1_SHIFT		1
#define LSM303D_CTRL0_HPIS1_WIDTH		1
#define LSM303D_CTRL0_HPIS1(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_HPIS1_SHIFT))&LSM303D_CTRL0_HPIS1_MASK)
#define LSM303D_CTRL0_HP_Click_MASK		0x04u
#define LSM303D_CTRL0_HP_Click_SHIFT	2
#define LSM303D_CTRL0_HP_Click_WIDTH	1
#define LSM303D_CTRL0_HP_Click(x)		(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_HP_Click_SHIFT))&LSM303D_CTRL0_HP_Click_MASK)
#define LSM303D_CTRL0_FTH_EN_MASK		0x20u
#define LSM303D_CTRL0_FTH_EN_SHIFT		5
#define LSM303D_CTRL0_FTH_EN_WIDTH		1
#define LSM303D_CTRL0_FTH_EN(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_FTH_EN_SHIFT))&LSM303D_CTRL0_FTH_EN_MASK)
#define LSM303D_CTRL0_FIFO_EN_MASK		0x40u
#define LSM303D_CTRL0_FIFO_EN_SHIFT		6
#define LSM303D_CTRL0_FIFO_EN_WIDTH		1
#define LSM303D_CTRL0_FIFO_EN(x)		(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_FIFO_EN_SHIFT))&LSM303D_CTRL0_FIFO_EN_MASK)
#define LSM303D_CTRL0_BOOT_MASK			0x80u
#define LSM303D_CTRL0_BOOT_SHIFT		7
#define LSM303D_CTRL0_BOOT_WIDTH		1
#define LSM303D_CTRL0_BOOT(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL0_BOOT_SHIFT))&LSM303D_CTRL0_BOOT_MASK)
#endif

/**
 * @brief BitField macros for CTRL1
 */
#if 1 // LSM303D CTRL1 BitFields
#define LSM303D_CTRL1_AXEN_MASK			0x01u
#define LSM303D_CTRL1_AXEN_SHIFT		0
#define LSM303D_CTRL1_AXEN_WIDTH		1
#define LSM303D_CTRL1_AXEN(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL1_AXEN_SHIFT))&LSM303D_CTRL1_AXEN_MASK)
#define LSM303D_CTRL1_AYEN_MASK			0x02u
#define LSM303D_CTRL1_AYEN_SHIFT		1
#define LSM303D_CTRL1_AYEN_WIDTH		1
#define LSM303D_CTRL1_AYEN(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL1_AYEN_SHIFT))&LSM303D_CTRL1_AYEN_MASK)
#define LSM303D_CTRL1_AZEN_MASK			0x04u
#define LSM303D_CTRL1_AZEN_SHIFT		2
#define LSM303D_CTRL1_AZEN_WIDTH		1
#define LSM303D_CTRL1_AZEN(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL1_AZEN_SHIFT))&LSM303D_CTRL1_AZEN_MASK)
#define LSM303D_CTRL1_BDU_MASK			0x08u
#define LSM303D_CTRL1_BDU_SHIFT			3
#define LSM303D_CTRL1_BDU_WIDTH			1
#define LSM303D_CTRL1_BDU(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL1_BDU_SHIFT))&LSM303D_CTRL1_BDU_MASK)
#define LSM303D_CTRL1_AODR_MASK			0xF0u
#define LSM303D_CTRL1_AODR_SHIFT		4
#define LSM303D_CTRL1_AODR_WIDTH		4
#define LSM303D_CTRL1_AODR(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL1_AODR_SHIFT))&LSM303D_CTRL1_AODR_MASK)
#endif

/**
 * @brief BitField macros for CTRL2
 */
#if 1 // LSM303D CTRL2 BitFields
#define LSM303D_CTRL2_SIM_MASK			0x01u
#define LSM303D_CTRL2_SIM_SHIFT			0
#define LSM303D_CTRL2_SIM_WIDTH			1
#define LSM303D_CTRL2_SIM(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL2_SIM_SHIFT))&LSM303D_CTRL2_SIM_MASK)
#define LSM303D_CTRL2_AST_MASK			0x02u
#define LSM303D_CTRL2_AST_SHIFT			1
#define LSM303D_CTRL2_AST_WIDTH			1
#define LSM303D_CTRL2_AST(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL2_AST_SHIFT))&LSM303D_CTRL2_AST_MASK)
#define LSM303D_CTRL2_AFS_MASK			0x38u
#define LSM303D_CTRL2_AFS_SHIFT			3
#define LSM303D_CTRL2_AFS_WIDTH			3
#define LSM303D_CTRL2_AFS(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL2_AFS_SHIFT))&LSM303D_CTRL2_AFS_MASK)
#define LSM303D_CTRL2_ABW_MASK			0xC0u
#define LSM303D_CTRL2_ABW_SHIFT			6
#define LSM303D_CTRL2_ABW_WIDTH			2
#define LSM303D_CTRL2_ABW(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL2_ABW_SHIFT))&LSM303D_CTRL2_ABW_MASK)
#endif

// Don't currently care about CTRL3-4

/**
 * @brief BitField macros for CTRL5
 */
#if 1 // LSM303D CTRL5 BitFields
#define LSM303D_CTRL5_LIR1_MASK			0x01u
#define LSM303D_CTRL5_LIR1_SHIFT		0
#define LSM303D_CTRL5_LIR1_WIDTH		1
#define LSM303D_CTRL5_LIR1(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL5_LIR1_SHIFT))&LSM303D_CTRL5_LIR1_MASK)
#define LSM303D_CTRL5_LIR2_MASK			0x02u
#define LSM303D_CTRL5_LIR2_SHIFT		1
#define LSM303D_CTRL5_LIR2_WIDTH		1
#define LSM303D_CTRL5_LIR2(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL5_LIR2_SHIFT))&LSM303D_CTRL5_LIR2_MASK)
#define LSM303D_CTRL5_M_ODR_MASK		0x1Cu
#define LSM303D_CTRL5_M_ODR_SHIFT		2
#define LSM303D_CTRL5_M_ODR_WIDTH		3
#define LSM303D_CTRL5_M_ODR(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL5_M_ODR_SHIFT))&LSM303D_CTRL5_M_ODR_MASK)
#define LSM303D_CTRL5_M_RES_MASK		0x60u
#define LSM303D_CTRL5_M_RES_SHIFT		5
#define LSM303D_CTRL5_M_RES_WIDTH		2
#define LSM303D_CTRL5_M_RES(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL5_M_RES_SHIFT))&LSM303D_CTRL5_M_RES_MASK)
#define LSM303D_CTRL5_TEMP_EN_MASK		0x80u
#define LSM303D_CTRL5_TEMP_EN_SHIFT		7
#define LSM303D_CTRL5_TEMP_EN_WIDTH		1
#define LSM303D_CTRL5_TEMP_EN(x)		(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL5_TEMP_EN_SHIFT))&LSM303D_CTRL5_TEMP_EN_MASK)
#endif

/**
 * @brief BitField macros for CTRL6
 */
#if 1 // LSM303D CTRL6 BitFields
#define LSM303D_CTRL6_MFS_MASK			0x60u
#define LSM303D_CTRL6_MFS_SHIFT			5
#define LSM303D_CTRL6_MFS_WIDTH			2
#define LSM303D_CTRL6_MFS(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL6_MFS_SHIFT))&LSM303D_CTRL6_MFS_MASK)
#endif
/**
 * @brief BitField macros for CTRL7
 */
#if 1 // LSM303D CTRL7 BitFields
#define LSM303D_CTRL7_MD_MASK			0x03u
#define LSM303D_CTRL7_MD_SHIFT			0
#define LSM303D_CTRL7_MD_WIDTH			2
#define LSM303D_CTRL7_MD(x)				(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL7_MD_SHIFT))&LSM303D_CTRL7_MD_MASK)
#define LSM303D_CTRL7_MLP_MASK			0x04u
#define LSM303D_CTRL7_MLP_SHIFT			2
#define LSM303D_CTRL7_MLP_WIDTH			1
#define LSM303D_CTRL7_MLP(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL7_MLP_SHIFT))&LSM303D_CTRL7_MLP_MASK)
#define LSM303D_CTRL7_T_ONLY_MASK		0x10u
#define LSM303D_CTRL7_T_ONLY_SHIFT		4
#define LSM303D_CTRL7_T_ONLY_WIDTH		1
#define LSM303D_CTRL7_T_ONLY(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL7_T_ONLY_SHIFT))&LSM303D_CTRL7_T_ONLY_MASK)
#define LSM303D_CTRL7_AFDS_MASK			0x20u
#define LSM303D_CTRL7_AFDS_SHIFT		5
#define LSM303D_CTRL7_AFDS_WIDTH		1
#define LSM303D_CTRL7_AFDS(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL7_AFDS_SHIFT))&LSM303D_CTRL7_AFDS_MASK)
#define LSM303D_CTRL7_AHPM_MASK			0xC0u
#define LSM303D_CTRL7_AHPM_SHIFT		6
#define LSM303D_CTRL7_AHPM_WIDTH		2
#define LSM303D_CTRL7_AHPM(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL7_AHPM_SHIFT))&LSM303D_CTRL7_AHPM_MASK)
#endif

// Don't currently care about the other BitFields

/** @} Close LSM303D_Macros group */

/** @addtogroup LSM303D_Config Configuration Options
 *  @brief Configuration structures for setting sensor configuration
 *  @{
 */

/** Acceleration data rate configuration
 * 	  LSM_AODR_Config AODR3 AODR2 AODR1 AODR0 Power mode selection
 * 	  		1			0 	  0 	0 	  0 	 Power-down mode
 * 	  		2			0 	  0 	0 	  1 	 3.125 Hz
 * 	  		3			0 	  0 	1 	  0 	 6.25 Hz
 * 	  		4			0 	  0 	1 	  1 	 12.5 Hz
 * 	  		5			0 	  1 	0 	  0 	 25 Hz
 * 	  		6			0 	  1 	0 	  1 	 50 Hz
 * 	  		7			0 	  1 	1 	  0 	 100 Hz
 * 	  		8			0 	  1 	1 	  1 	 200 Hz
 * 	  		9			1 	  0 	0 	  0 	 400 Hz
 * 	  		10			1 	  0 	0 	  1 	 800 Hz
 * 	  		11			1 	  0 	1 	  0 	 1600 Hz
 */
enum class LSM_AODR_Config {
	ONE		=	0b0000,		// Power-down mode
	TWO		=	0b0001,		// 3.125 Hz
	THREE	=	0b0010,		// 6.25 Hz
	FOUR	=	0b0011,		// 12.5 Hz
	FIVE 	= 	0b0100,		// 25 Hz
	SIX		=	0b0101,		// 50 Hz
	SEVEN	=	0b0110,		// 100 Hz
	EIGHT	= 	0b0111,		// 200 Hz
	NINE	=	0b1000,		// 400 Hz
	TEN		=	0b1001,		// 800 Hz
	ELEVEN	=	0b1010,		// 1600 Hz
};

/** Acceleration anti-alias filter bandwidth configuration
 *	  LSM_ABW_Config ABW1 ABW0 Anti-alias filter bandwidth
 *	  		1		  0 	0 			773 Hz
 *	  		2		  0 	1 			194 Hz
 *	  		3		  1 	0 			362 Hz
 *	  		4		  1 	1 			50 Hz
 */
enum class LSM_ABW_Config {
	ONE		=	0b00,		// 773 Hz
	TWO		=	0b01,		// 194 Hz
	THREE	=	0b10,		// 362 Hz
	FOUR	=	0b11		// 50 Hz
};

/** Acceleration full-scale selection
 *	LSM_AFS_Config AFS2 AFS1 AFS0 Acceleration full-scale
 *		   TWO		  0 	0 	0 			+/- 2g
 *		  FOUR	  	  0 	0 	1 			+/- 4g
 *		   SIX		  0 	1 	0 			+/- 6g
 *		  EIGHT	  	  0 	1 	1 			+/- 8g
 *		 SIXTEEN	  1 	0 	0 			+/- 16g
 */
enum class LSM_AFS_Config {
	TWO		=	0b000,		// +/-2g
	FOUR	=	0b001,		// +/-4g
	SIX		=	0b010,		// +/-6g
	EIGHT	=	0b011,		// +/-8g
	SIXTEEN	=	0b100,		// +/-16g
};

/** Magnetometer output data rate configuration
 *	  LSM_MODR_Config MODR2 MODR1 MODR0 Power mode selection
 *	  		 1			0 	  0 	0 		  3.125 Hz
 *	  		 2			0 	  0 	1 		  6.25 Hz
 *	  		 3			0 	  1 	0 		  12.5 Hz
 *	  		 4			0 	  1 	1 		  25 Hz
 *	  		 5			1 	  0 	0 		  50 Hz
 *	  		 6			1 	  0 	1 		  100 Hz(1)
 *	  1. Available only for accelerometer ODR > 50 Hz or accelerometer in power-down mode
 */
enum class LSM_MODR_Config {
	ONE		=	0b000,		// 3.125 Hz
	TWO		=	0b001,		// 6.25 Hz
	THREE	=	0b010,		// 12.5 Hz
	FOUR	=	0b011,		// 25 Hz
	FIVE	=	0b100,		// 50 Hz
	SIX		=	0b101		// 100 Hz
};

/** Magnetometer resolution configuration
 *  LSM_MRES_Config M_RES1 M_RES0 Resolution
 *  	  LOW		  0		  0		 Low
 *  	  HIGH		  1		  1		 High
 */
enum class LSM_MRES_Config {
	LOW		=	0b00,		// Low resolution mode
	HIGH	=	0b11		// High resolution mode
};

/** LSM303D Magnetometer full-scale configuration
 *	LSM_MFS_Config MFS1 MFS0 Magnetic full-scale
 *		  TWO		0 	  0 	  +/- 2 gauss
 *		  FOUR		0 	  1 	  +/- 4 gauss
 *		  EIGHT		1 	  0 	  +/- 8 gauss
 *		  TWELVE	1 	  1 	  +/- 12 gauss
 */
enum class LSM_MFS_Config {
	TWO		=	0b00,		// +/- 2 gauss
	FOUR	=	0b01,		// +/- 4 gauss
	EIGHT	=	0b10,		// +/- 8 gauss
	TWELVE	=	0b11		// +/- 12 gauss
};

/** LSM303D Magnetic sensor mode configuration
 *	LSM_MD_Config MD1 MD0 Magnetic sensor mode
 *	 CONTINUOUS	   0   0 	Continuous-conversion mode
 *	   SINGLE 	   0   1 	Single-conversion mode
 *		OFF 	   1   0 	Power-down mode
 */
enum class LSM_MD_Config {
	CONTINUOUS	=	0b00,	// Continuous-conversion mode
	SINGLE		=	0b01,	// Single-conversion mode
	OFF			=	0b10	// Power-down mode
};

/**
 * Structure for initializing the LSM303D
 */
typedef struct {
	LSM_AODR_Config aodr_config;	///< Accelerometer output data rate setting
	LSM_ABW_Config	abw_config;		///< Accelerometer anti-alias bandwidth setting
	LSM_AFS_Config	afs_config;		///< Accelerometer full-scale setting
	LSM_MODR_Config modr_config;	///< Magnetometer output data rate setting
	LSM_MRES_Config mres_config;	///< Magnetometer resolution setting
	LSM_MFS_Config	mfs_config;		///< Magnetometer full-scale setting
	LSM_MD_Config	md_config;		///< Magnetic sensor mode setting
} LSM303D_InitStruct;

/** @} Close LSM303D_Config group */

/**
 * @brief Class for interfacing with the ST LSM303D 3-axis accelerometer and magnetometer
 *
 * This class is responsible for handling the i2c register reads/writes
 * necessary to communicate with the LSM303D accelerometer/magnetometer. All I/O is via
 * i2c.
 *
 * The acceleration in the X, Y, or Z direction can be returned.
 * The magnetic field strength on the X, Y, or Z axis can be returned.
 */
class LSM303D {
private:
	I2C *i2c;					///< I2C global instance
	preFilter2 ax;				///< Filter to pre-filter raw accelerometer X-axis data
	preFilter2 ay;				///< Filter to pre-filter raw accelerometer Y-axis data
	preFilter2 az;				///< Filter to pre-filter raw accelerometer Z-axis data

	logger *log;				///< Logger instance for gathering data

	float accResolution;		///< Accelerometer resolution setting
	float magResolution;		///< Magnetometer resolution setting

	uint8_t accBuff[6];			///< Accelerometer buffer

	uint8_t magBuff[6];			///< Magnetometer buffer

	uint8_t address;			///< Slave address of the chip

	void enable(LSM303D_InitStruct init);
	void accCalibrate(void);

	int16_t getAccXRaw(void);
	int16_t getAccYRaw(void);
	int16_t getAccZRaw(void);

	int16_t getMagXRaw(void);
	int16_t getMagYRaw(void);
	int16_t getMagZRaw(void);

	float accXOffset;
	float accYOffset;
	float accZOffset;

public:
	LSM303D();
	LSM303D(LSM303D_InitStruct);

	void read(void);

	void readAcc(void);
	float getAccX(void);
	float getAccY(void);
	float getAccZ(void);
	float getAccXFiltered(void);
	float getAccYFiltered(void);
	float getAccZFiltered(void);

	void readMag(void);
	float getMagX(void);
	float getMagY(void);
	float getMagZ(void);

};

#endif

/** @} Close LSM303D group */
/** @} Close IMU group */
/** @} Close Sensors Group */
