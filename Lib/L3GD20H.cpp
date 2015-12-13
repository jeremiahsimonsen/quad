/**
 * @file
 *
 * @brief Class for interfacing with the ST L3GD20H 3-axis gyroscope
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 8, 2015
 *
 * Based on lps-arduino library from Pololu Corp. See license.txt for details
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup IMU
 *  @{
 */

/** @addtogroup L3GD20H
 *  @{
 */

#include "L3GD20H.h"

/**
 * @brief Instantiates an object using default I2C pins and configures the sensor to default
 */
L3GD20H::L3GD20H(void)
	: gx(PREFILTER_TAU), gy(PREFILTER_TAU), gz(PREFILTER_TAU)		// complementary
//	: gx(), gy(), gz()												// IIR or FIR
{
	// Get a pointer to the logger
	log = logger::instance();

	// Initialize members
	address = 0b11010110;
	dt = prevTick = 0;
	xOffset = yOffset = zOffset = 0.0f;

	// Get a pointer to the I2C instance
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Default gyro configuration
	L3GD20H_InitStruct init;
	init.odr_bw_config 	= 	L3GD_ODR_BW_Config::EIGHT;	// 200 Hz ODR, 12.5 Hz BW
	init.hpm_config 	= 	L3GD_HPM_Config::THREE;		// Normal mode
	init.hpcf_config 	= 	L3GD_HPCF_Config::FIVE;		// 1 Hz cut-off frequency for 200 Hz ODR
	init.fs_config 		= 	L3GD_FS_Config::MEDIUM;		// 500 dps full-scale
	resolution = 17.50e-3f;

	// Enable and configure the gyro
	enable(init);
}

/**
 * @brief Configures the sensor according to the parameters
 * @param init Sensor configuration parameters
 */
L3GD20H::L3GD20H(L3GD20H_InitStruct init)
	: gx(PREFILTER_TAU), gy(PREFILTER_TAU), gz(PREFILTER_TAU)		// complementary
//	: gx(), gy(), gz()												// IIR or FIR
{
	// Get a pointer to the logger
	log = logger::instance();

	// Initialize members
	address = 0b11010110;
	dt = prevTick = 0;
	xOffset = yOffset = zOffset = 0.0f;

	// Gyro configuration
	switch(init.fs_config) {
	case L3GD_FS_Config::LOW	: resolution = 8.75e-3f;  break;
	case L3GD_FS_Config::MEDIUM	: resolution = 17.50e-3f; break;
	case L3GD_FS_Config::HIGH	: resolution = 70.00e-3f; break;
	default: break;
	}

	// Get a pointer to the I2C instance
	i2c = I2C::Instance(I2C_SCL_PIN, I2C_SDA_PIN);

	// Enable and configure the gyroscope
	enable(init);
}

/**
 * @brief Configures the sensor operation
 */
void L3GD20H::enable(L3GD20H_InitStruct init) {
	// Set data rate and bandwidth; Enable sensor for 3-axis operation
	uint8_t buf = (uint8_t)(L3GD_CTRL1_DR(L3GD_ODR_BW_Config_DR(init.odr_bw_config))
					| L3GD_CTRL1_BW(L3GD_ODR_BW_Config_BW(init.odr_bw_config))
					| L3GD_CTRL1_PD_MASK
					| L3GD_CTRL1_ZEN_MASK
					| L3GD_CTRL1_YEN_MASK
					| L3GD_CTRL1_XEN_MASK);
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL1, &buf, 1);

	// Set low output data rate configuration
	buf = L3GD_LOW_ODR_Low_ODR(L3GD_ODR_BW_Config_LOW_ODR(init.odr_bw_config));
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::LOW_ODR, &buf, 1);

	// Set high-pass filter mode and high-pass filter cutoff frequency
	buf = (uint8_t)(L3GD_CTRL2_HPM(init.hpm_config)
					| L3GD_CTRL2_HPCF(init.hpcf_config));
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL2, &buf, 1);

	// Set full scale
	buf = (uint8_t)(L3GD_CTRL4_FS(init.fs_config)//);
			| L3GD_CTRL4_BDU_MASK);
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL4, &buf, 1);

	// Enable the high-pass filter
	buf = (uint8_t)(L3GD_CTRL5_HPen_MASK);
	i2c->memWrite(address, (uint8_t)L3GD20H_Reg::CTRL5, &buf, 1);

	// Initialize PA4 for externally measuring sampling frequency
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.Pin 	= GPIO_PIN_4;
//	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull	= GPIO_NOPULL;
//	GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// TIM initialization for alternative sample time measurement
	TimHandle.Instance = TIM6;
	TimHandle.Init.Period = 0xFFFF;
	TimHandle.Init.Prescaler = 1;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	TimHandle.State = HAL_TIM_STATE_RESET;

	// Initialize the timer
	if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
		// TODO: Error
		while(1);
	}

	// Start the timer
	if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK) {
		// TODO: Error
		while(1);
	}

	// Calibrate the sensor to eliminate offset error
	calibrate();
}

/**
 * @brief Basic calibration to eliminate offset errors
 *
 * Initializes the offset members so offset error can be reduced/eliminated.
 * The sensor must remain stationary during calibration.
 */
void L3GD20H::calibrate() {
	uint8_t samples = 128;
	float gx_offset = 0.0f, gy_offset = 0.0f, gz_offset = 0.0f;

	read();	// Initial read so values are correct
	// Take some readings and accumulate
	for (int i = 0; i < samples; i++) {
		read();
		gx_offset += getXFiltered();
		gy_offset += getYFiltered();
		gz_offset += getZFiltered();
		HAL_Delay(20);
	}

	// Average the offsets
	gx_offset = gx_offset / (float)samples;
	gy_offset = gy_offset / (float)samples;
	gz_offset = gz_offset / (float)samples;

	// Save the offsets
	xOffset = gx_offset;
	yOffset = gy_offset;
	zOffset = gz_offset;
}

/**
 * @brief Called by HAL_TIM_Base_Init. Enable TIM clock
 * @param htim Pointer to TimHandle
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	__HAL_RCC_TIM6_CLK_ENABLE();
}

/**
 * @brief Measure the sample time/rate
 * @return The time between samples in s
 */
float L3GD20H::getDT() {
	return (float)dt / 16e6f;
}

/**
 * @brief Initiates a read of all 3 axes
 * @return The return value of I2C::memRead()
 * 			0 on success, -1 on HAL_ERROR
 * 			Updates data in gyroBuff
 */
int8_t L3GD20H::read(void) {
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);

	// Grab the counter value
	uint32_t tmp = __HAL_TIM_GetCounter(&TimHandle);

	// Calculate the sample time
	if (tmp >= prevTick) {
		dt = tmp - prevTick;
	} else {
		dt = 65536 - prevTick + tmp;
	}
	// Store counter value
	prevTick = tmp;

	// Read from the gyro registers
	return i2c->memRead(address, ( (uint8_t)L3GD20H_Reg::OUT_X_L | (1<<7) ), gyroBuff, 6);
}

/**
 * @brief  Function to get the rate of angular rotation about the x axis (pitch)
 * @return Raw rate of angular rotation about the x axis (pitch) (register values)
 */
int16_t L3GD20H::getXRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw rate of angular rotation about the x axis (pitch)
	return -(int16_t)(gyroBuff[1]<<8 | gyroBuff[0]);
}

/**
 * @brief  Function to get the rate of angular rotation about the x axis (pitch)
 * @return Rate of angular rotation about the x axis (pitch) in degrees per second (dps)
 */
float L3GD20H::getX() {
	float x = (float)getXRaw() * resolution - xOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Gx %f\n\r", x);
	log->log(buff);
#endif
	return x;
}

/**
 * @brief  Function to get the filtered rate of angular rotation about the x axis (pitch)
 * @return Filtered angular velocity about the x axis (pitch) [dps]
 */
float L3GD20H::getXFiltered() {
	float32_t x = getX();
	float xf = gx.filterSample(&x);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Gxf %f\n\r", xf);
	log->log(buff);
#endif
	return xf;
}

/**
 * @brief  Function to get the rate of angular rotation about the y axis (roll)
 * @return Raw rate of angular rotation about the y axis (roll) (register values)
 */
int16_t L3GD20H::getYRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw rate of angular rotation about the y axis (roll)
	return -(int16_t)(gyroBuff[3]<<8 | gyroBuff[2]);
}

/**
 * @brief  Function to get the rate of angular rotation about the y axis (roll)
 * @return Rate of angular rotation about the y axis (roll) in degrees per second (dps)
 */
float L3GD20H::getY() {
	float y = (float)getYRaw() * resolution - yOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Gy %f\n\r", y);
	log->log(buff);
#endif
	return y;
}

/**
 * @brief  Function to get the filtered rate of angular rotation about the y axis (roll)
 * @return Filtered angular velocity about the y axis (roll) [dps]
 */
float L3GD20H::getYFiltered() {
	float32_t y = getY();
	float yf = gy.filterSample(&y);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Gyf %f\n\r", yf);
	log->log(buff);
#endif
	return yf;
}

/**
 * @brief  Function to get the rate of angular rotation about the z axis (yaw)
 * @return Raw rate of angular rotation about the z axis (yaw) (register values)
 */
int16_t L3GD20H::getZRaw(void) {
	// Wait for the measurement to be ready
	i2c->readyWait();

	// Return the raw rate of angular rotation about the z axis (yaw)
	return (int16_t)(gyroBuff[5]<<8 | gyroBuff[4]);
}

/**
 * @brief  Function to get the rate of angular rotation about the z axis (yaw)
 * @return Rate of angular rotation about the z axis (yaw) in degrees per second (dps)
 */
float L3GD20H::getZ() {
	float z = (float)getZRaw() * resolution - zOffset;
#ifdef LOG_RAW
	char buff[100];
	sprintf(buff, "Gz %f\n\r", z);
	log->log(buff);
#endif
	return z;
}

/**
 * @brief  Function to get the filtered rate of angular rotation about the z axis (yaw)
 * @return Filtered angular velocity about the z axis (yaw) [dps]
 */
float L3GD20H::getZFiltered() {
	float32_t z = getZ();
	float zf = gz.filterSample(&z);
#ifdef LOG_PREFILTERED
	char buff[100];
	sprintf(buff, "Gzf %f\n\r", zf);
	log->log(buff);
#endif
	return zf;
}

/** @} Close L3GD20H group */
/** @} Close IMU group */
/** @} Close Sensors Group */
