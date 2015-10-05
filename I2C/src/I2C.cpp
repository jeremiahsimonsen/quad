/**
 * @file I2C.cpp
 *
 * @brief Low level i2c class
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 27, 2015
 *
 */

#include <I2C.h>
#include "I2C_Msp.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

I2C::I2C() {
	scl = i2cPin::PB8;
	sda = i2cPin::PB9;

	initI2C((int)scl, (int)sda);
//	copyI2CHandle(hi2c);
}

I2C::I2C(i2cPin cl, i2cPin da) {
	// TODO: Add check to see if valid: scl is an scl pin and sda is an sda pin
	// TODO: Also check if scl and sda are on the same I2Cx
	scl = cl;
	sda = da;

	initI2C((int)scl, (int)sda);
//	copyI2CHandle(hi2c);
}

// TODO: Return an error value
void I2C::write(uint16_t devAddr, uint8_t *pData, uint16_t size) {
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Master_Transmit_DMA(&i2cHandle, devAddr, pData, size) != HAL_OK) {
		// return error;
	}
}

// TODO: Return an error value
void I2C::read(uint16_t devAddr, uint8_t *pData, uint16_t size) {
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Master_Receive_DMA(&i2cHandle, devAddr, pData, size) != HAL_OK) {
		// return error;
	}
}

// TODO: Return an error value
void I2C::memWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size) {
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
//	if (HAL_I2C_Mem_Write_DMA(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size) != HAL_OK) {
//		// return error;
//	}
	if (HAL_I2C_Mem_Write(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size, 1000) != HAL_OK) {
		while (1);
	}
}

// TODO: Return an error value
//uint8_t* I2C::memRead(uint16_t devAddr, uint16_t memAddr, uint8_t *pData1, uint8_t *pData2, uint16_t size) {
//	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
////	if (HAL_I2C_Mem_Read_DMA_Hacked(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData1, pData2, size) != HAL_OK) {
//	if (HAL_I2C_Mem_Read_DMA(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData1, size) != HAL_OK) {
//		// return error;
//	}
//	if ( (i2cHandle.hdmarx->Instance->CR & DMA_SxCR_CT) == 0) {
//		return pData1;
//	} else {
//		return pData2;
//	}
//}

void I2C::memRead(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size) {
	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Mem_Read(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size, 1000) != HAL_OK) {
		while(1);
	}
}



