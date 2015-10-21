/*
 * LidarLite.h
 *
 *  Created on: Oct 15, 2015
 *      Author: Jasmine
 */
#include "I2C.h"

#ifndef LIDARLITE_H
#define LIDARLITE_H

#define MEASURE_VAL 0x04
#define MEASURE_REG 0x00
#define STATUS_REG  0x47
#define DISTANCE_REG_HI 0x0f
#define DISTANCE_REG_LO 0x10
#define DISTANCE_REG_HILO 0x8f
#define VERSION_REG 0x41

#define ERROR_READ -1

// Status Bits
#define STAT_BUSY               0x01
#define STAT_REF_OVER           0x02
#define STAT_SIG_OVER           0x04
#define STAT_PIN                0x08
#define STAT_SECOND_PEAK        0x10
#define STAT_TIME               0x20
#define STAT_INVALID            0x40
#define STAT_EYE                0x80

class LidarLite {

private:
	I2C *i2c;
	uint8_t address;

	uint8_t dataBuff[2];



public:
	LidarLite(void);
	int8_t startMeasure(void);
	int8_t read(void);

	int16_t getDistRaw(void);
	float getDistIn(void);
};


#endif // LIDARLITE_H
