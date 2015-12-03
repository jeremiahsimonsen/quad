/**
 * @file led.h
 *
 * @brief Class for controlling board status LEDs
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 3, 2015
 *
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

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

enum class LED {
	RED = 0,
	GREEN,
	BLUE,
	ORANGE
};

enum class Board {
	STM32F4_DISCOVERY,
	DEATH_CHOPPER_9000
};

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
