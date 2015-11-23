//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

#include "uart.h"
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

//	Motor m1(TimerPin::PC8);	// PCB P3
//	Motor m2(TimerPin::PC6);	// PCB P5
//	Motor m3(TimerPin::PD14);	// PCB P7
//	Motor m4(TimerPin::PD12);	// PCB P9

	Motor m1(TimerPin::PC6);
	Motor m2(TimerPin::PC7);
	Motor m3(TimerPin::PC8);
	Motor m4(TimerPin::PC9);

	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

	uint8_t txBuff[] = "Hello world";
	uint8_t transferSize = 4;
	uint8_t DmaBuff[2*transferSize] = {0};
	uint8_t readBuff[transferSize] = {0};

	//	usart_transmit(txBuff);
	usart_receive_begin(DmaBuff, 2*transferSize);

	float s1 = 0.0f, s2 = 0.0f, s3 = 0.0f, s4 = 0.0f;
	while (1)
	{
		if (usart_read(DmaBuff, readBuff, transferSize) > 0) {
//			trace_printf("%d %d %d %d\n", readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
			char txBuff[50];
			sprintf(txBuff, "%d %d %d %d\n", readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
			usart_transmit((uint8_t *)txBuff);

			s1 = (float)readBuff[0] / 255.0f;
			s2 = (float)readBuff[1] / 255.0f;
			s3 = (float)readBuff[2] / 255.0f;
			s4 = (float)readBuff[3] / 255.0f;

			m1.setSpeed(s1);
			m2.setSpeed(s2);
			m3.setSpeed(s3);
			m4.setSpeed(s4);
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
