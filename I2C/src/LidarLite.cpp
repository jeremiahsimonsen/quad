/*
 * LidarLite.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: Jasmine
 */


#include "LidarLite.h"
#include "diag/Trace.h"

LidarLite::LidarLite(void) {
	address = 0xC4;
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
}

int8_t LidarLite::startMeasure(void) {
	uint8_t measure = MEASURE_VAL;
	trace_printf("Starting measure\n");
	int8_t retVal = i2c->memWrite(address, (uint8_t)MEASURE_REG, &measure, 1);

	if(retVal < 0){
		return retVal;
	}

	return 0;
}

int8_t LidarLite::read() {
	int8_t retVal;

//	i2c->readyWait();
	HAL_Delay(20);

	uint8_t hilo = DISTANCE_REG_HILO;
	uint8_t attempts = 0;

	trace_printf("Starting read\n");
	do {
		retVal = i2c->memRead(address, (uint8_t)DISTANCE_REG_HILO, dataBuff, 2);
		attempts++;
		HAL_Delay(1);
	}while (retVal < 0 && attempts < 100);

//	do {
//		retVal = i2c->write(address, &hilo,1);
//		attempts++;
//		HAL_Delay(10);
//	} while (retVal < 0 && attempts < 100);

	if (retVal < 0)
		return retVal;

//	retVal = i2c->read(address, dataBuff,2);
	return retVal;

}

int16_t LidarLite::getDistRaw(void) {
	i2c->readyWait();
	return dataBuff[0]<<8 | dataBuff[1];
}

float LidarLite::getDistIn(void) {
	return 0.0f;
}









