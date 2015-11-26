#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

#include "uart.h"
#include "Motor.h"
#include "IMU.h"
#include "pid.h"

#define TIMEOUT 100
#define START 255

//	static Motor front(TimerPin::PC8);	// PCB P3
//	static Motor rear(TimerPin::PC6);	// PCB P5
//	static Motor left(TimerPin::PD14);	// PCB P7
//	static Motor right(TimerPin::PD12);	// PCB P9

static Motor front(TimerPin::PC6);
static Motor rear(TimerPin::PC7);
static Motor left(TimerPin::PC8);
static Motor right(TimerPin::PC9);

void descend(float f, float b, float l, float r);

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

	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

	uint8_t transferSize = 6;
	uint8_t DmaBuff[2*transferSize] = {0};
	uint8_t readBuff[transferSize] = {0};

	//	usart_transmit(txBuff);
	usart_receive_begin(DmaBuff, 2*transferSize);

	uint8_t rxTimeout = 0;
	float throttle_cmd, pitch_cmd, roll_cmd, yaw_cmd;
	float front_s = 0.0f, rear_s = 0.0f, left_s = 0.0f, right_s = 0.0f;

	while (1)
	{
		if (usart_read(DmaBuff, readBuff, transferSize) > 0) {
//			trace_printf("%d %d %d %d\n", readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
			char txBuff[50];
			sprintf(txBuff, "%d %d %d %d\n", readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
			usart_transmit((uint8_t *)txBuff);

			if (readBuff[0] != START) {
				continue;
			}

			throttle_cmd = (float)readBuff[1] / 255.0f;
			pitch_cmd 	 = (float)readBuff[2] / 255.0f;
			roll_cmd 	 = (float)readBuff[3] / 255.0f;
			yaw_cmd 	 = (float)readBuff[4] / 255.0f;

			front_s = throttle_cmd - pitch_cmd - yaw_cmd;
			rear_s  = throttle_cmd + pitch_cmd - yaw_cmd;
			right_s = throttle_cmd - roll_cmd  + yaw_cmd;
			left_s  = throttle_cmd + roll_cmd  + yaw_cmd;

			front.setSpeed(front_s);
			rear.setSpeed(rear_s);
			left.setSpeed(left_s);
			right.setSpeed(right_s);

			rxTimeout = 0;
		} else {
			rxTimeout++;
			if (rxTimeout >= TIMEOUT) {
				descend(front_s, rear_s, left_s, right_s);
			}
		}
	}
}

#pragma GCC diagnostic pop

// TODO: Make this feedback controlled based on rangefinder for the final flight controller
void descend(float f, float b, float l, float r) {
	char txBuff[] = "Error, no command received. Descending now";
	usart_transmit((uint8_t *)txBuff);

	while(1) {
		f -= 0.01f;
		b -= 0.01f;
		l -= 0.01f;
		r -= 0.01f;

		front.setSpeed(f);
		rear.setSpeed(b);
		left.setSpeed(l);
		right.setSpeed(r);

		if (f <= 0.0f && b <= 0.0f && l <= 0.0f && r <= 0.0f) {
			while(1);
		}

		HAL_Delay(50);
	}
}


