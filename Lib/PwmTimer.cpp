/**
 * @file PwmTimer.cpp
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

/** @addtogroup Peripherals
 *  @{
 */

/** @addtogroup TIM
 *  @{
 */

#include <PwmTimer.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

/** @defgroup TIM_Class PwmTimer class
 *  @brief TIM abstraction for generating PWM
 *  @{
 */

/**
 * @brief Default constructor initialize pin PA0 for PWM with 50 Hz and 1 ms
 */
PwmTimer::PwmTimer() {
	pin = TimerPin::PA0;
	frequency = 50.0;
	pulseWidth = 1.0;

	// Initialize the TIM to generate the desired PWM waveform
	initTimer(frequency, pulseWidth, pin);
}

/**
 * @brief Initializes the specified GPIO pin, p, for PWM at the frequency, f
 * @param f PWM frequency in Hz
 * @param p PWM positive pulse width in ms
 */
PwmTimer::PwmTimer(float f, TimerPin p) {
	pin = p;

	// Only allow frequencies between 50 and 400 Hz - ESC limited
	if (f < 50.0) frequency = 50.0;
	else if (f > 400.0) frequency = 400.0;
	else frequency = f;
	pulseWidth = 1.0;

	// Initialize the TIM to generate the desired PWM waveform
	initTimer(frequency, pulseWidth, pin);
}

/**
 * @brief Sets the PWM frequency
 * @param f PWM frequency in Hz
 */
void PwmTimer::setFreq(float f) {
	frequency = f;

	// Desired counting frequency
	float fTick = 3360000.0f;

	// Calculate the necessary Prescaler and Auto-Reload Register values
	uint32_t PSC = (uint32_t) ( fTimer / fTick - 1 );
	uint32_t ARR = (uint32_t) ( fTick / f - 1 );

	// If max value is exceeded, try again
	while (ARR > 65535) {
		PSC++;
		fTick = fTimer / (float)(PSC + 1);
		ARR = (uint32_t) ( fTick / f - 1 );
	}

	// Store the TIM settings in the peripheral
	TimHandle.Init.Period = ARR;
	TimHandle.Init.Prescaler = PSC;
	TimHandle.State = HAL_TIM_STATE_RESET;
	// NOTE: Calls HAL_TIM_PWM_MspInit()
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	// Set the positive pulse width
	setWidth(pulseWidth);
}

/**
 * @brief Sets the PWM positive pulse width
 * @param w PWM positive pulse width in ms
 */
void PwmTimer::setWidth(float w) {
	pulseWidth = w;
	uint32_t channel;

	// Convert to an HAL TIM_CHANNEL_x
	uint32_t TimerChannelToCH[32] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1};

	channel = TimerChannelToCH[(int)ch];

	// Calculate the required Capture/Compare Register value
	uint32_t ccr = (uint32_t) ( (float)(TimHandle.Init.Period + 1) * pulseWidth * .001 * frequency - 1);

	// Set the CCR value
	__HAL_TIM_SET_COMPARE(&this->TimHandle, channel, ccr);
}

/**
 * @brief Helper function for configuring the TIM for the given parameters
 * @param f PWM frequency in Hz
 * @param w PWM positive pulse width in ms
 * @param p GPIO pin to initialize
 */
void PwmTimer::initTimer(float f, float w, TimerPin p) {
	// Arrays for mapping TimerPin p to STM HAL variables
	TimerChannel TimerPinToChannel[50] = {TimerChannel::TIM5_CH1, TimerChannel::TIM2_CH2, TimerChannel::TIM2_CH3, TimerChannel::TIM2_CH4, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM1_CH1, TimerChannel::TIM1_CH2, TimerChannel::TIM1_CH3, TimerChannel::TIM1_CH4, TimerChannel::TIM2_CH1, TimerChannel::TIM3_CH3, TimerChannel::TIM3_CH4, TimerChannel::TIM2_CH2, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM4_CH1, TimerChannel::TIM4_CH2, TimerChannel::TIM4_CH3, TimerChannel::TIM4_CH4, TimerChannel::TIM2_CH3, TimerChannel::TIM2_CH4, TimerChannel::TIM12_CH1, TimerChannel::TIM12_CH2, TimerChannel::TIM3_CH1, TimerChannel::TIM3_CH2, TimerChannel::TIM3_CH3, TimerChannel::TIM3_CH4, TimerChannel::TIM4_CH1, TimerChannel::TIM4_CH2, TimerChannel::TIM4_CH3, TimerChannel::TIM4_CH4, TimerChannel::TIM9_CH1, TimerChannel::TIM9_CH2, TimerChannel::TIM1_CH1, TimerChannel::TIM1_CH2, TimerChannel::TIM1_CH3, TimerChannel::TIM1_CH4, TimerChannel::TIM10_CH1, TimerChannel::TIM11_CH1, TimerChannel::TIM13_CH1, TimerChannel::TIM14_CH1, TimerChannel::TIM5_CH1, TimerChannel::TIM5_CH2, TimerChannel::TIM5_CH3, TimerChannel::TIM5_CH4, TimerChannel::TIM8_CH4, TimerChannel::TIM8_CH1, TimerChannel::TIM8_CH2, TimerChannel::TIM8_CH3};
	GPIO_TypeDef* TimerPinToGPIO_TD[50] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC, GPIOC, GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOE, GPIOF, GPIOF, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOI, GPIOI, GPIOI, GPIOI, GPIOI};
	uint16_t TimerPinToGpioPin[50] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,  GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,  GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7	};

	TIM_TypeDef* TimerChannelToTIM_TD[32] = {TIM1, TIM1, TIM1, TIM1, TIM2, TIM2, TIM2, TIM2, TIM3, TIM3, TIM3, TIM3, TIM4, TIM4, TIM4, TIM4, TIM5, TIM5, TIM5, TIM5, TIM8, TIM8, TIM8, TIM8, TIM9, TIM9, TIM10, TIM11, TIM12, TIM12, TIM13, TIM14};
	uint32_t TimerChannelToCH[32] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1};
	uint8_t TimerChannelToAF[32] = {GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM1, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF1_TIM2, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM3, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM4, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF2_TIM5, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM8, GPIO_AF3_TIM9, GPIO_AF3_TIM9, GPIO_AF3_TIM10, GPIO_AF3_TIM11, GPIO_AF9_TIM12, GPIO_AF9_TIM12, GPIO_AF9_TIM13, GPIO_AF9_TIM14};

	// Map to usable variables
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

	// Set pin configuration
	GPIO_InitStruct.Pin = GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = AF;

	HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

	// TIM Configuration
	ApbNum apb;

	// Determine which APB for f_timer calculation
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

	// Calculate PSC and ARR values for given PWM frequency
	float fTick = 3360000;
	uint32_t SysClkFreq = HAL_RCC_GetSysClockFreq();

	if (apb == ApbNum::APB1) fTimer = (float)(SysClkFreq/2);
	else fTimer = (float)SysClkFreq;

	uint32_t PSC = (uint32_t) ( fTimer / fTick - 1 );
	uint32_t ARR = (uint32_t) ( fTick / f - 1 );

	// If max value is exceeded, try again
	while (ARR > 65535) {
		PSC++;
		fTick = fTimer / (float)(PSC + 1);
		ARR = (uint32_t) ( fTick / f - 1 );
	}

	// Store the TIM settings in the peripheral
	TimHandle.Instance = TIMx;
	TimHandle.Init.Period = ARR;
	TimHandle.Init.Prescaler = PSC;
	TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.State = HAL_TIM_STATE_RESET;
	// NOTE: Calls HAL_TIM_PWM_MspInit()
	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	// Setup channel-specific settings
	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = (uint32_t) ( (float)(TimHandle.Init.Period + 1) * 1.0f * .001f * frequency - 1 );
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCNPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfig.OCNIdleState = TIM_OCIDLESTATE_RESET;

	// Store configuration settings in peripheral registers
	// NOTE: Disables PWM output
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	// Start/re-enable the PWM output
	if (HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK) {
		// TODO: Error_Handler();
		while(1);
	}

	setWidth(w);
}

/** @} Close TIM_Class group */

/** @addtogroup TIM_Functions HAL Required Functions
 *  @brief MCU-specific PWM initialization - required by ST HAL
 *  @{
 */

/**
 * MCU specific initialization. Called by HAL_TIM_PWM_Init()
 * @param htim TIM_HandleTypeDef storing TIM configuration
 */
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

/** @} Close TIM_Functions group */


/** @} Close TIM group */
/** @} Close Peripherals Group */



