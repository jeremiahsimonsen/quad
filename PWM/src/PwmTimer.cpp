/*
 * PWM.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: Jeremiah
 */

#include <PwmTimer.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

PwmTimer::PwmTimer() {
	pin = TimerPin::PA0;
	frequency = 51.5;
	pulseWidth = 1.0;

	initTimer(frequency, pulseWidth, pin);
}

PwmTimer::PwmTimer(float f, TimerPin p) {
	pin = p;
	if (f < 50.0) frequency = 50.0;
	else if (f > 400.0) frequency = 400.0;
	else frequency = f;
	pulseWidth = 1.0;

	initTimer(frequency, pulseWidth, pin);
}

void PwmTimer::setFreq(float f) {

}

// Note w in ms
void PwmTimer::setWidth(float w) {
	// TODO: Add error checking/clipping for w
	pulseWidth = w;
//	uint32_t ccr;
//	TIM_TypeDef *TIMx;
	uint32_t channel;

	// TODO: Refactor this crappy array mapping shit
//	TIM_TypeDef* TimerChannelToTIM_TD[32] = {TIM1, TIM1, TIM1, TIM1, TIM2, TIM2, TIM2, TIM2, TIM3, TIM3, TIM3, TIM3, TIM4, TIM4, TIM4, TIM4, TIM5, TIM5, TIM5, TIM5, TIM8, TIM8, TIM8, TIM8, TIM9, TIM9, TIM10, TIM11, TIM12, TIM12, TIM13, TIM14};
	uint32_t TimerChannelToCH[32] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1};

//	TIMx = TimerChannelToTIM_TD[(int)ch];
	channel = TimerChannelToCH[(int)ch];

	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = (uint32_t) ( (float)(TimHandle.Init.Period + 1) * pulseWidth * .001 * frequency - 1 );
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCNPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfig.OCNIdleState = TIM_OCIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	// Start the PWM signal generation
	if (HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}
}

void PwmTimer::initTimer(float f, float w, TimerPin p) {
	TimerChannel TimerPinToChannel[50] = {TimerChannel::TIM5_CH1, TimerChannel::TIM2_CH2, TimerChannel::TIM2_CH3, TimerChannel::TIM2_CH4, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM1_CH1, TimerChannel::TIM1_CH2, TimerChannel::TIM1_CH3, TimerChannel::TIM1_CH4, TimerChannel::TIM2_CH1, TimerChannel::TIM3_CH3, TimerChannel::TIM3_CH4, TimerChannel::TIM2_CH2, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM4_CH1, TimerChannel::TIM4_CH2, TimerChannel::TIM4_CH3, TimerChannel::TIM4_CH4, TimerChannel::TIM2_CH3, TimerChannel::TIM2_CH4, TimerChannel::TIM12_CH1, TimerChannel::TIM12_CH2, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM3_CH3, TimerChannel::TIM3_CH4, TimerChannel::TIM4_CH1, TimerChannel::TIM4_CH2, TimerChannel::TIM4_CH3, TimerChannel::TIM4_CH4, TimerChannel::TIM9_CH1, TimerChannel::TIM9_CH2, TimerChannel::TIM1_CH1, TimerChannel::TIM1_CH2, TimerChannel::TIM1_CH3, TimerChannel::TIM1_CH4, TimerChannel::TIM10_CH1, TimerChannel::TIM11_CH1, TimerChannel::TIM13_CH1, TimerChannel::TIM14_CH1, TimerChannel::TIM5_CH1, TimerChannel::TIM5_CH2, TimerChannel::TIM5_CH3, TimerChannel::TIM5_CH4, TimerChannel::TIM8_CH4, TimerChannel::TIM8_CH1, TimerChannel::TIM8_CH2, TimerChannel::TIM8_CH3};
	GPIO_TypeDef* TimerPinToGPIO_TD[50] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC, GPIOC, GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOF, GPIOF, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOI, GPIOI, GPIOI, GPIOI, GPIOI};
	uint16_t TimerPinToGpioPin[50] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,  GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,  GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7	};

	TIM_TypeDef* TimerChannelToTIM_TD[32] = {TIM1, TIM1, TIM1, TIM1, TIM2, TIM2, TIM2, TIM2, TIM3, TIM3, TIM3, TIM3, TIM4, TIM4, TIM4, TIM4, TIM5, TIM5, TIM5, TIM5, TIM8, TIM8, TIM8, TIM8, TIM9, TIM9, TIM10, TIM11, TIM12, TIM12, TIM13, TIM14};
	uint32_t TimerChannelToCH[32] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1};
	uint8_t TimerChannelToAF[32] = {GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM9, GPIO_AF3_TIM9, GPIO_AF3_TIM10, GPIO_AF3_TIM11, GPIO_AF9_TIM12, GPIO_AF9_TIM12, GPIO_AF9_TIM13, GPIO_AF9_TIM14};

	ch = TimerPinToChannel[(int)p];
	GPIO_TypeDef *GPIO_PORT = TimerPinToGPIO_TD[(int)p];
	uint16_t GPIO_PIN = TimerPinToGpioPin[(int)p];

	TIM_TypeDef *TIMx = TimerChannelToTIM_TD[(int)ch];
	uint32_t channel = TimerChannelToCH[(int)ch];
	uint32_t AF = TimerChannelToAF[(int)ch];

	// GPIO Initialization
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the appropriate GPIO clock
	switch((uint32_t)GPIO_PORT) {
	case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
	case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
	case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
	case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
	case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
	case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
	case (uint32_t)GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
	case (uint32_t)GPIOI: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
	default: break;
	}

	GPIO_InitStruct.Pin = GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = AF;

	HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

	// TIM Configuration
	TIM_OC_InitTypeDef sConfig;
	ApbNum apb;

	switch ((uint32_t) TIMx) {
	case (uint32_t)TIM1: apb = ApbNum::APB2; break;
	case (uint32_t)TIM2: apb = ApbNum::APB1; break;
	case (uint32_t)TIM3: apb = ApbNum::APB1; break;
	case (uint32_t)TIM4: apb = ApbNum::APB1; break;
	case (uint32_t)TIM5: apb = ApbNum::APB1; break;
	case (uint32_t)TIM8: apb = ApbNum::APB2; break;
	case (uint32_t)TIM9: apb = ApbNum::APB2; break;
	case (uint32_t)TIM10: apb = ApbNum::APB2; break;
	case (uint32_t)TIM11: apb = ApbNum::APB2; break;
	case (uint32_t)TIM12: apb = ApbNum::APB1; break;
	case (uint32_t)TIM13: apb = ApbNum::APB1; break;
	case (uint32_t)TIM14: apb = ApbNum::APB1; break;
	default: apb = ApbNum::APB1;
	}

	float fTick = 3360000;
	uint32_t SysClkFreq = HAL_RCC_GetSysClockFreq();
	float fTimer;
	if (apb == ApbNum::APB1) fTimer = (float)SysClkFreq / 2;
	else fTimer = (float)SysClkFreq;
	uint32_t PSC = (uint32_t) ( fTimer / fTick - 1 );
	uint32_t ARR = (uint32_t) ( fTick / f - 1 );
	while (ARR > 65535) {
		PSC++;
		fTick = fTimer / (float)(PSC + 1);
		ARR = (uint32_t) ( fTick / f - 1 );
	}

	TimHandle.Instance = TIMx;
	TimHandle.Init.Period = ARR;
	TimHandle.Init.Prescaler = PSC;
	TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.State = HAL_TIM_STATE_RESET;
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.Pulse = (uint32_t) ( (float)(ARR + 1) * f * w * .001 - 1 );		// ccr value
	sConfig.OCNPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.OCNIdleState = TIM_OCIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	// Start the PWM signal generation
	if (HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	uint32_t inst = (uint32_t) htim->Instance;

	// Enable the appropriate TIMx Peripheral clock
	switch(inst) {
	case (uint32_t)TIM1:	__HAL_RCC_TIM1_CLK_ENABLE(); break;
	case (uint32_t)TIM2:	__HAL_RCC_TIM2_CLK_ENABLE(); break;
	case (uint32_t)TIM3:	__HAL_RCC_TIM3_CLK_ENABLE(); break;
	case (uint32_t)TIM4:	__HAL_RCC_TIM4_CLK_ENABLE(); break;
	case (uint32_t)TIM5:	__HAL_RCC_TIM5_CLK_ENABLE(); break;
	case (uint32_t)TIM8:	__HAL_RCC_TIM8_CLK_ENABLE(); break;
	case (uint32_t)TIM9:	__HAL_RCC_TIM9_CLK_ENABLE(); break;
	case (uint32_t)TIM10:	__HAL_RCC_TIM10_CLK_ENABLE(); break;
	case (uint32_t)TIM11:	__HAL_RCC_TIM11_CLK_ENABLE(); break;
	case (uint32_t)TIM12:	__HAL_RCC_TIM12_CLK_ENABLE(); break;
	case (uint32_t)TIM13:	__HAL_RCC_TIM13_CLK_ENABLE(); break;
	case (uint32_t)TIM14:	__HAL_RCC_TIM14_CLK_ENABLE(); break;
	default:	break;
	}

	// GPIO clock is enabled in PwmTimer::initTimer()
}





