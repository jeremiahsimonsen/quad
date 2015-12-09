/**
 * @file dac.h
 *
 * @brief Class for outputting values via the DAC
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 8, 2015
 *
 */

#ifndef DAC_H_
#define DAC_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dac.h"
//#include "stm32f4xx_hal_rcc.h"
//#include "stm32f4xx_hal_gpio.h"
//#include "stm32f4_discovery.h"
//#include "stm32f407xx.h"

class dac {
private:
	DAC_HandleTypeDef DacHandle;

public:
	dac();

	void putSample(float x);
};

#endif
