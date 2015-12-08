/**
 * @file Logger.cpp
 *
 * @brief Class for logging and transmitting data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

#include "logger.h"

logger* logger::loggerInstance = NULL;

logger* logger::instance() {
	if (loggerInstance == NULL) {
		loggerInstance = new logger();
	}

	return loggerInstance;
}

logger::logger() {
	activeBuffer = &pingBuffer[0];
}

void logger::log(char *s) {
	int length = strlen(activeBuffer);

	if (length + strlen(s) < LOG_SIZE) {
		strcat(activeBuffer, s);
	} else {
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

void logger::dump() {
	usart_transmit((uint8_t *)activeBuffer);
}
