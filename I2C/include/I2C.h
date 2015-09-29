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
	I2C_HandleTypeDef i2cHandle;

	i2cPin scl, sda;

	void initI2C();

public:
	I2C();
	I2C(i2cPin cl, i2cPin da);

	void write(uint16_t addr, uint8_t *pData, uint16_t size);
};

#endif
