/*
 * LidarLite.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: Jasmine
 */


#include "LidarLite.h"
#include "diag/Trace.h"

#ifdef LIDAR_I2C
LidarLite::LidarLite(void) {
	address = 0xC4;
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
}

int8_t LidarLite::startMeasure(void) {
	uint8_t measure = MEASURE_VAL;
	trace_printf("Starting measure\n");
	int8_t retVal = i2c->memWrite(address, (uint8_t)MEASURE_REG, &measure, 1);

	if(retVal < 0){
		return retVal;
	}

	return 0;
}

int8_t LidarLite::read() {
	int8_t retVal;

//	i2c->readyWait();
	HAL_Delay(20);

	uint8_t hilo = DISTANCE_REG_HILO;
	uint8_t attempts = 0;

	trace_printf("Starting read\n");
	do {
		retVal = i2c->memRead(address, (uint8_t)DISTANCE_REG_HILO, dataBuff, 2);
		attempts++;
		HAL_Delay(1);
	}while (retVal < 0 && attempts < 100);

//	do {
//		retVal = i2c->write(address, &hilo,1);
//		attempts++;
//		HAL_Delay(10);
//	} while (retVal < 0 && attempts < 100);

	if (retVal < 0)
		return retVal;

//	retVal = i2c->read(address, dataBuff,2);
	return retVal;

}

int16_t LidarLite::getDistRaw(void) {
	i2c->readyWait();
	return dataBuff[0]<<8 | dataBuff[1];
}

float LidarLite::getDistIn(void) {
	return 0.0f;
}
#else

extern "C" {
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
void TIM2_IRQHandler(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
};

#define NUM_SAMPLES 25

static TIM_HandleTypeDef TimHandle;
static TIM_IC_InitTypeDef sConfig;
static volatile uint32_t pWidthBuff[NUM_SAMPLES];
static HAL_LockTypeDef pWidthLock = HAL_UNLOCKED;

#define _GET_LOCK_RETURN(__LOCK__)                                           \
							do{                                        \
								if((__LOCK__) == HAL_LOCKED)   \
								{                                      \
								   return HAL_BUSY;                    \
								}                                      \
								else                                   \
								{                                      \
								   (__LOCK__) = HAL_LOCKED;    \
								}                                      \
							  }while (0)
#define _GET_LOCK_NORETURN(__LOCK__)                                           \
							do{                                        \
								if((__LOCK__) == HAL_LOCKED)   \
								{                                      \
								   return;                    \
								}                                      \
								else                                   \
								{                                      \
								   (__LOCK__) = HAL_LOCKED;    \
								}                                      \
							  }while (0)
#define _UNLOCK(__LOCK__)												\
							do{											\
								(__LOCK__) = HAL_UNLOCKED;				\
							} while (0)

LidarLite::LidarLite() {
	TimHandle.Instance = TIM2;
	TimHandle.Init.Period = 0xFFFFFFFF;
	TimHandle.Init.Prescaler = 0;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;

	if (HAL_TIM_IC_Init(&TimHandle) != HAL_OK) {
		// Error
	}

	sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sConfig.ICFilter = 0;
	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
		// Error
	}

	sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK) {
		// Error
	}

	// Use PB5 as Trigger
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);		// Pull low for continuous

	if (HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_1) != HAL_OK) {
		// Error
	}
	if (HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_2) != HAL_OK) {
		// Error
	}
}

float LidarLite::getDistRaw() {
	_GET_LOCK_RETURN(pWidthLock);

	uint32_t pWidthSum = 0;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		pWidthSum += pWidthBuff[i];
	}

	_UNLOCK(pWidthLock);

	float dist = (float)pWidthSum / (float)NUM_SAMPLES;

	return dist;
}

float LidarLite::getDistIn() {
	float dist = getDistRaw() / 84e6 / 25.4e-6;
	return dist;
}

// Input Capture using TIM2CH1 and TIM2CH2 on PB3
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler() {
	HAL_TIM_IRQHandler(&TimHandle);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	static uint32_t cntValBuff[4];
	static uint8_t cntValBuffIndex = 0;
	static uint8_t pWidthBuffIndex = 0;

	// If falling edge
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		_GET_LOCK_NORETURN(pWidthLock);

		// Read and store the CCR values
		cntValBuff[cntValBuffIndex++] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);		// Rising edge CNT value
		cntValBuff[cntValBuffIndex++] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);		// Falling edge CNT value

		// Reset circular buffer index
		if (cntValBuffIndex == 4)
			cntValBuffIndex = 0;

		// Calculate new positive pulse width and store in circular history buffer
		if (cntValBuffIndex == 2) {
			pWidthBuff[pWidthBuffIndex++] = cntValBuff[1] - cntValBuff[0];
		} else if (cntValBuffIndex == 0) {
			pWidthBuff[pWidthBuffIndex++] = cntValBuff[3] - cntValBuff[2];
		}

		// Reset circular buffer index
		if (pWidthBuffIndex == NUM_SAMPLES)
			pWidthBuffIndex = 0;

		_UNLOCK(pWidthLock);
	}
}

#endif


