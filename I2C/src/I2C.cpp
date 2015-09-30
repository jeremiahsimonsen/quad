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
}

I2C::I2C(i2cPin cl, i2cPin da) {
	// TODO: Add check to see if valid: scl is an scl pin and sda is an sda pin
	// TODO: Also check if scl and sda are on the same I2Cx
	scl = cl;
	sda = da;

	initI2C((int)scl, (int)sda);
}

//void I2C::write(uint16_t addr, uint8_t *pData, uint16_t size) {
//	while(HAL_I2C_Master_Transmit(&i2cHandle, addr, pData, size, 1000) != HAL_OK)
//	{
//		/* Error_Handler() function is called when Timeout error occurs.
//		   When Acknowledge failure occurs (Slave don't acknowledge it's address)
//		   Master restarts communication */
//		if (HAL_I2C_GetError(&i2cHandle) != HAL_I2C_ERROR_AF)
//		{
//		  while(1);
//		}
//	}
//}




