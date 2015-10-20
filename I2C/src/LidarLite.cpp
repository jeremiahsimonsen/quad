/*
 * LidarLite.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: Jasmine
 */


#include "LidarLite.h"

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
	int hiVal, loVal;
	unsigned char pdata = MEASURE_VAL;

	// send "measure" command
	if(i2c->memWrite((uint8_t)LIDAR_LITE_ADRS, (uint8_t)MEASURE_REG, &pdata, 1)<0){
		return -1;
	}

	// Read second byte and append with first
	loVal = i2c->memRead((uint8_t)LIDAR_LITE_ADRS, (uint8_t)DISTANCE_REG_LO, &pdata, 1);
	loVal = pdata;
	//loVal = _read_byteNZ(fd, DISTANCE_REG_LO) ;
	//if (_dbg) printf(" Lo=%d\n", loVal);

	// read first byte
	hiVal = i2c->memRead((uint8_t)LIDAR_LITE_ADRS, (uint8_t)DISTANCE_REG_HI, &pdata, 1);
	hiVal = pdata;
	//hiVal = _read_byte(fd, DISTANCE_REG_HI) ;
	//if (_dbg) printf ("Hi=%d ", hiVal);

	return ((hiVal<<8)|loVal);
}
//
//unsigned char lidar_version(int fd) {
//		return( (unsigned char) _read_byteNZ(fd, VERSION_REG) );
//		}
//
//unsigned char lidar_status(int fd) {
//		return( (unsigned char) wiringPiI2CReadReg8(fd, STATUS_REG) );
//		}
//
//void lidar_status_print(unsigned char status) {
//if (status != 0)
//	printf("STATUS BYTE: 0x%x ", (unsigned int) status);
//
//if (status & STAT_BUSY) printf("busy \n");
//if (status & STAT_REF_OVER) printf("reference overflow \n");
//if (status & STAT_SIG_OVER) printf("signal overflow \n");
//if (status & STAT_PIN) printf("mode select pin \n");
//if (status & STAT_SECOND_PEAK) printf("second peak \n");
//if (status & STAT_TIME) printf("active between pairs \n");
//if (status & STAT_INVALID) printf("no signal \n");
//if (status & STAT_EYE) printf(" eye safety \n");
//}
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
