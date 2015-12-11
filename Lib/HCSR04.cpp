/**
 * @file HCSR04.cpp
 *
 * @brief Class for interacting with the HC-SR04 ultrasonic rangefinder
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 10, 2015
 *
 */


#include "HCSR04.h"

extern "C" {
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
void TIM2_IRQHandler(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
};

#define NUM_SAMPLES 15

static TIM_HandleTypeDef Tim2Handle;
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

HCSR04::HCSR04() {
	// TIM configuration settings
	Tim2Handle.Instance = TIM2;
	Tim2Handle.Init.Period = 0xFFFFFFFF;
	Tim2Handle.Init.Prescaler = 0;
	Tim2Handle.Init.ClockDivision = 0;
	Tim2Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim2Handle.Init.RepetitionCounter = 0;
	Tim2Handle.State = HAL_TIM_STATE_RESET;

	// Initialize the TIM
	if (HAL_TIM_IC_Init(&Tim2Handle) != HAL_OK) {
		// TODO: Error
	}

	// Channel specific configuration
	// TIM2_CH1 detects rising edges, TIM2_CH2 detects falling edges so pulse width can be measured
	sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sConfig.ICFilter = 0;
	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
		// TODO: Error
	}

	sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, TIM_CHANNEL_2) != HAL_OK) {
		// TODO: Error
	}

	// Old from LIDAR Lite, may need to make PB5 an input so it doesn't affect anything on PCB
//	GPIO_InitTypeDef GPIO_InitStruct;
//
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//
//	GPIO_InitStruct.Pin = GPIO_PIN_5;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);		// Pull low for continuous

	// Start input capture with interrupts
	if (HAL_TIM_IC_Start_IT(&Tim2Handle, TIM_CHANNEL_1) != HAL_OK) {
		// Error
	}
	if (HAL_TIM_IC_Start_IT(&Tim2Handle, TIM_CHANNEL_2) != HAL_OK) {
		// Error
	}

	// Start the TRIG pulse generation to take measurements
	// 50 Hz sample rate, 11 us trig pulse
	PwmTimer trigger(50.0f, TimerPin::PC9);		// P2:PWM (ESC1) on PCB
	trigger.setWidth(0.011f);
}

float HCSR04::getDistRaw() {
	// Locking since interrupt changes pWidthBuff
	_GET_LOCK_RETURN(pWidthLock);

	uint32_t pWidthSum = 0;

	// Accumulate samples for averaging
	for (int i = 0; i < NUM_SAMPLES; i++) {
		pWidthSum += pWidthBuff[i];
	}

	// Give the lock back so the ISR can take new samples
	_UNLOCK(pWidthLock);

	// Calculate the average
	float dist = (float)pWidthSum / (float)NUM_SAMPLES;

	return dist;
}

float HCSR04::getDistIn() {
	float dist = getDistRaw() / 84e6f * 340 / 2 * 39.37f;
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
	HAL_TIM_IRQHandler(&Tim2Handle);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	static uint32_t cntValBuff[4];
	static uint8_t cntValBuffIndex = 0;
	static uint8_t pWidthBuffIndex = 0;

	// If falling edge
	if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
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
			if (cntValBuff[1] >= cntValBuff[0]) {
				pWidthBuff[pWidthBuffIndex++] = cntValBuff[1] - cntValBuff[0];
			} else {
//				pWidthBuff[pWidthBuffIndex++] = 4294967296 - cntValBuff[0] + cntValBuff[1];
			}
		} else if (cntValBuffIndex == 0) {
			if (cntValBuff[3] >= cntValBuff[2]) {
				pWidthBuff[pWidthBuffIndex++] = cntValBuff[3] - cntValBuff[2];
			} else {
//				pWidthBuff[pWidthBuffIndex++] = 4294967296 - cntValBuff[2] + cntValBuff[3];
			}
		}

		// Reset circular buffer index
		if (pWidthBuffIndex == NUM_SAMPLES)
			pWidthBuffIndex = 0;

		_UNLOCK(pWidthLock);
	}
}



