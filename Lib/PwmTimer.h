/**
 * @file PwmTimer.h
 *
 * @brief Class for generating PWM waveforms on the STM32F407
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 11, 2015
 *
 * This class takes care of the low-level configuration required for PWM signal
 * generation. It allows for simple initialization by specifying a compatible
 * GPIO pin to the constructor.
 *
 * The default constructor PwmTimer::PwmTimer() starts PWM on PA0 with a
 * frequency of 50 Hz and pulse width of 1 ms, consistent with the default for
 * many hobby ESCs. The frequency and pin can be specified using the alternate
 * constructor.
 *
 * The PwmTimer::setFreq() and PwmTimer::setWidth() functions adjust the PWM
 * frequency and positive pulse width, respectively.
 *
 */

#ifndef PWMTIMER_H_
#define PWMTIMER_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

/**
 * GPIO pins that are PWM compatible
 */
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

/**
 * TIM channels for the STM32F407
 */
enum class TimerChannel {
	TIM1_CH1 = 0,//!< TIM1_CH1
	TIM1_CH2,    //!< TIM1_CH2
	TIM1_CH3,    //!< TIM1_CH3
	TIM1_CH4,    //!< TIM1_CH4
	TIM2_CH1,    //!< TIM2_CH1
	TIM2_CH2,    //!< TIM2_CH2
	TIM2_CH3,    //!< TIM2_CH3
	TIM2_CH4,    //!< TIM2_CH4
	TIM3_CH1,    //!< TIM3_CH1
	TIM3_CH2,    //!< TIM3_CH2
	TIM3_CH3,    //!< TIM3_CH3
	TIM3_CH4,    //!< TIM3_CH4
	TIM4_CH1,    //!< TIM4_CH1
	TIM4_CH2,    //!< TIM4_CH2
	TIM4_CH3,    //!< TIM4_CH3
	TIM4_CH4,    //!< TIM4_CH4
	TIM5_CH1,    //!< TIM5_CH1
	TIM5_CH2,    //!< TIM5_CH2
	TIM5_CH3,    //!< TIM5_CH3
	TIM5_CH4,    //!< TIM5_CH4
	TIM8_CH1,    //!< TIM8_CH1
	TIM8_CH2,    //!< TIM8_CH2
	TIM8_CH3,    //!< TIM8_CH3
	TIM8_CH4,    //!< TIM8_CH4
	TIM9_CH1,    //!< TIM9_CH1
	TIM9_CH2,    //!< TIM9_CH2
	TIM10_CH1,   //!< TIM10_CH1
	TIM11_CH1,   //!< TIM11_CH1
	TIM12_CH1,   //!< TIM12_CH1
	TIM12_CH2,   //!< TIM12_CH2
	TIM13_CH1,   //!< TIM13_CH1
	TIM14_CH1    //!< TIM14_CH1
};

/**
 * APBx options - determines f_timer
 */
enum class ApbNum {
	APB1,//!< APB1
	APB2 //!< APB2
};

class PwmTimer {
private:
	TIM_HandleTypeDef TimHandle;	///< STM HAL variable containing TIM config

	TimerPin pin;					///< The pin on which to generate PWM
	TimerChannel ch;				///< The TIM and channel corresponding to the pin
	float fTimer;					///< The counting frequency of the timer
	float frequency;				///< PWM frequency in Hz
	float pulseWidth;				///< PWM positive pulse width in ms

	/**
	 * Helper function for configuring the TIM for the given parameters
	 * @param f PWM frequency in Hz
	 * @param w PWM positive pulse width in ms
	 * @param p GPIO pin to initialize
	 */
	void initTimer(float f, float w, TimerPin p);

public:
	/**
	 * Default constructor initialize pin PA0 for PWM with 50 Hz and 1 ms
	 */
	PwmTimer();
	/**
	 * Initializes the specified GPIO pin, p, for PWM at the frequency, f
	 * @param f PWM frequency in Hz
	 * @param p PWM positive pulse width in ms
	 */
	PwmTimer(float f, TimerPin p);

	/**
	 * Sets the PWM frequency
	 * @param f PWM frequency in Hz
	 */
	void setFreq(float f);
	/**
	 * Sets the PWM positive pulse width
	 * @param w PWM positive pulse width in ms
	 */
	void setWidth(float w);
};

#endif /* PWMTIMER_H_ */
