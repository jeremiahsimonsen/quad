#include "I2C.h"
#include "LPS25H.h"
#include "L3GD20H.h"
#include "LSM303D.h"
#include "IMU.h"
#include "LidarLite.h"
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

void i2c_lowlevel_test(void);
void lps_test(void);
void l3g_test(void);
void lsm_test(void);
void imu_test(void);
void lidar_test(void);
// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.

//	i2c_lowlevel_test();
//	lps_test();
//	l3g_test();
//	lsm_test();
	lidar_test();
}

#pragma GCC diagnostic pop

void i2c_lowlevel_test() {
		uint8_t slave_addr = 0b10111010;
		I2C *i2c = I2C::Instance(i2cPin::PB6, i2cPin::PB9);

		uint8_t data1[3] = {0x0};
		uint8_t data2[3] = {0x0};

		uint8_t on = 0xB4;
		// Power on and set output data rate to 12.5 Hz
		i2c->memWrite(slave_addr, 0x20, &on, 1);

		while(1) {
			int16_t temperature;
			uint8_t x;
#if USE_DOUBLE_BUFFERING
			x = i2c->memRead(slave_addr, 0x2B|(1<<7), data1, data2, 2);	// read temperature

			if (x == 1) {
				temperature = (int16_t) (data1[1]<<8 | data1[0]);
			} else if (x == 2) {
				temperature = (int16_t) (data2[1]<<8 | data2[0]);
			}
#else
			i2c->memRead(slave_addr, 0x2B|(1<<7), data1, 2);			// read temperature

			i2c->readyWait();
			temperature = (int16_t) (data1[1]<<8 | data1[0]);
#endif

			float t = 108.5f + (float)temperature / 480.0f * 1.8f;

			char t_str[50];
			sprintf(t_str,"%f",t);
			trace_printf("Temp: %s\n", t_str);
		}
}

void lps_test() {
	LPS25H lps;

	while (1)
	{
		float t = lps.readTemperatureF();

		char t_str[50];
		sprintf(t_str,"%f",t);
		trace_printf("Temp: %s\n", t_str);
	}
}

void l3g_test() {
	L3GD20H l3g;

	while (1) {
		float x,y,z;

		l3g.read();

		x = l3g.getX();
		y = l3g.getY();
		z = l3g.getZ();

		char t_str[50];
		sprintf(t_str, "X: %f\tY: %f\tZ: %f", x, y, z);
		trace_printf("%s\n", t_str);
	}
}

void lsm_test() {
	LSM303D lsm;

	while (1) {
		float accX, accY, accZ;
		float magX, magY, magZ;

		lsm.read();
//		lsm.readAcc();
//		lsm.readMag();

		accX = lsm.getAccX();
		accY = lsm.getAccY();
		accZ = lsm.getAccZ();

		magX = lsm.getMagX();
		magY = lsm.getMagY();
		magZ = lsm.getMagZ();

		char t_str[50];
		sprintf(t_str, "AccX: %f\tAccY: %f\tAccZ: %f", accX, accY, accZ);
		trace_printf("%s\n", t_str);
		sprintf(t_str, "MagX: %f\tMagY: %f\tMagZ: %f", magX, magY, magZ);
		trace_printf("%s\n\n", t_str);
	}
}

void imu_test() {
	IMU imu;
	float roll, pitch;

	while (1) {
		roll = imu.getRoll();
		pitch = imu.getPitch();

		char str[50];
		sprintf(str, "Roll: %f\tPitch: %f", roll, pitch);
		trace_printf("%s\n", str);
	}
}

void lidar_test() {
	LidarLite lidar;
	int accel;
	while(1) {
		accel = lidar.lidar_read();
		if(accel<0) trace_printf("error sending read");
		trace_printf("%d\n", accel);
	}
}
