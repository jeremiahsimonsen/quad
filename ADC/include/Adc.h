/**
 * @file Adc.h
 *
 * @brief Class for using the ADC
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 19, 2015
 *
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

enum class AdcPin {
	PA2,	// ADC1_IN2
	PA3		// ADC1_IN3
};

class Adc {
private:
	ADC_HandleTypeDef AdcHandle;

	AdcPin pin;

	void initAdc(AdcPin p);

public:
	Adc();
	Adc(AdcPin p);

	uint32_t read();
};

#endif
