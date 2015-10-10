/**
 * @file LSM303D.cpp
 *
 * @brief Class for interfacing with the ST LSM303D 3-axis accelerometer and magnetometer
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 9, 2015
 *
 * Based on lsm303-arduino library from Pololu Corp. See license.txt for details
 *
 */

#include "LSM303D.h"

LSM303D::LSM303D() {
	address = 0b00111010;

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);
}

void LSM303D::enable(void) {

}

void LSM303D::read(void) {

}

void LSM303D::readAcc(void) {

}

int16_t LSM303D::getAccX(void) {

}

int16_t LSM303D::getAccY(void) {

}

int16_t LSM303D::getAccZ(void) {

}

void LSM303D::readMag(void) {

}

int16_t LSM303D::getMagX(void) {

}

int16_t LSM303D::getMagY(void) {

}

int16_t LSM303D::getMagZ(void) {

}




