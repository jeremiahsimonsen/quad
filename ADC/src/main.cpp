//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Adc.h"
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

	Adc adc(AdcPin::PA2);
	Motor m(TimerPin::PC9);

	// Infinite loop
	while (1)
	{
		uint32_t adcVal = adc.read();
		float speed = mapAdcValToMotorSpeed(adcVal);
		m.setSpeed(speed);

		HAL_Delay(40);
	}
}

float mapAdcValToMotorSpeed(uint32_t val) {
	return (float)val / 4096.0f;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
