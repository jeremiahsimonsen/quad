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

	LSM303D_InitStruct init;
	init.aodr_config = LSM_AODR_Config::EIGHT;		// 200 Hz Accelerometer output data rate
	init.abw_config  = LSM_ABW_Config::ONE;			// 773 Hz Acceleration anti-alias filter bandwidth
	init.afs_config  = LSM_AFS_Config::FOUR;		// ±4g Acceleration full-scale
	init.modr_config = LSM_MODR_Config::FIVE;		// 50 Hz Magnetometer output data rate
	init.mres_config = LSM_MRES_Config::HIGH;		// Magnetometer high-resolution mode
	init.mfs_config  = LSM_MFS_Config::FOUR;		// ± 4 gauss Magnetometer full-scale
	init.md_config   = LSM_MD_Config::CONTINUOUS;	// Magnetic sensor continuous mode
	enable(init);
}

LSM303D::LSM303D(LSM303D_InitStruct init) {
	address = 0b00111010;

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	enable(init);
}

void LSM303D::enable(LSM303D_InitStruct init) {
	uint8_t buf;

	// Set Accelerometer output data rate; Enable 3 axis acc operation
	buf = LSM303D_CTRL1_AODR(init.aodr_config)
			| LSM303D_CTRL1_AXEN_MASK
			| LSM303D_CTRL1_AYEN_MASK
			| LSM303D_CTRL1_AZEN_MASK;
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL1, &buf, 1);

	// Set accelerometer anti-alias filter bandwidth and full-scale
	buf = LSM303D_CTRL2_ABW(init.abw_config)
			| LSM303D_CTRL2_AFS(init.afs_config);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL2, &buf, 1);

	// Set magnetometer resolution and output data rate
	buf = LSM303D_CTRL5_M_RES(init.mres_config)
			| LSM303D_CTRL5_M_ODR(init.modr_config);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL5, &buf, 1);

	// Set magnetic full-scale
	buf = LSM303D_CTRL6_MFS(init.mfs_config);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL6, &buf, 1);

	// Set magnetic sensor mode
	buf = LSM303D_CTRL7_MD(init.md_config);
	i2c->memWrite(address, (uint8_t)LSM303D_Reg::CTRL7, &buf, 1);
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




