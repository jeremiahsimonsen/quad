/*
 * PwmTimer.h
 *
 *  Created on: Sep 11, 2015
 *      Author: Jeremiah
 */

#ifndef PWMTIMER_H_
#define PWMTIMER_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

enum class TimerPin {
	PA0 = 0,// TIM5_CH1
	PA1,	// TIM2_CH2
	PA2,	// TIM2_CH3
	PA3,	// TIM2_CH4
	PA6,	// TIM3_CH1
	PA7,	// TIM3_CH2
	PA8,	// TIM1_CH1
	PA9,	// TIM1_CH2
	PA10,	// TIM1_CH3
	PA11, 	// TIM1_CH4
	PA15,	// TIM2_CH1

	PB0,	// TIM3_CH3
	PB1,	// TIM3_CH4
	PB3,	// TIM2_CH2
	PB4,	// TIM3_CH1
	PB5,	// TIM3_CH2
	PB6,	// TIM4_CH1
	PB7,	// TIM4_CH2
	PB8,	// TIM4_CH3
	PB9,	// TIM4_CH4
	PB10,	// TIM2_CH3
	PB11, 	// TIM2_CH4
	PB14,	// TIM12_CH1
	PB15,	// TIM12_CH2

	PC6,	// TIM3_CH1
	PC7,	// TIM3_CH2
	PC8,	// TIM3_CH3
	PC9,	// TIM3_CH4

	PD12,	// TIM4_CH1
	PD13,	// TIM4_CH2
	PD14,	// TIM4_CH3
	PD15,	// TIM4_CH4

	PE5,	// TIM9_CH1
	PE6,	// TIM9_CH2
	PE9,	// TIM1_CH1
	PE11,	// TIM1_CH2
	PE13,	// TIM1_CH3
	PE14,	// TIM1_CH4

	PF6,	// TIM10_CH1
	PF7,	// TIM11_CH1
	PF8,	// TIM13_CH1
	PF9,	// TIM14_CH1

	PH10,	// TIM5_CH1
	PH11,	// TIM5_CH2
	PH12,	// TIM5_CH3

	PI0,	// TIM5_CH4
	PI2,	// TIM8_CH4
	PI5,	// TIM8_CH1
	PI6, 	// TIM8_CH2
	PI7		// TIM8_CH3
};

enum class TimerChannel {
	TIM1_CH1 = 0,
	TIM1_CH2,
	TIM1_CH3,
	TIM1_CH4,
	TIM2_CH1,
	TIM2_CH2,
	TIM2_CH3,
	TIM2_CH4,
	TIM3_CH1,
	TIM3_CH2,
	TIM3_CH3,
	TIM3_CH4,
	TIM4_CH1,
	TIM4_CH2,
	TIM4_CH3,
	TIM4_CH4,
	TIM5_CH1,
	TIM5_CH2,
	TIM5_CH3,
	TIM5_CH4,
	TIM8_CH1,
	TIM8_CH2,
	TIM8_CH3,
	TIM8_CH4,
	TIM9_CH1,
	TIM9_CH2,
	TIM10_CH1,
	TIM11_CH1,
	TIM12_CH1,
	TIM12_CH2,
	TIM13_CH1,
	TIM14_CH1
};

enum class ApbNum {
	APB1,
	APB2
};

class PwmTimer {
private:
	TIM_HandleTypeDef TimHandle;
	TimerPin pin;
	float frequency;
	float pulseWidth;

	void initTimer(float f, float w, TimerPin p);

public:
	PwmTimer();
	PwmTimer(float f, TimerPin p);

	void setFreq(float f);
	void setWidth(float w);
};

#endif /* PWMTIMER_H_ */
