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

	Motor m1(TimerPin::PC8);
	Motor m2(TimerPin::PC6);

	L3GD20H_InitStruct gyroConfig;
	gyroConfig.fs_config = L3GD_FS_Config::MEDIUM;
	gyroConfig.odr_bw_config = L3GD_ODR_BW_Config::NINETEEN;

	LSM303D_InitStruct accelConfig;
	accelConfig.aodr_config = LSM_AODR_Config::FOUR;
	accelConfig.abw_config = LSM_ABW_Config::ONE;
	accelConfig.afs_config = LSM_AFS_Config::FOUR;
	accelConfig.modr_config = LSM_MODR_Config::SIX;
	accelConfig.mres_config = LSM_MRES_Config::HIGH;
	accelConfig.mfs_config = LSM_MFS_Config::FOUR;
	accelConfig.md_config = LSM_MD_Config::CONTINUOUS;

	IMU imu(gyroConfig, accelConfig);
	Adc vSense(AdcPin::PA2);
	Adc iSense(AdcPin::PA3);
	LidarLite lidar;
	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

	usart_transmit((uint8_t *)"Hello world!\n\r");

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
		float maxSpeed = 0.5f;

		float speed = (roll + 90.0f) / 180.0f * maxSpeed;

		m1.setSpeed(speed);
		m2.setSpeed(maxSpeed-speed);

		if (iter % 30 == 0) {
//			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\n\r", height, roll, pitch);
			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\tVoltage: %f\n\r", height, roll, pitch, v);
//			sprintf(txBuff, "Height: %f\tRoll: %f\tPitch: %f\tVoltage: %f\tCurrent%f\n\r", height, roll, pitch, v, i);
//			float dt = imu.getDT();
//			sprintf(txBuff, "DT: %f\n\r", dt);
//			sprintf(txBuff, "!ANG:%f,%f,%f\n\r", roll, pitch, 0.0f);
//			sprintf(txBuff, "Speed: %f\n\r", speed);
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
