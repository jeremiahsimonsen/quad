/*
 * LidarLite.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: Jasmine
 */


#include "LidarLite.h"
#include "diag/Trace.h"

LidarLite::LidarLite(void) {

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
	//enable(init);
}


//// Read distance in cm from LidarLite
//uint8_t L3GD20H::read(void) {
//#if USE_DOUBLE_BUFFERING
//	buffIndicator = i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff1, gyroBuff2, 6);
//	return buffIndicator;
//#else
//	return i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff, 6);
//#endif
//}

int LidarLite::lidar_read() {
	int retVal;
	uint8_t pdata[2] = {MEASURE_VAL, 0};

	// send "measure" command
	if(i2c->memWrite((uint8_t)LIDAR_LITE_ADRS, (uint8_t)MEASURE_REG, pdata, 1)<0){
		return -1;
	}

	HAL_Delay(20);

	uint8_t hilo = DISTANCE_REG_HILO;
	uint8_t attempts = 0;
	while ((retVal = i2c->write((uint8_t)LIDAR_LITE_ADRS, &hilo,1)) < 0 && attempts < 100) {
		attempts++;
		HAL_Delay(1);
	}
//	retVal = i2c->write((uint8_t)LIDAR_LITE_ADRS, &hilo,1);
	if (retVal < 0)
		return -2;
	retVal = i2c->read((uint8_t)LIDAR_LITE_ADRS, pdata,2);
//	retVal = i2c->memRead((uint8_t)LIDAR_LITE_ADRS, (uint8_t)DISTANCE_REG_HILO, pdata, 2);

	i2c->readyWait();
	if (retVal < 0)
		return -3;
	else
		return pdata[0]<<8 | pdata[1];

	// Read second byte and append with first
//	loVal = i2c->memRead((uint8_t)LIDAR_LITE_ADRS, (uint8_t)DISTANCE_REG_LO, &pdata, 1);
//	loVal = pdata;
	//loVal = _read_byteNZ(fd, DISTANCE_REG_LO) ;
	//if (_dbg) printf(" Lo=%d\n", loVal);

	// read first byte
//	hiVal = i2c->memRead((uint8_t)LIDAR_LITE_ADRS, (uint8_t)DISTANCE_REG_HI, &pdata, 1);
//	hiVal = pdata;
	//hiVal = _read_byte(fd, DISTANCE_REG_HI) ;
	//if (_dbg) printf ("Hi=%d ", hiVal);

//	return ((hiVal<<8)|loVal);
}
//
//unsigned char lidar_version(int fd) {
//		return( (unsigned char) _read_byteNZ(fd, VERSION_REG) );
//		}

unsigned char LidarLite::lidar_status() {
//	return( (unsigned char) wiringPiI2CReadReg8(fd, STATUS_REG) );
	uint8_t reg = STATUS_REG;
	int retVal = i2c->write((uint8_t)LIDAR_LITE_ADRS, &reg,1);
	if (retVal < 0)
		return -1;
	retVal = i2c->read((uint8_t)LIDAR_LITE_ADRS, &reg,1);

	i2c->readyWait();
	if (retVal < 0)
		return -2;
	else
		return reg;
}

void lidar_status_print(unsigned char status) {
if (status != 0)
	trace_printf("STATUS BYTE: 0x%x ", (unsigned int) status);

if (status & STAT_BUSY) trace_printf("busy \n");
if (status & STAT_REF_OVER) trace_printf("reference overflow \n");
if (status & STAT_SIG_OVER) trace_printf("signal overflow \n");
if (status & STAT_PIN) trace_printf("mode select pin \n");
if (status & STAT_SECOND_PEAK) trace_printf("second peak \n");
if (status & STAT_TIME) trace_printf("active between pairs \n");
if (status & STAT_INVALID) trace_printf("no signal \n");
if (status & STAT_EYE) trace_printf(" eye safety \n");
}
//
//// Read a byte from I2C register.  Repeat if not ready
//unsigned char  _read_byte(int fd, int reg)
//{
//return _read_byte_raw(fd, reg, true);
//}
//
//	// Read Lo byte from I2C register.  Repeat if not ready or zero
//unsigned char  _read_byteNZ(int fd, int reg) {
//return _read_byte_raw(fd, reg, false);
//}
//
//// Read byte from I2C register.  Special handling for zero value
//unsigned char  _read_byte_raw(int fd, int reg, bool allowZero) {
//int i;
//unsigned char val;
//delay(1);
//	 while (true) {
//		val = wiringPiI2CReadReg8(fd, reg);
//
//		// Retry on error
//		if (val == ERROR_READ || (val==0 && !allowZero) ) {
//			delay (20) ;		// ms
//		   // if (_dbg) printf(".");
//			if (i++ > 50) {
//			   // Timeout
//			   printf("Timeout");
//			   return (ERROR_READ);
//			   }
//		  }
//		  else return(val);
//	 }
//}
