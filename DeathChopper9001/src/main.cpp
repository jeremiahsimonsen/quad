#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

#include "uart.h"
#include "Motor.h"
#include "IMU.h"
#include "pid.h"
#include "pid2.h"
#include "led.h"

// UART RX parameters
#define TIMEOUT 500

// Flight Parameters
#define MAX_ANGLE 10.0f					// Maximum pitch & roll angle [deg]
#define MAX_RATE  180.0f				// Maximum yaw rate [deg/s]

#define PITCH_KP 0.5f
#define PITCH_KI 0.0f
#define PITCH_KD 0.0f

#define ROLL_KP  0.5f
#define ROLL_KI  0.0f
#define ROLL_KD  0.0f

//#define DISCOVERY_BOARD
#define DEATH_CHOPPER

#define RX_TIMEOUT_ENABLE

#define LOOP_DELAY 10	// Main loop delay in ms

#ifdef DEATH_CHOPPER
#define BOARD Board::DEATH_CHOPPER_9000
static Motor left(TimerPin::PC8);	// PCB P3
static Motor rear(TimerPin::PC6);	// PCB P5
static Motor front(TimerPin::PD14);	// PCB P7
static Motor right(TimerPin::PD12);	// PCB P9
#endif
#ifdef DISCOVERY_BOARD
#define BOARD Board::STM32F4_DISCOVERY
static Motor front(TimerPin::PC6);
static Motor rear(TimerPin::PC7);
static Motor left(TimerPin::PC8);
static Motor right(TimerPin::PC9);
#endif

static led *leds;

//void descend(float f, float b, float l, float r);
void abort(void);

void main()
{
	// At this stage the system clock should have already been configured
	// at high speed.
	leds = new led(BOARD);

	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

	uint8_t *readBuff = NULL;
	uint32_t iter = 0;

	char txBuff[] = "USART working\n\r";
	usart_transmit((uint8_t *)txBuff);
	usart_receive_begin();

	uint32_t rxTimeout = 0;
//#warning "Throttle set to full"
	float throttle_cmd = 0.0f, pitch_cmd = 0.0f, roll_cmd = 0.0f, yaw_cmd = 0.0f;
	float pitch_y, roll_y;
	float pitch_e, roll_e;
	float u_pitch, u_roll;
	float u_pitch_cmd, u_roll_cmd;
	float front_s = 0.0f, rear_s = 0.0f, left_s = 0.0f, right_s = 0.0f;

	L3GD20H_InitStruct gyroConfig;
	gyroConfig.fs_config = L3GD_FS_Config::MEDIUM;
	gyroConfig.hpcf_config = L3GD_HPCF_Config::THREE;
	gyroConfig.hpm_config = L3GD_HPM_Config::THREE;
	gyroConfig.odr_bw_config = L3GD_ODR_BW_Config::EIGHT;

	LSM303D_InitStruct accelConfig;
	accelConfig.aodr_config = LSM_AODR_Config::SIX;
	accelConfig.abw_config = LSM_ABW_Config::FOUR;
	accelConfig.afs_config = LSM_AFS_Config::FOUR;
	accelConfig.modr_config = LSM_MODR_Config::SIX;
	accelConfig.mres_config = LSM_MRES_Config::HIGH;
	accelConfig.mfs_config = LSM_MFS_Config::FOUR;
	accelConfig.md_config = LSM_MD_Config::CONTINUOUS;

	IMU imu(gyroConfig, accelConfig);

	pid2 pitch_pid(PITCH_KP, PITCH_KI, PITCH_KD);
	pid2 roll_pid(ROLL_KP, ROLL_KI, ROLL_KD);

	while (1)
	{
		leds->toggle(LED::BLUE);
		readBuff = usart_read();
		if (readBuff != NULL) {
//			char txBuff[100];
//			sprintf(txBuff, "Received: %d %d %d %d %d %d\n\r", readBuff[0], readBuff[1], readBuff[2], readBuff[3], readBuff[4], readBuff[5]);
//			usart_transmit((uint8_t *)txBuff);

			if (readBuff[0] != START || readBuff[5] != STOP) {
				leds->turnOn(LED::RED);
				abort();
			}

			throttle_cmd = (float)readBuff[1] / 253.0f;
			pitch_cmd 	 = ((float)readBuff[2] - 127.0) / 127.0f * MAX_ANGLE;
			roll_cmd 	 = ((float)readBuff[3] - 127.0) / 127.0f * MAX_ANGLE;
			yaw_cmd 	 = ((float)readBuff[4] - 127.0) / 127.0f * MAX_RATE;

//			char txBuff2[100];
//			sprintf(txBuff2, "Motors: %f %f %f %f\n\r", front_s, rear_s, right_s, left_s);
//			usart_transmit((uint8_t *)txBuff2);

			rxTimeout = 0;
		}
#ifdef RX_TIMEOUT_ENABLE
		else {
			rxTimeout++;
			if (rxTimeout >= TIMEOUT) {
//				descend(front_s, rear_s, left_s, right_s);
				leds->turnOn(LED::RED);
				abort();
			}
		}
#endif

		// Measure the "output" angles
//		pitch_y = imu.getPitch();
//		roll_y  = imu.getRoll();
		imu.getRollPitch(&roll_y, &pitch_y);

		// Calculate the errors
		pitch_e = pitch_cmd - pitch_y;
		roll_e  = roll_cmd  - roll_y;

		// Calculate PID control outputs
//		u_pitch = pitch_pid.calculate(pitch_e);
//		u_roll  = roll_pid.calculate(roll_e);
		u_pitch = pitch_pid.calculate(pitch_e, LOOP_DELAY);
		u_roll  = roll_pid.calculate(roll_e, LOOP_DELAY);

		// Convert to motor commands
		u_pitch_cmd = u_pitch / MAX_ANGLE;
		u_roll_cmd  = u_roll  / MAX_ANGLE;

		front_s = throttle_cmd - u_pitch_cmd;// - yaw_cmd;
		rear_s  = throttle_cmd + u_pitch_cmd;// - yaw_cmd;
		right_s = throttle_cmd - u_roll_cmd;//  + yaw_cmd;
		left_s  = throttle_cmd + u_roll_cmd;//  + yaw_cmd;

		front.setSpeed(front_s);
		rear.setSpeed(rear_s);
		left.setSpeed(left_s);
		right.setSpeed(right_s);

		if (iter % 10 == 0) {
			char txBuff2[100];
			sprintf(txBuff2, "%f\t%f\n\r", pitch_y, roll_y);
//			sprintf(txBuff2, "Motors: %f %f %f %f\n\r", front_s, rear_s, right_s, left_s);
			usart_transmit((uint8_t *)txBuff2);
		}

		iter++;
		HAL_Delay(LOOP_DELAY);
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


void abort() {
	front.setSpeed(0.0f);
	rear.setSpeed(0.0f);
	left.setSpeed(0.0f);
	right.setSpeed(0.0f);
	while(1);
}
