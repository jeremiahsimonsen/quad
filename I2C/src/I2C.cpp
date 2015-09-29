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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

I2C::I2C() {
	scl = i2cPin::PB8;
	sda = i2cPin::PB9;

	initI2C();
}

I2C::I2C(i2cPin scl, i2cPin sda) {
	// TODO: Add check to see if valid scl is an scl pin and sda is an sda pin
	// TODO: Also check if scl and sda are on the same I2Cx
	this->scl = scl;
	this->sda = sda;

	initI2C();
}

void I2C::initI2C() {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_TypeDef *i2cPinToGPIO_TD[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOH};
	uint16_t i2cPinToGpioPin[] = {GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8};
	I2C_TypeDef *i2cPinToI2C_TD[] = {I2C3, I2C1, I2C1, I2C1, I2C1, I2C2, I2C2, I2C3, I2C2, I2C2, I2C2, I2C2, I2C3, I2C3};

	GPIO_TypeDef *SCL_PORT = i2cPinToGPIO_TD[(int)scl];
	GPIO_TypeDef *SDA_PORT = i2cPinToGPIO_TD[(int)sda];
	uint16_t SCL_PIN = i2cPinToGpioPin[(int)scl];
	uint16_t SDA_PIN = i2cPinToGpioPin[(int)sda];
	I2C_TypeDef I2Cx = i2cPinToI2C_TD[(int)scl];

	// Enable the appropriate GPIO clocks
	switch((uint32_t)SCL_PORT) {
	case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
	case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
	case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
	case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
	case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
	case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
	case (uint32_t)GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
	case (uint32_t)GPIOI: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
	default: break;
	}
	if (SCL_PORT != SDA_PORT) {
		switch((uint32_t)SDA_PORT) {
		case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
		case (uint32_t)GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
		case (uint32_t)GPIOI: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
		default: break;
		}
	}

	GPIO_InitStruct.Pin = SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;	// same for all 3 peripherals
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);

	i2cHandle.Instance = I2Cx;
	i2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.ClockSpeed      = 100000;
	i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	i2cHandle.Init.OwnAddress1     = 0xFE;
	i2cHandle.Init.OwnAddress2     = 0xFE;

	if (HAL_I2C_Init(&i2cHandle) != HAL_OK) {
		// TODO: ErrorHandler();
		while(1);
	}
}

void I2C::write(uint16_t addr, uint8_t *pData, uint16_t size) {
	while(HAL_I2C_Master_Transmit(&i2cHandle, addr, pData, size, 1000) != HAL_OK)
	{
		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge it's address)
		   Master restarts communication */
		if (HAL_I2C_GetError(&i2cHandle) != HAL_I2C_ERROR_AF)
		{
		  while(1);
		}
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
	// Do nothing
	// Clocks are enabled in initI2C() and GPIOs are configured
}
#pragma GCC diagnostic pop
