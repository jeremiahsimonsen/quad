//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "PwmTimer.h"

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

	HAL_Init();

//	PwmTimer pwm1(51.5, TimerPin::PA8);
//	PwmTimer pwm2(51.5, TimerPin::PA1);
//	PwmTimer pwm3(51.5, TimerPin::PA6);
	PwmTimer pwm4(51.5, TimerPin::PB7);
	PwmTimer pwm5(51.5, TimerPin::PA0);
//	PwmTimer pwm9(51.5, TimerPin::PE6);
//	PwmTimer pwm12(51.5, TimerPin::PB14);

	float w = 1.0;
	float dir = 1.0;

	while (1) {
		if (w >= 2.5) dir *= -1.0f;
		else if (w <= 0.5) dir *= -1.0f;
		w += dir * 0.1f;

		pwm4.setWidth(w);

//		for (int i = 0; i < 1000; i++) {
			// do nothing
//		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
