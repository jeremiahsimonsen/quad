/**
 * @file
 *
 * @brief Class for controlling board status LEDs
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 3, 2015
 *
 */

/** @addtogroup System
 *  @{
 */

/** @addtogroup LED
 *  @{
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

/**
 * @brief GPIO pins connected to LEDs
 *
 * GPIO pins with LEDs on the STM32F4-Discovery or the Death Chopper 9000
 * development boards
 */
enum class LED_Pin {
	// PCB
	PE2 = 0,// Red
	PE3,	// Green
	PE4, 	// Blue
	PE5,	// Orange
	// Discovery board
	PD12,	// Green
	PD13,	// Orange
	PD14,	// Red
	PD15	// Blue
};

/**
 * @brief LED colors
 */
enum class LED {
	RED = 0,//!< RED
	GREEN,  //!< GREEN
	BLUE,   //!< BLUE
	ORANGE  //!< ORANGE
};

/**
 * @brief Development boards
 */
enum class Board {
	STM32F4_DISCOVERY,//!< STM32F4_DISCOVERY
	DEATH_CHOPPER_9000//!< DEATH_CHOPPER_9000
};

/**
 * @brief LED control
 *
 * This class abstracts the control of the status LEDs so that other code
 * that toggles LEDs is independent of which development board is being
 * used.
 *
 * The LED setup is determined by the Board argument to the constructor.
 */
class led {
private:
	Board board;
	LED_Pin led_map[4];

	void init_Discovery(void);
	void init_DeathChopper(void);

public:
	led(Board b);

	void turnOn(LED l);
	void turnOff(LED l);
	void toggle(LED l);
};

#endif

/** @} Close LED group */
/** @} Close Debugging group */
