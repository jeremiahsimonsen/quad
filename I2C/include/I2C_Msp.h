/**
 * @file I2C_Msp.h
 *
 * @brief MSP functions and interrupts for I2C
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 29, 2015
 *
 */

#ifndef _I2C_MSP_H_
#define _I2C_MSP_H_

//#ifdef __cplusplus
//  extern "C" {
//#endif

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

/*
 * Definitions for I2C1
 */
#define I2C1_TX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2C1_TX_DMA_STREAM              DMA1_Stream6
#define I2C1_RX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2C1_RX_DMA_STREAM              DMA1_Stream5

#define I2C1_DMA_TX_IRQn                DMA1_Stream6_IRQn
#define I2C1_DMA_RX_IRQn                DMA1_Stream5_IRQn
#define I2C1_DMA_TX_IRQHandler          DMA1_Stream6_IRQHandler
#define I2C1_DMA_RX_IRQHandler          DMA1_Stream5_IRQHandler

/*
 * Definitions for I2C2
 */
#define I2C2_TX_DMA_CHANNEL             DMA_CHANNEL_7
#define I2C2_TX_DMA_STREAM              DMA1_Stream7
#define I2C2_RX_DMA_CHANNEL             DMA_CHANNEL_7
#define I2C2_RX_DMA_STREAM              DMA1_Stream3

#define I2C2_DMA_TX_IRQn                DMA1_Stream7_IRQn
#define I2C2_DMA_RX_IRQn                DMA1_Stream3_IRQn
#define I2C2_DMA_TX_IRQHandler          DMA1_Stream7_IRQHandler
#define I2C2_DMA_RX_IRQHandler          DMA1_Stream3_IRQHandler

/*
 * Definitions for I2C3
 */
#define I2C3_TX_DMA_CHANNEL             DMA_CHANNEL_3
#define I2C3_TX_DMA_STREAM              DMA1_Stream4
#define I2C3_RX_DMA_CHANNEL             DMA_CHANNEL_3
#define I2C3_RX_DMA_STREAM              DMA1_Stream2

#define I2C3_DMA_TX_IRQn                DMA1_Stream4_IRQn
#define I2C3_DMA_RX_IRQn                DMA1_Stream2_IRQn
#define I2C3_DMA_TX_IRQHandler          DMA1_Stream4_IRQHandler
#define I2C3_DMA_RX_IRQHandler          DMA1_Stream2_IRQHandler

void initI2C(int scl, int sda);
void deInitI2C(int scl, int sda);

void I2C_MemWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);
void I2C_Mem_Read(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);

extern "C"{
	extern I2C_HandleTypeDef i2cHandle;
};

//#ifdef __cplusplus
//  }
//#endif

#endif
