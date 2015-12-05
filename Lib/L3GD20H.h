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
 * Based in part on the l3g-arduino library from Pololu Corp. See license.txt for details
 *
 */

#ifndef L3GD20H_H_
#define L3GD20H_H_

#include "I2C.h"
#include "preFilter.h"
#include "preFilter2.h"

/**
 * Register enumerations for the ST L3GD20H gyro
 */
enum class L3GD20H_Reg {
	WHO_AM_I	=	0x0F,   //!< WHO_AM_I

	CTRL1		= 	0x20,    //!< CTRL1
	CTRL2		=	0x21,     //!< CTRL2
	CTRL3		=	0x22,     //!< CTRL3
	CTRL4		=	0x23,     //!< CTRL4
	CTRL5		=	0x24,     //!< CTRL5

	REFERENCE	=	0x25,  //!< REFERENCE

	OUT_TEMP	=	0x26,   //!< OUT_TEMP

	STATUS		= 	0x27,   //!< STATUS

	OUT_X_L		=	0x28,   //!< OUT_X_L
	OUT_X_H		=	0x29,   //!< OUT_X_H

	OUT_Y_L		=	0x2A,   //!< OUT_Y_L
	OUT_Y_H		=	0x2B,   //!< OUT_Y_H

	OUT_Z_L		=	0x2C,   //!< OUT_Z_L
	OUT_Z_H		=	0x2D,   //!< OUT_Z_H

	FIFO_CTRL	=	0x2E,  //!< FIFO_CTRL
	FIFO_SRC	=	0x2F,   //!< FIFO_SRC

	IG_CFG		=	0x30,    //!< IG_CFG
	IG_SRC		=	0x31,    //!< IG_SRC
	IG_THS_XH	=	0x32,  //!< IG_THS_XH
	IG_THS_XL	=	0x33,  //!< IG_THS_XL
	IG_THS_YH	=	0x34,  //!< IG_THS_YH
	IG_THS_YL	=	0x35,  //!< IG_THS_YL
	IG_THS_ZH	=	0x36,  //!< IG_THS_ZH
	IG_THS_ZL	=	0x37,  //!< IG_THS_ZL
	IG_DURATION	=	0x38,//!< IG_DURATION

	LOW_ODR		=	0x39    //!< LOW_ODR
};

/**
 * BitField macros for writable registers
 */
#if 1 // L3GD20H CTRL1 BitFields
#define L3GD_CTRL1_YEN_MASK		0x01u
#define L3GD_CTRL1_YEN_SHIFT	0
#define L3GD_CTRL1_YEN_WIDTH	1
#define L3GD_CTRL1_YEN(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_YEN_SHIFT))&L3GD_CTRL1_YEN_MASK)
#define L3GD_CTRL1_XEN_MASK		0x02u
#define L3GD_CTRL1_XEN_SHIFT	1
#define L3GD_CTRL1_XEN_WIDTH	1
#define L3GD_CTRL1_XEN(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_XEN_SHIFT))&L3GD_CTRL1_XEN_MASK)
#define L3GD_CTRL1_ZEN_MASK		0x04u
#define L3GD_CTRL1_ZEN_SHIFT	2
#define L3GD_CTRL1_ZEN_WIDTH	1
#define L3GD_CTRL1_ZEN(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_ZEN_SHIFT))&L3GD_CTRL1_ZEN_MASK)
#define L3GD_CTRL1_PD_MASK		0x08u
#define L3GD_CTRL1_PD_SHIFT		3
#define L3GD_CTRL1_PD_WIDTH		1
#define L3GD_CTRL1_PD(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_PD_SHIFT))&L3GD_CTRL1_PD_MASK)
#define L3GD_CTRL1_BW_MASK		0x30u
#define L3GD_CTRL1_BW_SHIFT		4
#define L3GD_CTRL1_BW_WIDTH		2
#define L3GD_CTRL1_BW(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_BW_SHIFT))&L3GD_CTRL1_BW_MASK)
#define L3GD_CTRL1_DR_MASK		0xC0u
#define L3GD_CTRL1_DR_SHIFT		6
#define L3GD_CTRL1_DR_WIDTH		2
#define L3GD_CTRL1_DR(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL1_DR_SHIFT))&L3GD_CTRL1_DR_MASK)
#endif

#if 1 // L3GD20H CTRL2 BitFields
#define L3GD_CTRL2_HPCF_MASK			0x0Fu
#define L3GD_CTRL2_HPCF_SHIFT			0
#define L3GD_CTRL2_HPCF_WIDTH			4
#define L3GD_CTRL2_HPCF(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL2_HPCF_SHIFT))&L3GD_CTRL2_HPCF_MASK)
#define L3GD_CTRL2_HPM_MASK				0x30u
#define L3GD_CTRL2_HPM_SHIFT			4
#define L3GD_CTRL2_HPM_WIDTH			2
#define L3GD_CTRL2_HPM(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL2_HPM_SHIFT))&L3GD_CTRL2_HPM_MASK)
#define L3GD_CTRL2_LVLen_MASK			0x40u
#define L3GD_CTRL2_LVLen_SHIFT			6
#define L3GD_CTRL2_LVLen_WIDTH			1
#define L3GD_CTRL2_LVLen(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL2_LVLen_SHIFT))&L3GD_CTRL2_LVLen_MASK)
#define L3GD_CTRL2_EXTRen_MASK			0x80u
#define L3GD_CTRL2_EXTRen_SHIFT			7
#define L3GD_CTRL2_EXTRen_WIDTH			1
#define L3GD_CTRL2_EXTRen(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL2_EXTRen_SHIFT))&L3GD_CTRL2_EXTRen_MASK)
#endif

#if 1 // L3GD20H CTRL3 BitFields
#define L3GD_CTRL3_INT2_Empty_MASK		0x01u
#define L3GD_CTRL3_INT2_Empty_SHIFT		0
#define L3GD_CTRL3_INT2_Empty_WIDTH		1
#define L3GD_CTRL3_INT2_Empty(x)		(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT2_Empty_SHIFT))&L3GD_CTRL3_INT2_Empty_MASK)
#define L3GD_CTRL3_INT2_ORun_MASK		0x02u
#define L3GD_CTRL3_INT2_ORun_SHIFT		1
#define L3GD_CTRL3_INT2_ORun_WIDTH		1
#define L3GD_CTRL3_INT2_ORun(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT2_ORun_SHIFT))&L3GD_CTRL3_INT2_ORun_MASK)
#define L3GD_CTRL3_INT2_FTH_MASK		0x04u
#define L3GD_CTRL3_INT2_FTH_SHIFT		2
#define L3GD_CTRL3_INT2_FTH_WIDTH		1
#define L3GD_CTRL3_INT2_FTH(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT2_FTH_SHIFT))&L3GD_CTRL3_INT2_FTH_MASK)
#define L3GD_CTRL3_INT2_DRDY_MASK		0x08u
#define L3GD_CTRL3_INT2_DRDY_SHIFT		3
#define L3GD_CTRL3_INT2_DRDY_WIDTH		1
#define L3GD_CTRL3_INT2_DRDY(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT2_DRDY_SHIFT))&L3GD_CTRL3_INT2_DRDY_MASK)
#define L3GD_CTRL3_PP_OD_MASK			0x10u
#define L3GD_CTRL3_PP_OD_SHIFT			4
#define L3GD_CTRL3_PP_OD_WIDTH			1
#define L3GD_CTRL3_PP_OD(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_PP_OD_SHIFT))&L3GD_CTRL3_PP_OD_MASK)
#define L3GD_CTRL3_H_Lactive_MASK		0x20u
#define L3GD_CTRL3_H_Lactive_SHIFT		5
#define L3GD_CTRL3_H_Lactive_WIDTH		1
#define L3GD_CTRL3_H_Lactive(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_H_Lactive_SHIFT))&L3GD_CTRL3_H_Lactive_MASK)
#define L3GD_CTRL3_INT1_Boot_MASK		0x40u
#define L3GD_CTRL3_INT1_Boot_SHIFT		6
#define L3GD_CTRL3_INT1_Boot_WIDTH		1
#define L3GD_CTRL3_INT1_Boot(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT1_Boot_SHIFT))&L3GD_CTRL3_INT1_Boot_MASK)
#define L3GD_CTRL3_INT1_IG_MASK			0x80u
#define L3GD_CTRL3_INT1_IG_SHIFT		7
#define L3GD_CTRL3_INT1_IG_WIDTH		1
#define L3GD_CTRL3_INT1_IG(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL3_INT1_IG_SHIFT))&L3GD_CTRL3_INT1_IG_MASK)
#endif

#if 1 // L3GD20H CTRL4 BitFields
#define L3GD_CTRL4_SIM_MASK				0x01u
#define L3GD_CTRL4_SIM_SHIFT			0
#define L3GD_CTRL4_SIM_WIDTH			1
#define L3GD_CTRL4_SIM(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_SIM_SHIFT))&L3GD_CTRL4_SIM_MASK)
#define L3GD_CTRL4_ST_MASK				0x06u
#define L3GD_CTRL4_ST_SHIFT				1
#define L3GD_CTRL4_ST_WIDTH				2
#define L3GD_CTRL4_ST(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_ST_SHIFT))&L3GD_CTRL4_ST_MASK)
#define L3GD_CTRL4_IMPen_MASK			0x08u
#define L3GD_CTRL4_IMPen_SHIFT			3
#define L3GD_CTRL4_IMPen_WIDTH			1
#define L3GD_CTRL4_IMPen(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_IMPen_SHIFT))&L3GD_CTRL4_IMPen_MASK)
#define L3GD_CTRL4_FS_MASK				0x30u
#define L3GD_CTRL4_FS_SHIFT				4
#define L3GD_CTRL4_FS_WIDTH				2
#define L3GD_CTRL4_FS(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_FS_SHIFT))&L3GD_CTRL4_FS_MASK)
#define L3GD_CTRL4_BLE_MASK				0x40u
#define L3GD_CTRL4_BLE_SHIFT			6
#define L3GD_CTRL4_BLE_WIDTH			1
#define L3GD_CTRL4_BLE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_BLE_SHIFT))&L3GD_CTRL4_BLE_MASK)
#define L3GD_CTRL4_BDU_MASK				0x80u
#define L3GD_CTRL4_BDU_SHIFT			7
#define L3GD_CTRL4_BDU_WIDTH			1
#define L3GD_CTRL4_BDU(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL4_BDU_SHIFT))&L3GD_CTRL4_BDU_MASK)
#endif

#if 1 // L3GD20H CTRL5 BitFields
#define L3GD_CTRL5_Out_Sel_MASK			0x03u
#define L3GD_CTRL5_Out_Sel_SHIFT		0
#define L3GD_CTRL5_Out_Sel_WIDTH		2
#define L3GD_CTRL5_Out_Sel(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_Out_Sel_SHIFT))&L3GD_CTRL5_Out_Sel_MASK)
#define L3GD_CTRL5_IG_Sel_MASK			0x0Cu
#define L3GD_CTRL5_IG_Sel_SHIFT			2
#define L3GD_CTRL5_IG_Sel_WIDTH			2
#define L3GD_CTRL5_IG_Sel(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_IG_Sel_SHIFT))&L3GD_CTRL5_IG_Sel_MASK)
#define L3GD_CTRL5_HPen_MASK			0x10u
#define L3GD_CTRL5_HPen_SHIFT			4
#define L3GD_CTRL5_HPen_WIDTH			1
#define L3GD_CTRL5_HPen(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_HPen_SHIFT))&L3GD_CTRL5_HPen_MASK)
#define L3GD_CTRL5_StopOnFTH_MASK		0x20u
#define L3GD_CTRL5_StopOnFTH_SHIFT		5
#define L3GD_CTRL5_StopOnFTH_WIDTH		1
#define L3GD_CTRL5_StopOnFTH(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_StopOnFTH_SHIFT))&L3GD_CTRL5_StopOnFTH_MASK)
#define L3GD_CTRL5_FIFO_EN_MASK			0x40u
#define L3GD_CTRL5_FIFO_EN_SHIFT		6
#define L3GD_CTRL5_FIFO_EN_WIDTH		1
#define L3GD_CTRL5_FIFO_EN(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_FIFO_EN_SHIFT))&L3GD_CTRL5_FIFO_EN_MASK)
#define L3GD_CTRL5_BOOT_MASK			0x80u
#define L3GD_CTRL5_BOOT_SHIFT			7
#define L3GD_CTRL5_BOOT_WIDTH			1
#define L3GD_CTRL5_BOOT(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_CTRL5_BOOT_SHIFT))&L3GD_CTRL5_BOOT_MASK)
#endif

#if 1 // L3GD20H STATUS BitFields
#define L3GD_STATUS_XDA_MASK			0x01u
#define L3GD_STATUS_XDA_SHIFT			0
#define L3GD_STATUS_XDA_WIDTH			1
#define L3GD_STATUS_XDA(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_XDA_SHIFT))&L3GD_STATUS_XDA_MASK)
#define L3GD_STATUS_YDA_MASK			0x02u
#define L3GD_STATUS_YDA_SHIFT			1
#define L3GD_STATUS_YDA_WIDTH			1
#define L3GD_STATUS_YDA(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_YDA_SHIFT))&L3GD_STATUS_YDA_MASK)
#define L3GD_STATUS_ZDA_MASK			0x04u
#define L3GD_STATUS_ZDA_SHIFT			2
#define L3GD_STATUS_ZDA_WIDTH			1
#define L3GD_STATUS_ZDA(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_ZDA_SHIFT))&L3GD_STATUS_ZDA_MASK)
#define L3GD_STATUS_ZYXDA_MASK			0x08u
#define L3GD_STATUS_ZYXDA_SHIFT			3
#define L3GD_STATUS_ZYXDA_WIDTH			1
#define L3GD_STATUS_ZYXDA(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_ZYXDA_SHIFT))&L3GD_STATUS_ZYXDA_MASK)
#define L3GD_STATUS_XOR_MASK			0x10u
#define L3GD_STATUS_XOR_SHIFT			4
#define L3GD_STATUS_XOR_WIDTH			1
#define L3GD_STATUS_XOR(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_XOR_SHIFT))&L3GD_STATUS_XOR_MASK)
#define L3GD_STATUS_YOR_MASK			0x20u
#define L3GD_STATUS_YOR_SHIFT			5
#define L3GD_STATUS_YOR_WIDTH			1
#define L3GD_STATUS_YOR(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_YOR_SHIFT))&L3GD_STATUS_YOR_MASK)
#define L3GD_STATUS_ZOR_MASK			0x40u
#define L3GD_STATUS_ZOR_SHIFT			6
#define L3GD_STATUS_ZOR_WIDTH			1
#define L3GD_STATUS_ZOR(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_ZOR_SHIFT))&L3GD_STATUS_ZOR_MASK)
#define L3GD_STATUS_ZYXOR_MASK			0x80u
#define L3GD_STATUS_ZYXOR_SHIFT			7
#define L3GD_STATUS_ZYXOR_WIDTH			1
#define L3GD_STATUS_ZYXOR(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_STATUS_ZYXOR_SHIFT))&L3GD_STATUS_ZYXOR_MASK)
#endif

#if 1 // L3GD20H FIFO_CTRL BitFields
#define L3GD_FIFO_CTRL_FTH_MASK			0x1Fu
#define L3GD_FIFO_CTRL_FTH_SHIFT		0
#define L3GD_FIFO_CTRL_FTH_WIDTH		5
#define L3GD_FIFO_CTRL_FTH(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_CTRL_FTH_SHIFT))&L3GD_FIFO_CTRL_FTH_MASK)
#define L3GD_FIFO_CTRL_FM_MASK			0xE0u
#define L3GD_FIFO_CTRL_FM_SHIFT			5
#define L3GD_FIFO_CTRL_FM_WIDTH			1
#define L3GD_FIFO_CTRL_FM(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_CTRL_FM_SHIFT))&L3GD_FIFO_CTRL_FM_MASK)
#endif

#if 1 // L3GD20H FIFO_SRC BitFields
#define L3GD_FIFO_SRC_FSS_MASK			0x1Fu
#define L3GD_FIFO_SRC_FSS_SHIFT			0
#define L3GD_FIFO_SRC_FSS_WIDTH			5
#define L3GD_FIFO_SRC_FSS(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_SRC_FSS_SHIFT))&L3GD_FIFO_SRC_FSS_MASK)
#define L3GD_FIFO_SRC_EMPTY_MASK		0x20u
#define L3GD_FIFO_SRC_EMPTY_SHIFT		5
#define L3GD_FIFO_SRC_EMPTY_WIDTH		1
#define L3GD_FIFO_SRC_EMPTY(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_SRC_EMPTY_SHIFT))&L3GD_FIFO_SRC_EMPTY_MASK)
#define L3GD_FIFO_SRC_OVRN_MASK			0x40u
#define L3GD_FIFO_SRC_OVRN_SHIFT		6
#define L3GD_FIFO_SRC_OVRN_WIDTH		1
#define L3GD_FIFO_SRC_OVRN(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_SRC_OVRN_SHIFT))&L3GD_FIFO_SRC_OVRN_MASK)
#define L3GD_FIFO_SRC_FTH_MASK			0x80u
#define L3GD_FIFO_SRC_FTH_SHIFT			7
#define L3GD_FIFO_SRC_FTH_WIDTH			1
#define L3GD_FIFO_SRC_FTH(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_SRC_FTH_SHIFT))&L3GD_FIFO_SRC_FTH_MASK)
#endif

#if 1 // L3GD20H IG_CFG BitFields
#define L3GD_IG_CFG_XLIE_MASK			0x01u
#define L3GD_IG_CFG_XLIE_SHIFT			0
#define L3GD_IG_CFG_XLIE_WIDTH			1
#define L3GD_IG_CFG_XLIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_XLIE_SHIFT))&L3GD_IG_CFG_XLIE_MASK)
#define L3GD_IG_CFG_XHIE_MASK			0x02u
#define L3GD_IG_CFG_XHIE_SHIFT			1
#define L3GD_IG_CFG_XHIE_WIDTH			1
#define L3GD_IG_CFG_XHIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_XHIE_SHIFT))&L3GD_IG_CFG_XHIE_MASK)
#define L3GD_IG_CFG_YLIE_MASK			0x04u
#define L3GD_IG_CFG_YLIE_SHIFT			2
#define L3GD_IG_CFG_YLIE_WIDTH			1
#define L3GD_IG_CFG_YLIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_YLIE_SHIFT))&L3GD_IG_CFG_YLIE_MASK)
#define L3GD_IG_CFG_YHIE_MASK			0x08u
#define L3GD_IG_CFG_YHIE_SHIFT			3
#define L3GD_IG_CFG_YHIE_WIDTH			1
#define L3GD_IG_CFG_YHIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_YHIE_SHIFT))&L3GD_IG_CFG_YHIE_MASK)
#define L3GD_IG_CFG_ZLIE_MASK			0x10u
#define L3GD_IG_CFG_ZLIE_SHIFT			4
#define L3GD_IG_CFG_ZLIE_WIDTH			1
#define L3GD_IG_CFG_ZLIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_ZLIE_SHIFT))&L3GD_IG_CFG_ZLIE_MASK)
#define L3GD_IG_CFG_ZHIE_MASK			0x20u
#define L3GD_IG_CFG_ZHIE_SHIFT			5
#define L3GD_IG_CFG_ZHIE_WIDTH			1
#define L3GD_IG_CFG_ZHIE(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_ZHIE_SHIFT))&L3GD_IG_CFG_ZHIE_MASK)
#define L3GD_IG_CFG_LIR_MASK			0x40u
#define L3GD_IG_CFG_LIR_SHIFT			6
#define L3GD_IG_CFG_LIR_WIDTH			1
#define L3GD_IG_CFG_LIR(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_LIR_SHIFT))&L3GD_IG_CFG_LIR_MASK)
#define L3GD_IG_CFG_ANDOR_MASK			0x80u
#define L3GD_IG_CFG_ANDOR_SHIFT			7
#define L3GD_IG_CFG_ANDOR_WIDTH			1
#define L3GD_IG_CFG_ANDOR(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_IG_CFG_ANDOR_SHIFT))&L3GD_IG_CFG_ANDOR_MASK)
#endif

#if 1 // L3GD20H IG_SRC BitFields
#define L3GD_IG_SRC_XL_MASK				0x01u
#define L3GD_IG_SRC_XL_SHIFT			0
#define L3GD_IG_SRC_XL_WIDTH			1
#define L3GD_IG_SRC_XL(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_XL_SHIFT))&L3GD_IG_SRC_XL_MASK)
#define L3GD_IG_SRC_XH_MASK				0x02u
#define L3GD_IG_SRC_XH_SHIFT			1
#define L3GD_IG_SRC_XH_WIDTH			1
#define L3GD_IG_SRC_XH(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_XH_SHIFT))&L3GD_IG_SRC_XH_MASK)
#define L3GD_IG_SRC_YL_MASK				0x04u
#define L3GD_IG_SRC_YL_SHIFT			2
#define L3GD_IG_SRC_YL_WIDTH			1
#define L3GD_IG_SRC_YL(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_YL_SHIFT))&L3GD_IG_SRC_YL_MASK)
#define L3GD_IG_SRC_YH_MASK				0x08u
#define L3GD_IG_SRC_YH_SHIFT			3
#define L3GD_IG_SRC_YH_WIDTH			1
#define L3GD_IG_SRC_YH(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_YH_SHIFT))&L3GD_IG_SRC_YH_MASK)
#define L3GD_IG_SRC_ZL_MASK				0x10u
#define L3GD_IG_SRC_ZL_SHIFT			4
#define L3GD_IG_SRC_ZL_WIDTH			1
#define L3GD_IG_SRC_ZL(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_ZL_SHIFT))&L3GD_IG_SRC_ZL_MASK)
#define L3GD_IG_SRC_ZH_MASK				0x20u
#define L3GD_IG_SRC_ZH_SHIFT			5
#define L3GD_IG_SRC_ZH_WIDTH			1
#define L3GD_IG_SRC_ZH(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_ZH_SHIFT))&L3GD_IG_SRC_ZH_MASK)
#define L3GD_IG_SRC_IA_MASK				0x40u
#define L3GD_IG_SRC_IA_SHIFT			6
#define L3GD_IG_SRC_IA_WIDTH			1
#define L3GD_IG_SRC_IA(x)				(((uint8_t)(((uint8_t)(x))<<L3GD_IG_SRC_IA_SHIFT))&L3GD_IG_SRC_IA_MASK)
#endif

#if 1 // L3GD20H LOW_ODR BitFields
#define L3GD_LOW_ODR_Low_ODR_MASK		0x01u
#define L3GD_LOW_ODR_Low_ODR_SHIFT		0
#define L3GD_LOW_ODR_Low_ODR_WIDTH		1
#define L3GD_LOW_ODR_Low_ODR(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_LOW_ODR_Low_ODR_SHIFT))&L3GD_LOW_ODR_Low_ODR_MASK)
#define L3GD_LOW_ODR_SW_RES_MASK		0x04u
#define L3GD_LOW_ODR_SW_RES_SHIFT		2
#define L3GD_LOW_ODR_SW_RES_WIDTH		1
#define L3GD_LOW_ODR_SW_RES(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_LOW_ODR_SW_RES_SHIFT))&L3GD_LOW_ODR_SW_RES_MASK)
#define L3GD_LOW_ODR_I2C_dis_MASK		0x08u
#define L3GD_LOW_ODR_I2C_dis_SHIFT		3
#define L3GD_LOW_ODR_I2C_dis_WIDTH		1
#define L3GD_LOW_ODR_I2C_dis(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_LOW_ODR_I2C_dis_SHIFT))&L3GD_LOW_ODR_I2C_dis_MASK)
#define L3GD_LOW_ODR_DRDY_HL_MASK		0x20u
#define L3GD_LOW_ODR_DRDY_HL_SHIFT		5
#define L3GD_LOW_ODR_DRDY_HL_WIDTH		1
#define L3GD_LOW_ODR_DRDY_HL(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_LOW_ODR_DRDY_HL_SHIFT))&L3GD_LOW_ODR_DRDY_HL_MASK)
#endif

/** DataRate and BandWidth configurations
 *	L3GD_ODR_BW_Config | Low_ODR(1) | DR <1:0> | BW <1:0> | ODR [Hz] | Cut-Off [Hz](2)
 *			1				 1 			 00 		00 		  12.5	 	  n.a.
 *							 1 			 00 		01 		  12.5	 	  n.a.
 *							 1 			 00 		10 		  12.5	 	  n.a.
 *							 1 			 00 		11 		  12.5	 	  n.a.
 *			2				 1 			 01 		00 		  25 		  n.a.
 *							 1 			 01 		01 		  25 		  n.a.
 *							 1 			 01 		10 		  25 		  n.a.
 *							 1 			 01 		11 		  25 		  n.a.
 *			3				 1 			 1X 		00 		  50 		  16.6
 *							 1 			 1X 		01 		  50 		  16.6
 *							 1 			 1X 		10 		  50 		  16.6
 *							 1 			 1X 		11 		  50 		  16.6
 *			4				 0 			 00 		00 		  100		  12.5
 *			5				 0 			 00 		01 		  100		  25
 *			6				 0 			 00 		10 		  100		  25
 *			7				 0 			 00 		11 		  100		  25
 *			8				 0 			 01 		00 		  200		  12.5
 *			9				 0 			 01 		01 		  200		  -
 *			10				 0 			 01 		10 		  200		  -
 *			11				 0 			 01 		11 		  200		  70
 *			12				 0 			 10 		00 		  400		  20
 *			13				 0 			 10 		01 		  400		  25
 *			14				 0 			 10 		10 		  400		  50
 *			15				 0 			 10 		11 		  400		  110
 *			16				 0 			 11 		00 		  800		  30
 *			17				 0 			 11 		01 		  800		  35
 *			18				 0 			 11 		10 		  800		  -
 *			19				 0 			 11 		11 		  800		  100
 *	1. this bit is described in register LOW_ODR (39h register).
 *	2. values in the table are indicative and they can vary proportionally with the specific ODR value.
 */
enum class L3GD_ODR_BW_Config {
	ONE			=	0b10000,	// ODR = 12.5 Hz	Cut-off = n.a.
	TWO			=	0b10100,	// ODR = 25 Hz		Cut-off = n.a.
	THREE		=	0b11000,	// ODR = 50 Hz		Cut-off = 16.6 Hz
	FOUR		=	0b00000,	// ODR = 100 Hz		Cut-off = 12.5 Hz
	FIVE		=	0b00001,	// ODR = 100 Hz		Cut-off = 25 Hz
	SIX			=	0b00010,	// ODR = 100 Hz		Cut-off = 25 Hz
	SEVEN		=	0b00011,	// ODR = 100 Hz		Cut-off = 25 Hz
	EIGHT		=	0b00100,	// ODR = 200 Hz		Cut-off = 12.5 Hz
	NINE		=	0b00101,	// ODR = 200 Hz		Cut-off = -
	TEN			=	0b00110,	// ODR = 200 Hz		Cut-off = -
	ELEVEN		=	0b00111,	// ODR = 200 Hz		Cut-off = 70 Hz
	TWELVE		=	0b01000,	// ODR = 400 Hz		Cut-off = 20 Hz
	THIRTEEN	=	0b01001,	// ODR = 400 Hz		Cut-off = 25 Hz
	FOURTEEN	=	0b01010,	// ODR = 400 Hz		Cut-off = 50 Hz
	FIFTEEN		=	0b01011,	// ODR = 400 Hz		Cut-off = 110 Hz
	SIXTEEN		=	0b01100,	// ODR = 800 Hz		Cut-off = 30 Hz
	SEVENTEEN	= 	0b01101,	// ODR = 800 Hz		Cut-off = 35 Hz
	EIGHTEEN	=	0b01110,	// ODR = 800 Hz		Cut-off = -
	NINETEEN	=	0b01111		// ODR = 800 Hz		Cut-off = 100 Hz
};

/**
 * Macros to access the bits corresponding to BitFields in the above configurations
 */
#define L3GD_ODR_BW_Config_LOW_ODR(x)	(((uint8_t)(((uint8_t)(x))>>4))&0b1)
#define L3GD_ODR_BW_Config_DR(x)		(((uint8_t)(((uint8_t)(x))>>2))&0b11)
#define L3GD_ODR_BW_Config_BW(x)		(((uint8_t)(((uint8_t)(x))))&0b11)

enum class L3GD_HPM_Config {
	ONE			=	0b00,		// Normal mode (reset reading REFERENCE register)
	TWO			=	0b01,		// Reference signal for filtering
	THREE		=	0b10,		// Normal mode
	FOUR		=	0b11		// Autoreset on interrupt event
};

enum class L3GD_HPCF_Config {
	// Cut-off frequency vs ODR	//	12.5	25		50		100		200		400		800
	ONE 		=	0b0000,		//	 1		 2		 4		  8		 15		 30		 56
	TWO			=	0b0001,		//	 0.5	 1		 2		  4		  8		 15		 30
	THREE		= 	0b0010,		//	 0.2	 0.5	 1		  2		  4		  8		 15
	FOUR		=	0b0011,		//	 0.1	 0.2	 0.5	  1		  2		  4		  8
	FIVE		=	0b0100,		//	 0.05	 0.1	 0.2	  0.5	  1		  2		  4
	SIX			=	0b0101,		//	 0.02	 0.05	 0.1	  0.2	  0.5	  1		  2
	SEVEN		=	0b0110,		//	 0.01	 0.02	 0.05	  0.1	  0.2	  0.5	  1
	EIGHT		=	0b0111,		//	 0.005	 0.01	 0.02	  0.05	  0.1	  0.2	  0.5
	NINE		=	0b1000,		//	 0.002	 0.005	 0.01	  0.02	  0.05	  0.1	  0.2
	TEN			=	0b1001,		//	 0.001	 0.002	 0.005	  0.01	  0.02	  0.05	  0.1
};

/**
 * Full-scale configurations
 */
enum class L3GD_FS_Config {
	LOW			=	0b00,		// FS = 245 dps -> resolution = 8.75 mdps/digit
	MEDIUM		= 	0b01,		// FS = 500 dps -> resolution = 17.50 mdps/digit
	HIGH		=	0b10,		// FS = 2000 dps -> resolution = 70.00 mdps/digit
};

/**
 * Structure for initializing the L3GD20H
 */
typedef struct {
	L3GD_ODR_BW_Config	odr_bw_config;
	L3GD_HPM_Config		hpm_config;
	L3GD_HPCF_Config	hpcf_config;
	L3GD_FS_Config		fs_config;
} L3GD20H_InitStruct;

class L3GD20H {
private:
	I2C *i2c;
	preFilter2 gx, gy, gz;

	float resolution;

#if USE_DOUBLE_BUFFERING
	uint8_t buffIndicator;	// Indicates which buffer is readable
	uint8_t gyroBuff1[6];	// Gyro angular velocity buffer 1
	uint8_t gyroBuff2[6];	// Gyro angular velocity buffer 2
#else
	uint8_t gyroBuff[6];	// Gyro angular velocity buffer
#endif

	TIM_HandleTypeDef TimHandle;
	uint32_t prevTick;
	uint32_t dt;

	float xOffset;
	float yOffset;
	float zOffset;
	uint8_t address;

	void enable(L3GD20H_InitStruct init);
	void calibrate(void);

	int16_t getXRaw(void);		// Roll
	int16_t getYRaw(void);		// Pitch
	int16_t getZRaw(void);		// Yaw

public:
	L3GD20H(void);
	L3GD20H(L3GD20H_InitStruct init);

	float getDT(void);

	int8_t read(void);

	float getX(void);		// Roll
	float getY(void);		// Pitch
	float getZ(void);		// Yaw
	float getXFiltered(void);
	float getYFiltered(void);
	float getZFiltered(void);
};

#endif





