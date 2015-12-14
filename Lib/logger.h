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
 * This class is used to log and transmit data for debugging purposes.
 * It uses a software double buffer - one buffer is being filled, while
 * the other is being transmitted over the UART. Data is only transmitted
 * when a buffer has been filled to reduce performance overhead.
 *
 * @note It is assumed that init_usart() has been called before a logger is
 * created.
 */

/** @addtogroup Debugging
 *  @{
 */

/** @addtogroup LOGGER Data logger
 *  @{
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <string.h>

#include "uart.h"

// Size of the log buffers
#define LOG_SIZE 1024

//#define LOG_RAW
//#define LOG_PREFILTERED
//#define LOG_ACC_ANGLE
//#define LOG_COMP_FILTERED
//#define LOG_OUTPUT

/**
 * @brief Class for logging and transmitting data
 *
 * This class is used to log and transmit data for debugging purposes.
 * It uses a software double buffer - one buffer is being filled, while
 * the other is being transmitted over the UART. Data is only transmitted
 * when a buffer has been filled to reduce performance overhead.
 *
 * A singleton pattern is used so only a single logger instance can be created.
 * To use the class, a pointer is retrieved using instance()
 */
class logger {
private:
	char pingBuffer[LOG_SIZE] = {0};		///< Logger Ping buffer
	char pongBuffer[LOG_SIZE] = {0};		///< Logger Pong buffer
	char *activeBuffer;						///< Points to the active buffer

	// Private constructors for singleton pattern
	logger();
	logger(logger const&);
	logger& operator=(logger const&);

	static logger *loggerInstance;

public:
	static logger* instance();

	void log(char *s);
	void dump(void);
};

#endif

/** @} Close LOGGER group */
/** @} Close Debugging group */
