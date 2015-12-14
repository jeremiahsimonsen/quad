/**
 * @file
 *
 * @brief Error handling
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 13, 2015
 *
 */

/** @addtogroup System
 *  @{
 */

/** @addtogroup ERROR
 *  @{
 */

#ifndef ERRDC9000_H_
#define ERRDC9000_H_

/**
 * @brief Death Chopper 9000 errors
 */
enum class errDC9000 {
	FLIPPING,					///< Quadcopter is flipping
	REMOTE_CONTROL,				///< Remote control error
	TIMEOUT_ERROR,				///< Remote control timeout
	UART_INIT_ERROR,			///< UART initialization error
	UART_DEINIT_ERROR,			///< UART de-init error
	UART_IO_ERROR,				///< UART communication error
	I2C_INIT_ERROR,				///< I2C initialization error
	I2C_DEINIT_ERROR,			///< I2C de-init error
	I2C_IO_ERROR,				///< I2C communication error
	LPS_INIT_ERROR,				///< LPS25H initialization error
	LPS_IO_ERROR,				///< LPS25H comm error
	L3G_INIT_ERROR,				///< L3GD20H initialization error
	L3G_IO_ERROR,				///< L3GD20H comm error
	LSM_INIT_ERROR,				///< LSM303D initialization error
	LSM_IO_ERROR,				///< LSM303D comm error
	PWM_INIT_ERROR,				///< PWM initialization error
	LIDAR_INIT_ERROR,			///< LIDAR Lite initialization error
	ULTRASONIC_INIT_ERROR,		///< HC-SR04 initialization error
	ADC_INIT_ERROR,				///< ADC initialization error
	ADC_IO_ERROR				///< ADC read errors
};

void Error_Handler(errDC9000 e);

#endif

/** @} Close ERROR group */
/** @} Close System group */
