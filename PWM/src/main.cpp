//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "PwmTimer.h"
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

	HAL_Init();

//	PwmTimer pwm1(51.5, TimerPin::PA8);
//	PwmTimer pwm2(51.5, TimerPin::PA1);
//	PwmTimer pwm3(51.5, TimerPin::PA6);
//	PwmTimer pwm4(50.0, TimerPin::PC9);
//	PwmTimer pwm5(51.5, TimerPin::PA0);
//	PwmTimer pwm9(51.5, TimerPin::PE6);
//	PwmTimer pwm12(51.5, TimerPin::PB14);

//	pwm4.setFreq(100.0f);
//	pwm4.setWidth(1.5f);

	Motor m(TimerPin::PC9);
//
	m.setSpeed(1.0f);

	float fDir = 1.0f;
	float wDir = 1.0f;
	float f = 50.0f;
	float w = ZERO_SPEED_WIDTH;

//	while (1) {
//		if (f < 50.0f || f > 400.0f) fDir *= -1;
//		f += 1.0f*fDir;
//		pwm4.setFreq(f);
//
//		if (w < ZERO_SPEED_WIDTH || w > FULL_SPEED_WIDTH) wDir *= -1;
//		w += 0.1f * wDir;
//		pwm4.setWidth(w);
//
//		HAL_Delay(40);
//	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
