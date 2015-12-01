#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

#include "uart.h"
#include "Motor.h"
#include "IMU.h"
#include "pid.h"

// UART RX parameters
#define TIMEOUT 100000
#define START 255

// Flight Parameters
#define MAX_ANGLE 30.0f					// Maximum pitch & roll angle [deg]
#define MAX_RATE  180.0f				// Maximum yaw rate [deg/s]

#define PITCH_KP 0.1f
#define PITCH_KI 0.0f
#define PITCH_KD 0.0f

#define ROLL_KP  0.1f
#define ROLL_KI  0.0f
#define ROLL_KD  0.0f

//static Motor rear(TimerPin::PC8);	// PCB P3
//static Motor right(TimerPin::PC6);	// PCB P5
//static Motor front(TimerPin::PD14);	// PCB P7
//static Motor left(TimerPin::PD12);	// PCB P9

static Motor front(TimerPin::PC6);
static Motor rear(TimerPin::PC7);
static Motor left(TimerPin::PC8);
static Motor right(TimerPin::PC9);

//void descend(float f, float b, float l, float r);

void main()
{
	// At this stage the system clock should have already been configured
	// at high speed.

	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

	uint8_t transferSize = 6;
	uint8_t DmaBuff[2*transferSize] = {0};
	uint8_t readBuff[transferSize] = {0};
	uint32_t iter = 0;

	char txBuff[] = "USART working\n\r";
	usart_transmit((uint8_t *)txBuff);
	usart_receive_begin(DmaBuff, 2*transferSize);

//	uint32_t rxTimeout = 0;
	float throttle_cmd = 0.0f, pitch_cmd = 0.0f, roll_cmd = 0.0f, yaw_cmd = 0.0f;
	float pitch_y, roll_y;
	float pitch_e, roll_e;
	float u_pitch, u_roll;
	float u_pitch_cmd, u_roll_cmd;
	float front_s = 0.0f, rear_s = 0.0f, left_s = 0.0f, right_s = 0.0f;

	L3GD20H_InitStruct gyroConfig;
	gyroConfig.fs_config = L3GD_FS_Config::MEDIUM;
	gyroConfig.odr_bw_config = L3GD_ODR_BW_Config::NINETEEN;

	LSM303D_InitStruct accelConfig;
	accelConfig.aodr_config = LSM_AODR_Config::THREE;
	accelConfig.abw_config = LSM_ABW_Config::ONE;
	accelConfig.afs_config = LSM_AFS_Config::FOUR;
	accelConfig.modr_config = LSM_MODR_Config::SIX;
	accelConfig.mres_config = LSM_MRES_Config::HIGH;
	accelConfig.mfs_config = LSM_MFS_Config::FOUR;
	accelConfig.md_config = LSM_MD_Config::CONTINUOUS;

	IMU imu(gyroConfig, accelConfig);

	pid pitch_pid(PITCH_KP, PITCH_KI, PITCH_KD);
	pid roll_pid(ROLL_KP, ROLL_KI, ROLL_KD);

	while (1)
	{
		if (usart_read(DmaBuff, readBuff, transferSize) > 0) {
//			trace_printf("%d %d %d %d\n", readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
			char txBuff[100];
			sprintf(txBuff, "Received: %d %d %d %d %d %d\n\r", readBuff[0], readBuff[1], readBuff[2], readBuff[3], readBuff[4], readBuff[5]);
			usart_transmit((uint8_t *)txBuff);

//			if (readBuff[0] != START) {
//				continue;
//			}

			throttle_cmd = (float)readBuff[1] / 255.0f;
			pitch_cmd 	 = ((float)readBuff[2] - 128.0) / 128.0f * MAX_ANGLE;
			roll_cmd 	 = ((float)readBuff[3] - 128.0) / 128.0f * MAX_ANGLE;
			yaw_cmd 	 = ((float)readBuff[4] - 128.0) / 128.0f;

			char txBuff2[100];
			sprintf(txBuff2, "Motors: %f %f %f %f\n\r", front_s, rear_s, right_s, left_s);
			usart_transmit((uint8_t *)txBuff2);
//
//			rxTimeout = 0;
		} //else {
//			rxTimeout++;
//			if (rxTimeout >= TIMEOUT) {
//				descend(front_s, rear_s, left_s, right_s);
//			}
//		}

		// Measure the "output" angles
		pitch_y = imu.getPitch();
		roll_y  = imu.getRoll();

		// Calculate the errors
		pitch_e = pitch_cmd - pitch_y;
		roll_e  = roll_cmd  - roll_y;

		// Calculate PID control outputs
		u_pitch = pitch_pid.calculate(pitch_e);
		u_roll  = roll_pid.calculate(roll_e);

		// Convert to motor commands
		u_pitch_cmd = u_pitch / MAX_ANGLE;
		u_roll_cmd  = u_roll  / MAX_ANGLE;

		front_s = throttle_cmd - u_pitch_cmd - yaw_cmd;
		rear_s  = throttle_cmd + u_pitch_cmd - yaw_cmd;
		right_s = throttle_cmd - u_roll_cmd  + yaw_cmd;
		left_s  = throttle_cmd + u_roll_cmd  + yaw_cmd;

		front.setSpeed(front_s);
		rear.setSpeed(rear_s);
		left.setSpeed(left_s);
		right.setSpeed(right_s);

		if (iter % 50 == 0) {
			char txBuff2[100];
			sprintf(txBuff2, "Motors: %f %f %f %f\n\r", front_s, rear_s, right_s, left_s);
			usart_transmit((uint8_t *)txBuff2);
		}

		iter++;
	}
}


// TODO: Make this feedback controlled based on rangefinder for the final flight controller
//void descend(float f, float b, float l, float r) {
//	char txBuff[] = "Error, no command received. Descending now";
//	usart_transmit((uint8_t *)txBuff);
//
//	while(1) {
//		f -= 0.01f;
//		b -= 0.01f;
//		l -= 0.01f;
//		r -= 0.01f;
//
//		front.setSpeed(f);
//		rear.setSpeed(b);
//		left.setSpeed(l);
//		right.setSpeed(r);
//
//		if (f <= 0.0f && b <= 0.0f && l <= 0.0f && r <= 0.0f) {
//			while(1);
//		}
//
//		HAL_Delay(50);
//	}
//}


