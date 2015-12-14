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

/** @addtogroup Debugging
 *  @{
 */

/** @addtogroup LED
 *  @{
 */

#include "led.h"

/**
 * @brief Create an LED control object
 * @param b Development board being used
 *
 * This initializes the appropriate GPIO depending on b and initially sets
 * all LEDs off.
 */
led::led(Board b) {
	board = b;

	// Build the map depending on the board
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

/**
 * @brief Helper function to initialize LED GPIOs on the Discovery board
 *
 * This initializes the GPIO pins for the STM32F4-Discovery dev board.
 */
void led::init_Discovery() {
	// Enable the GPIO clock
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// Set GPIO as push-pull output
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin 	= GPIO_PIN_12;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;

	// Initialize the GPIOs
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// Reset all LED pins so they are off
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

/**
 * @brief Helper function to initialize LED GPIOs on the Death Chopper 9000 board
 *
 * This initializes the GPIO pins for the Death Chopper 9000 dev board.
 */
void led::init_DeathChopper() {
	// Enable the GPIO clock
	__HAL_RCC_GPIOE_CLK_ENABLE();

	// Set GPIOs as push-pull output
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin 	= GPIO_PIN_2;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;

	// Initialize the GPIOs
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= GPIO_PIN_3;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin		= GPIO_PIN_5;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// Reset all LED GPIO pins so they are off
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
}

/**
 * @brief Turn LED l on
 * @param l The LED to turn on
 */
void led::turnOn(LED l) {
	// Fetch correct GPIO pin depending on board and l
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	// Set the GPIO high to turn LED on
	HAL_GPIO_WritePin(GPIOx, gpio_pin, GPIO_PIN_SET);
}

/**
 * @brief Turn LED l off
 * @param l The LED to turn off
 */
void led::turnOff(LED l) {
	// Fetch correct GPIO pin depending on board and l
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	// Set the GPIO low to turn LED off
	HAL_GPIO_WritePin(GPIOx, gpio_pin, GPIO_PIN_RESET);
}

/**
 * @brief Toggle LED l
 * @param l The LED to toggle
 */
void led::toggle(LED l) {
	// Fetch correct GPIO pin depending on board and l
	LED_Pin p = led_map[(int)l];
	GPIO_TypeDef *GPIOx = (int)p <= (int)LED_Pin::PE5 ? GPIOE : GPIOD;
	uint16_t LED_Pin_To_GPIO[] = {GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	uint16_t gpio_pin = LED_Pin_To_GPIO[(int)p];

	// Toggle the GPIO to toggle the LED
	HAL_GPIO_TogglePin(GPIOx, gpio_pin);
}

/** @} Close LED group */
/** @} Close Debugging group */
