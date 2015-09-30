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

float mapAdcValToMotorSpeed(uint32_t val);

int main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.

	uint8_t slave_addr = 0b01011101;
	I2C i2c(i2cPin::PB8, i2cPin::PB9);
//	I2C i2c();

	uint8_t on_buf[] = {0x20, 0xB0};

	// Infinite loop
	while (1)
	{

	}
}

float mapAdcValToMotorSpeed(uint32_t val) {
	return (float)val / 4096.0f;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
