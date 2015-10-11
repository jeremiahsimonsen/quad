/**
 * @file LSM303D.h
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

#ifndef LSM303D_H_
#define LSM303D_H_

#include "I2C.h"

enum class LSM303D_Reg {
	TEMP_OUT_L		= 0x05,
	TEMP_OUT_H		= 0x06,

	STATUS_M		= 0x07,

	OUT_X_L_M 		= 0x08,
	OUT_X_H_M 		= 0x09,
	OUT_Y_L_M 		= 0x0A,
	OUT_Y_H_M 		= 0x0B,
	OUT_Z_L_M 		= 0x0C,
	OUT_Z_H_M 		= 0x0D,

	WHO_AM_I 		= 0x0F,

	INT_CTRL_M 		= 0x12,
	INT_SRC_M 		= 0x13,
	INT_THS_L_M 	= 0x14,
	INT_THS_H_M 	= 0x15,

	OFFSET_X_L_M 	= 0x16,
	OFFSET_X_H_M 	= 0x17,
	OFFSET_Y_L_M 	= 0x18,
	OFFSET_Y_H_M 	= 0x19,
	OFFSET_Z_L_M 	= 0x1A,
	OFFSET_Z_H_M 	= 0x1B,

	REFERENCE_X 	= 0x1C,
	REFERENCE_Y 	= 0x1D,
	REFERENCE_Z 	= 0x1E,

	CTRL0 			= 0x1F,
	CTRL1 			= 0x20,
	CTRL2 			= 0x21,
	CTRL3 			= 0x22,
	CTRL4 			= 0x23,
	CTRL5 			= 0x24,
	CTRL6 			= 0x25,
	CTRL7 			= 0x26,

	STATUS_A 		= 0x27,

	OUT_X_L_A 		= 0x28,
	OUT_X_H_A 		= 0x29,
	OUT_Y_L_A 		= 0x2A,
	OUT_Y_H_A 		= 0x2B,
	OUT_Z_L_A 		= 0x2C,
	OUT_Z_H_A 		= 0x2D,

	FIFO_CTRL 		= 0x2E,
	FIFO_SRC 		= 0x2F,

	IG_CFG1 		= 0x30,
	IG_SRC1 		= 0x31,
	IG_THS1 		= 0x32,
	IG_DUR1 		= 0x33,
	IG_CFG2 		= 0x34,
	IG_SRC2			= 0x35,
	IG_THS2 		= 0x36,
	IG_DUR2 		= 0x37,

	CLICK_CFG 		= 0x38,
	CLICK_SRC 		= 0x39,
	CLICK_THS 		= 0x3A,

	TIME_LIMIT 		= 0x3B,
	TIME_LATENCY 	= 0x3C,
	TIME_WINDOW 	= 0x3D,

	Act_THS 		= 0x3E,
	Act_DUR 		= 0x3F
};

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

#if 1 // LSM303D CTRL6 BitFields
#define LSM303D_CTRL6_MFS_MASK			0x60u
#define LSM303D_CTRL6_MFS_SHIFT			5
#define LSM303D_CTRL6_MFS_WIDTH			2
#define LSM303D_CTRL6_MFS(x)			(((uint8_t)(((uint8_t)(x))<<LSM303D_CTRL6_MFS_SHIFT))&LSM303D_CTRL6_MFS_MASK)
#endif

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

class LSM303D {
private:
	I2C *i2c;
	uint8_t address;

	float accResolution;
	uint8_t accBuffIndicator;	// Indicates which buffer is readable
	uint8_t accBuff1[6];		// Accelerometer buffer 1
	uint8_t accBuff2[6];		// Accelerometer buffer 2

	float magResolution;
	uint8_t magBuffIndicator;	// Indicates which buffer is readable
	uint8_t magBuff1[6];		// Magnetometer buffer 1
	uint8_t magBuff2[6];		// Magnetometer buffer 2

	// TODO: Make this configurable; Probably pass an initialization struct to constructor
	void enable(void);

public:
	LSM303D();

	void read(void);

	void readAcc(void);
	int16_t getAccX(void);
	int16_t getAccY(void);
	int16_t getAccZ(void);

	void readMag(void);
	int16_t getMagX(void);
	int16_t getMagY(void);
	int16_t getMagZ(void);

};

#endif
