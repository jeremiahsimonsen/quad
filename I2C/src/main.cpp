#include <I2C.h>
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

	uint8_t data[3] = {0xB0, 0x00, 0x00};
	// Power on and set output data rate to 12.5 Hz
	i2c.memWrite(slave_addr, 0x20, data, 1);
	data[0] = 0x00;

	// Infinite loop
	while (1)
	{
		int32_t pressure;
		uint8_t x;

		i2c.memRead(slave_addr, 0x28|(1<<7), data, 3);

		pressure = data[2] << 16 | data[1] << 8 | data[0];
		trace_printf("Pressure: %d\n", pressure);

//		trace_printf("I am: %d\n", x);
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
