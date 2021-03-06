/**
 * @file Adc.cpp
 *
 * @brief Class for using the ADC
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 19, 2015
 *
 */

/** @addtogroup Peripherals
 *  @{
 */

/** @defgroup Adc ADC (Analog to Digital Converter)
 *  @brief Module for reading from the ADC
 *  @{
 */

#include "Adc.h"
#include "errDC9000.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

static ADC_HandleTypeDef AdcHandle;

/** @defgroup Adc_Class Adc class
 *  @brief ADC abstraction for reading analog values
 *  @{
 */

/**
 * @brief Default constructor to initialize ADC for reading pin PA2
 */
Adc::Adc() {
	pin = AdcPin::PA2;

	// Initialize the ADC
	initAdc(pin);
}

/**
 * @brief Constructor to initialize the ADC for reading from specified pin
 * @param p
 */
Adc::Adc(AdcPin p) {
	pin = p;

	// Initialize the ADC
	initAdc(pin);
}

/**
 * @brief Initialize hardware so ADC can read from pin p
 * @param p GPIO pin to read from
 */
void Adc::initAdc(AdcPin p) {
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the GPIO clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure the GPIO pin
	GPIO_InitStruct.Pin = (p == AdcPin::PA2) ? GPIO_PIN_2 : GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Configure the ADC
	AdcHandle.Instance = ADC1;
	AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.ScanConvMode = DISABLE;
	AdcHandle.Init.EOCSelection = DISABLE;
	AdcHandle.Init.ContinuousConvMode = DISABLE;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.NbrOfConversion = 1;
	AdcHandle.Init.DiscontinuousConvMode = ENABLE;
	AdcHandle.Init.NbrOfDiscConversion = 1;
	AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.State = HAL_ADC_STATE_RESET;

	if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
		Error_Handler(errDC9000::ADC_INIT_ERROR);
	}

	// Configure the ADC channel
	ADC_ChannelConfTypeDef sConfig;

	sConfig.Channel = (p == AdcPin::PA2) ? ADC_CHANNEL_2 : ADC_CHANNEL_3;
	sConfig.Rank = (p == AdcPin::PA2) ? 1 : 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
		Error_Handler(errDC9000::ADC_INIT_ERROR);
	}
}

/**
 * @brief Perform a conversion and read from the ADC data register
 * @return Raw sample value
 */
uint32_t Adc::read() {
	uint32_t retval;

	// Start the conversion
	if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
		Error_Handler(errDC9000::ADC_IO_ERROR);
	}

	// Wait for end of conversion
	if (HAL_ADC_PollForConversion(&AdcHandle, 1000) != HAL_OK) {
		Error_Handler(errDC9000::ADC_IO_ERROR);
	}

	// Get the conversion value
	retval = HAL_ADC_GetValue(&AdcHandle);

	// Stop the ADC
	if (HAL_ADC_Stop(&AdcHandle) != HAL_OK) {
		Error_Handler(errDC9000::ADC_IO_ERROR);
	}

	return retval;
}

/** @} Close Adc_Class group */

/** @addtogroup Adc_Functions HAL-Required Functions
 *  @brief MCU-specific ADC initialization - required by ST HAL
 *  @{
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {


	// Enable the ADC clock
	__HAL_RCC_ADC1_CLK_ENABLE();

	// GPIO configured in initAdc()
}
#pragma GCC diagnostic pop

/** @} Close Adc_Functions group */

/** @} Close Adc group */
/** @} Close Peripherals Group */
