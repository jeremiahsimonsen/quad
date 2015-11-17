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

static bool enableMotors = false;

void EXTILine0_Config(void);
extern "C" {
	void EXTI0_IRQHandler(void);
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
}

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

	LidarLite lidar;

//	while(1) {
//		float dist = lidar.getDistIn();
//		char str[50];
//		sprintf(str, "%f",dist);
//		trace_printf("%s\n",str);
//	}

	EXTILine0_Config();

	Motor m1(TimerPin::PC9);
	Motor m2(TimerPin::PC7);

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
	Adc vSense(AdcPin::PA2);
//	Adc iSense(AdcPin::PA3);

	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);

//	usart_transmit((uint8_t *)"Hello world!\n\r");

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

		if (enableMotors == true) {
			m1.setSpeed(speed);
			m2.setSpeed(maxSpeed-speed);
		} else {
			m1.setSpeed(0.0f);
			m2.setSpeed(0.0f);
		}

		if (iter % 2 == 0) {
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
		HAL_Delay(500);
	}
}

float mapAdcValToMotorSpeed(uint32_t val) {
	return (float)val / 4096.0f;
}

#pragma GCC diagnostic pop

void EXTILine0_Config(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == KEY_BUTTON_PIN) {
		enableMotors = enableMotors == true? false : true;
	}
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

// ----------------------------------------------------------------------------
