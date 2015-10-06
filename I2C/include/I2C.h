/**
 * @file I2C.h
 *
 * @brief Low level i2c class
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 27, 2015
 *
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
//#include "I2C_Msp.h"

//BEGIN: I2C_MSP.h
#if 1
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
#endif
//END: I2C_MSP
#define I2C_USE_DMA

enum class i2cPin {
	PA8 = 0,// I2C3_SCL
	PB6,	// I2C1_SCL
	PB7,	// I2C1_SDA
	PB8, 	// I2C1_SCL
	PB9,	// I2C1_SDA
	PB10,	// I2C2_SCL
	PB11,	// I2C2_SDA
	PC9,	// I2C3_SDA
	PF0,	// I2C2_SDA
	PF1,	// I2C2_SCL
	PH4,	// I2C2_SCL
	PH5,	// I2C2_SDA
	PH7,	// I2C3_SCL
	PH8,	// I2C3_SDA
};

class I2C {
private:
//	I2C_HandleTypeDef *hi2c;

	i2cPin scl, sda;

public:
	I2C();
	I2C(i2cPin cl, i2cPin da);

	void write(uint16_t devAddr, uint8_t *pData, uint16_t size);
	void read(uint16_t devAddr, uint8_t *pData, uint16_t size);

	void memWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);
	void memRead(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);
};

extern "C"{
	extern I2C_HandleTypeDef i2cHandle;
};

#endif
