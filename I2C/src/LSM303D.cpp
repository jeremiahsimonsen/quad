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
	accResolution    = 0.122e-3f;
	magResolution	 = 0.160e-3f;
	enable(init);
}

LSM303D::LSM303D(LSM303D_InitStruct init) {
	address = 0b00111010;

	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	switch(init.afs_config) {
	case LSM_AFS_Config::TWO	: accResolution = 0.061e-3f; break;
	case LSM_AFS_Config::FOUR	: accResolution = 0.122e-3f; break;
	case LSM_AFS_Config::SIX	: accResolution = 0.183e-3f; break;
	case LSM_AFS_Config::EIGHT	: accResolution = 0.244e-3f; break;
	case LSM_AFS_Config::SIXTEEN: accResolution = 0.732e-3f; break;
	default: break;
	}

	switch(init.mfs_config) {
	case LSM_MFS_Config::TWO	: magResolution = 0.080e-3f; break;
	case LSM_MFS_Config::FOUR	: magResolution = 0.160e-3f; break;
	case LSM_MFS_Config::EIGHT	: magResolution = 0.320e-3f; break;
	case LSM_MFS_Config::TWELVE	: magResolution = 0.479e-3f; break;
	default: break;
	}

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
	readAcc();
	readMag();
}

void LSM303D::readAcc(void) {
	accBuffIndicator = i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_A | (1<<7) ), accBuff1, accBuff2, 6);
}

float LSM303D::getAccX(void) {
	if (accBuffIndicator == 1)
		return (float)(accBuff1[1]<<8 | accBuff1[0]) * accResolution;
	else if (accBuffIndicator == 2) {
		return (float)(accBuff2[1]<<8 | accBuff2[0]) * accResolution;
	} else {
		return 0;
	}
}

float LSM303D::getAccY(void) {
	if (accBuffIndicator == 1)
		return (float)(accBuff1[3]<<8 | accBuff1[2]) * accResolution;
	else if (accBuffIndicator == 2) {
		return (float)(accBuff2[3]<<8 | accBuff2[2]) * accResolution;
	} else {
		return 0;
	}
}

float LSM303D::getAccZ(void) {
	if (accBuffIndicator == 1)
		return (float)(accBuff1[5]<<8 | accBuff1[4]) * accResolution;
	else if (accBuffIndicator == 2) {
		return (float)(accBuff2[5]<<8 | accBuff2[4]) * accResolution;
	} else {
		return 0;
	}
}

void LSM303D::readMag(void) {
	magBuffIndicator = i2c->memRead(address, ( (uint8_t)LSM303D_Reg::OUT_X_L_M | (1<<7) ), magBuff1, magBuff2, 6);
}

float LSM303D::getMagX(void) {
	if (magBuffIndicator == 1)
		return (float)(magBuff1[1]<<8 | magBuff1[0]) * magResolution;
	else if (magBuffIndicator == 2) {
		return (float)(magBuff2[1]<<8 | magBuff2[0]) * magResolution;
	} else {
		return 0;
	}
}

float LSM303D::getMagY(void) {
	if (magBuffIndicator == 1)
		return (float)(magBuff1[3]<<8 | magBuff1[2]) * magResolution;
	else if (magBuffIndicator == 2) {
		return (float)(magBuff2[3]<<8 | magBuff2[2]) * magResolution;
	} else {
		return 0;
	}
}

float LSM303D::getMagZ(void) {
	if (magBuffIndicator == 1)
		return (float)(magBuff1[5]<<8 | magBuff1[4]) * magResolution;
	else if (magBuffIndicator == 2) {
		return (float)(magBuff2[5]<<8 | magBuff2[4]) * magResolution;
	} else {
		return 0;
	}
}




