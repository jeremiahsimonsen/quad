/**
 * @file
 *
 * @brief Error handling
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 13, 2015
 *
 */

/** @addtogroup System
 *  @{
 */

/** @defgroup ERROR Error Handling
 *  @brief Error checking and handling
 *  @{
 */

#include "errDC9000.h"
#include "DeathChopper9000.h"

/**
 * @brief Error messages corresponding to errDC9000
 */
const char* errDC9000_msg[] = {
	"Death Chopper has flipped\n\r",	// FLIPPING
	"Remote control error\n\r",			// REMOTE_CONTROL
	"Remote control timed out\n\r",		// TIMEOUT_ERROR
	"UART Init error\n\r",				// UART_INIT_ERROR
	"UART De-init error\n\r",			// UART_DEINIT_ERROR
	"UART communication error",			// UART_IO_ERROR
	"I2C Init error\n\r",				// I2C_INIT_ERROR
	"I2C De-Init error\n\r",			// I2C_DEINIT_ERROR
	"I2C communication error",			// I2C_IO_ERROR
	"LPS25H init error\n\r",			// LPS_INIT_ERROR
	"LPS25H communication error\n\r",	// LPS_IO_ERROR
	"L3GD20H init error\n\r",			// L3G_INIT_ERROR
	"L3GD20H communication error\n\r",	// L3G_IO_ERROR
	"LSM303D init error\n\r",			// LSM_INIT_ERROR
	"LSM303D communication error\n\r",	// LSM_IO_ERROR
	"PWM init error\n\r",				// PWM_INIT_ERROR
	"LIDAR Lite init error\n\r",		// LIDAR_INIT_ERROR
	"HC-SR04 init error\n\r",			// ULTRASONIC_INIT_ERROR
	"ADC init error\n\r",				// ADC_INIT_ERROR
	"ADC read error\n\r"				// ADC_IO_ERROR
};

/**
 * @brief Handle errors
 * @param e The error
 *
 * Send an error message over UART, turn on the error LED, and shut all
 * motors off.
 *
 * @todo Possibly descend instead of killing the motors immediately
 */
void Error_Handler(errDC9000 e) {
	// If the error has to do with UART, can't send error message via UART
	if (e != errDC9000::UART_INIT_ERROR && e != errDC9000::UART_IO_ERROR && e != errDC9000::UART_DEINIT_ERROR) {
		usart_transmit((uint8_t *)errDC9000_msg[(int)e]);
	}

	leds->turnOn(LED::RED);

	DeathChopper9000 *dc9000 = DeathChopper9000::instance();
	dc9000->abort();
}

/** @} Close ERROR group */
/** @} Close System group */
