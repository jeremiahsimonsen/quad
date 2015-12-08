/**
 * @file Logger.h
 *
 * @brief Class for logging and transmitting data
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 7, 2015
 *
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <string.h>

#include "uart.h"

#define LOG_SIZE 1024

//#define LOG_RAW
//#define LOG_PREFILTERED
//#define LOG_ACC_ANGLE
//#define LOG_COMP_FILTERED
//#define LOG_OUTPUT

class logger {
private:
	char pingBuffer[LOG_SIZE] = {0};
	char pongBuffer[LOG_SIZE] = {0};
	char *activeBuffer;

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
