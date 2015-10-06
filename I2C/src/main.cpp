#include <I2C.h>
#include "I2C_Msp.h"
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Motor.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.

//	HAL_Init();

	uint8_t slave_addr = 0b10111010;
	I2C i2c(i2cPin::PB6, i2cPin::PB9);
//	initI2C((int)i2cPin::PB6, (int)i2cPin::PB9);

	uint8_t data[3] = {0x0};

	uint8_t on = 0xB4;
	// Power on and set output data rate to 12.5 Hz
//	i2c.memWrite(slave_addr, 0x20, &on, 1);
	I2C_MemWrite(slave_addr, 0x20, &on, 1);
//	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
//	if (HAL_I2C_Mem_Write(&i2cHandle, slave_addr, 0x20, I2C_MEMADD_SIZE_8BIT, &on, 1, 1000) != HAL_OK)
//		while(1);

	// Infinite loop
	while (1)
	{
		int32_t pressure;
		uint8_t x;

//		i2c.memRead(slave_addr, 0x28|(1<<7), data, 3);
		i2c.memRead(slave_addr, 0x2B|(1<<7), data, 2);
//		i2c.memRead(slave_addr, 0x0F, &x, 1);
//		I2C_Mem_Read(slave_addr, 0x2B|(1<<7), (uint8_t *)data, 2);
//		while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
//		if (HAL_I2C_Mem_Read(&i2cHandle, slave_addr, 0x2B|(1<<7), I2C_MEMADD_SIZE_8BIT, data, 2, 1000) != HAL_OK) {
//			while(1);
//		}

//		trace_printf("Temp_L = %d\n", data[0]);
//		trace_printf("Temp_H = %d\n", data[1]);

//		pressure = data[2] << 16 | data[1] << 8 | data[0];
		int16_t temperature = (int16_t) (data[1]<<8 | data[0]);
		float t = 108.5f + (float)temperature / 480.0f * 1.8f;
		char t_str[50];
		sprintf(t_str,"%f",t);
//		trace_printf("Pressure: %d\n", pressure);
		trace_printf("Temp: %s\n", t_str);

//		trace_printf("I am: %d\n", x);
//		HAL_Delay(500);
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
