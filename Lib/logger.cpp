/**
 * @file
 *
 * @brief Class for logging and transmitting data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

/** @defgroup System System/Debug
 *  @brief System level and debugging interfaces
 *  @{
 */

/** @defgroup LOGGER Data logger
 *  @brief Log data via UART for testing purposes
 *  @{
 */

#include "logger.h"

// Global logger instance
logger* logger::loggerInstance = NULL;

/**
 * @brief  Fetch the global logger
 * @return Pointer to the global logger singleton
 *
 * Used to "create" a logger using the singleton pattern
 */
logger* logger::instance() {
	// If the logger has not been created, create it
	if (loggerInstance == NULL) {
		loggerInstance = new logger();
	}

	// Return pointer to the logger
	return loggerInstance;
}

/**
 * @brief Create a logger instance
 *
 * Initializes the active buffer pointer.
 *
 * @note This is only called from logger::instance() one time
 */
logger::logger() {
	activeBuffer = &pingBuffer[0];
}

/**
 * @brief Log data
 * @param s String or byte buffer to log
 *
 * Appends data to the activeBuffer log. When the buffer can no longer
 * hold additional data, it will be transmitted.
 */
void logger::log(char *s) {
	int length = strlen(activeBuffer);

	// If the current buffer can hold the new string, concatenate
	if (length + strlen(s) < LOG_SIZE) {
		strcat(activeBuffer, s);
	} else {	// Otherwise, transmit and switch buffers
		dump();
		if (activeBuffer == pingBuffer) {
			activeBuffer = pongBuffer;
		} else {
			activeBuffer = pingBuffer;
		}
		activeBuffer[0] = 0;
		strcat(activeBuffer, s);
	}
}

/**
 * @brief Transmit buffer
 *
 * Starts a UART transmission to send the logged data.
 *
 * @note It is possible that this function will block, so care should be taken
 * to not log too much data. UART is slow.
 */
void logger::dump() {
	usart_transmit((uint8_t *)activeBuffer);
}

/** @} Close LOGGER group */
/** @} Close Debugging group */
