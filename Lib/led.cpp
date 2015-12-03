/**
 * @file led.cpp
 *
 * @brief Class for controlling board status LEDs
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 3, 2015
 *
 */

#include "led.h"

led::led(Board b) {
	board = b;

	if (board == Board::DEATH_CHOPPER_9000) {
		led_map[(int)LED::RED]    = LED_Pin::PE2;
		led_map[(int)LED::GREEN]  = LED_Pin::PE3;
		led_map[(int)LED::BLUE]   = LED_Pin::PE4;
		led_map[(int)LED::ORANGE] = LED_Pin::PE5;
		init_DeathChopper();
	} else {
		led_map[(int)LED::RED]    = LED_Pin::PD14;
		led_map[(int)LED::GREEN]  = LED_Pin::PD12;
		led_map[(int)LED::BLUE]   = LED_Pin::PD15;
		led_map[(int)LED::ORANGE] = LED_Pin::PD13;
		init_Discovery();
	}
}

void led::init_Discovery() {
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin 	= GPIO_PIN_12;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void led::init_DeathChopper() {
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin 	= GPIO_PIN_2;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= GPIO_PIN_3;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_5;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
}

void led::turnOn(LED l) {
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	HAL_GPIO_WritePin(GPIOx, gpio_pin, GPIO_PIN_SET);
}

void led::turnOff(LED l) {
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	HAL_GPIO_WritePin(GPIOx, gpio_pin, GPIO_PIN_RESET);
}

void led::toggle(LED l) {
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	HAL_GPIO_TogglePin(GPIOx, gpio_pin);
}


