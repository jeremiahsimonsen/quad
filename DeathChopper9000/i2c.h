/*
 * author@jasmine despres
 * name@I2C.h
 * description@header file to manage i2c functionality
 * date@09-14-15
 */



#ifndef I2C_H
#define I2C_H

class I2C
{
private:
	I2C() {	}
	~I2C();
public:
	int 	init(void);
	int 	startCondition(void);
	int 	stop(void);
	char 	sendByte(void);
	char 	readByte(void);
};