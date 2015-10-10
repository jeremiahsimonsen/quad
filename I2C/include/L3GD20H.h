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

// CTRL1 = DR1 DR0 BW1 BW0 PD ZEN XEN YEN
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

// CTRL2 = EXTRen | LVLen | HPM[1:0] | HPCF[3:0]
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

// CTRL3 = INT1_IG | INT1_Boot | H_Lactive | PP_OD | INT2_DRDY | INT2_FTH | INT2_ORun | INT2_Empty
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

// CTRL4 = BDU | BLE | FS1 | FS0 | IMPen | ST2 | ST1 | SIM
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

// CTRL5 = BOOT | FIFO_EN | StopOnFTH | HPen | IG_Sel1 | IG_Sel0 | Out_Sel1 | Out_Sel0
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

// STATUS = ZYXOR | ZOR | YOR | XOR | ZYXDA | ZDA | YDA | XDA
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

// FIFO_CTRL = FM2 | FM1 | FM0 | FTH4 | FTH3 | FTH2 | FTH1 | FTH0
#define L3GD_FIFO_CTRL_FTH_MASK			0x1Fu
#define L3GD_FIFO_CTRL_FTH_SHIFT		0
#define L3GD_FIFO_CTRL_FTH_WIDTH		5
#define L3GD_FIFO_CTRL_FTH(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_CTRL_FTH_SHIFT))&L3GD_FIFO_CTRL_FTH_MASK)
#define L3GD_FIFO_CTRL_FM_MASK			0xE0u
#define L3GD_FIFO_CTRL_FM_SHIFT			5
#define L3GD_FIFO_CTRL_FM_WIDTH			1
#define L3GD_FIFO_CTRL_FM(x)			(((uint8_t)(((uint8_t)(x))<<L3GD_FIFO_CTRL_FM_SHIFT))&L3GD_FIFO_CTRL_FM_MASK)

// FIFO_SRC = FTH | OVRN | EMPTY | FSS[4:0]
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

// IG_CFG = ANDOR | LIR | ZHIE | ZLIE | YHIE | YLIE | XHIE | XLIE
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

// IG_SRC = 0 | IA | ZH | ZL | YH | YL | XH | XL
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

// LOW_ODR = -- | -- DRDY_HL | 0 | I2C_dis | SW_RES | 0 | Low_ODR
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

/* DR and BW configuration
 *	Low_ODR(1) | DR <1:0> | BW <1:0> | ODR [Hz] | Cut-Off [Hz](2)
 *		1 			00 		   00 		 12.5	 	  n.a.
 *		1 			00 		   01 		 12.5	 	  n.a.
 *		1 			00 		   10 		 12.5	 	  n.a.
 *		1 			00 		   11 		 12.5	 	  n.a.
 *		1 			01 		   00 		 25 		  n.a.
 *		1 			01 		   01 		 25 		  n.a.
 *		1 			01 		   10 		 25 		  n.a.
 *		1 			01 		   11 		 25 		  n.a.
 *		1 			1X 		   00 		 50 		  16.6
 *		1 			1X 		   01 		 50 		  16.6
 *		1 			1X 		   10 		 50 		  16.6
 *		1 			1X 		   11 		 50 		  16.6
 *		0 			00 		   00 		 100		  12.5
 *		0 			00 		   01 		 100		  25
 *		0 			00 		   10 		 100		  25
 *		0 			00 		   11 		 100		  25
 *		0 			01 		   00 		 200		  12.5
 *		0 			01 		   01 		 200		  -
 *		0 			01 		   10 		 200		  -
 *		0 			01 		   11 		 200		  70
 *		0 			10 		   00 		 400		  20
 *		0 			10 		   01 		 400		  25
 *		0 			10 		   10 		 400		  50
 *		0 			10 		   11 		 400		  110
 *		0 			11 		   00 		 800		  30
 *		0 			11 		   01 		 800		  35
 *		0 			11 		   10 		 800		  -
 *		0 			11 		   11 		 800		  100
 *	1. this bit is described in register LOW_ODR (39h register).
 *	2. values in the table are indicative and they can vary proportionally with the specific ODR value.
 */

typedef struct {

} L3GD20H_InitStruct;

class L3GD20H {
private:
	I2C *i2c;
	uint8_t address;

	uint8_t buffIndicator;	// Indicates which buffer is readable
	uint8_t gyroBuff1[6];	// Gyro angular velocity buffer 1
	uint8_t gyroBuff2[6];	// Gyro angular velocity buffer 2

	// TODO: Make this configurable; Probably pass an initialization struct to constructor
	void enable(void);

public:
	L3GD20H(void);

	void read(void);
	// TODO: Change getters to return rates in dps
	int16_t getX(void);		// Roll
	int16_t getY(void);		// Pitch
	int16_t getZ(void);		// Yaw
};

#endif
