/**
 * @file
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

/** @addtogroup Adc
 *  @{
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

/**
 * @brief ADC GPIO Pins to be used
 */
enum class AdcPin {
	PA2,	// ADC1_IN2
	PA3		// ADC1_IN3
};

/**
 * @brief Class for reading from the ADC
 */
class Adc {
private:
	AdcPin pin;					///< GPIO pin to read

	void initAdc(AdcPin p);

public:
	Adc();
	Adc(AdcPin p);

	uint32_t read();
};

#endif

/** @} Close Adc group */
/** @} Close Peripherals Group */
