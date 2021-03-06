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
#include "IMU.h"
#include "LidarLite.h"
#include "uart.h"

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

	Motor m1(TimerPin::PC9);
	Motor m2(TimerPin::PC7);
	IMU imu;
	Adc vSense(AdcPin::PA2);
//	Adc iSense(AdcPin::PA3);
	LidarLite lidar;
	init_USART(3, 3, 115200);

	float roll, pitch;
	float height;
	float v;//, i;
	uint32_t iter = 0;
	char txBuff[50];

	// Infinite loop
	while (1)
	{
		roll = imu.getRoll();
		pitch = imu.getPitch();

		height = lidar.getDistIn();

		uint32_t vRaw = vSense.read();
//		uint32_t iRaw = iSense.read();
		v = (float)vRaw * 3.0f / 4096.0f / 63.69e-3f;
//		i = (float)iRaw * 3.0f / 4096.0f / 18.30e-3f;

//		uint32_t adcVal = adc.read();

//		float maxSpeed = mapAdcValToMotorSpeed(adcVal);
		float maxSpeed = 0.2f;

		float speed = (pitch + 90.0f) / 180.0f * maxSpeed;

		m2.setSpeed(2*speed);
		m1.setSpeed(maxSpeed-speed);

		if (iter % 10 == 0) {
//			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\n\r", height, roll, pitch);
			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\tVoltage: %f\n\r", height, roll, pitch, v);
//			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\tVoltage: %f\tCurrent%f\n\r", height, roll, pitch, v, i);
			usart_transmit((uint8_t *)txBuff);
//			trace_printf("%s", txBuff);
		}

		iter++;
		HAL_Delay(40);
	}
}

float mapAdcValToMotorSpeed(uint32_t val) {
	return (float)val / 4096.0f;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
