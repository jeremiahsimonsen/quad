/**
 * @file
 *
 * @brief Death Chopper 9000
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 13, 2015
 *
 */

/** @defgroup DC9000 Death Chopper 9000
 *  @brief Abstraction for the quadcopter
 *  @{
 */

#include "DeathChopper9000.h"
#include "errDC9000.h"

// Global DeathChopper9000 instance
DeathChopper9000* DeathChopper9000::dc9000Instance = NULL;

led *leds = NULL;

/**
 * @brief Obtain a Death Chopper 9000
 * @return Pointer to the DeathChopper9000 singleton instance
 */
DeathChopper9000* DeathChopper9000::instance() {
	// If the DC9000 has not been created, create it
	if (dc9000Instance == NULL) {
		dc9000Instance = new DeathChopper9000();
	}

	// Return point to the DC9000
	return dc9000Instance;
}

/**
 * @brief Construct a DeathChopper9000 object
 *
 * Initializes all of the required peripherals:
 * 		- Adc for voltage sensing
 * 		- @ref Motor "Motors"
 * 		- @ref LED "LEDs"
 * 		- @ref UART "UART" for remote control via XBee
 * 		- @ref IMU "IMU" for orientation sensing
 * 			- @ref LPS25H "LPS25H" Altimeter
 * 			- @ref L3GD20H "L3GD20H" Gyroscope
 * 			- @ref LSM303D "LSM303D" Accelerometer/Magnetometer
 * 				- @ref I2C
 *
 * Initializes signal processing and feedback control:
 * 		- @ref PID "PID" controllers
 * 		- @ref PREFILTER
 * 		- @ref COMPFILTER
 */
DeathChopper9000::DeathChopper9000()
	: vSense(VSENSE_PIN),
	  front(MOTOR_FRONT_PIN), rear(MOTOR_REAR_PIN),
	  left(MOTOR_LEFT_PIN), right(MOTOR_RIGHT_PIN),
	  pitch_pid(PITCH_KP, PITCH_KI, PITCH_KD),
	  roll_pid(ROLL_KP, ROLL_KI, ROLL_KD),
	  rangefinder()

{
	/*
	 * Initialize global variables
	 */

	// Initialize the LED controller
	leds = new led(BOARD);

	// Initialize the UART and say hello
	init_USART(3, 6, 57600, UART_WORDLENGTH_9B, UART_STOPBITS_1, UART_PARITY_EVEN);
	char txBuff[] = "Death Chopper 9000 reporting for duty.\n\r";
	usart_transmit((uint8_t *)txBuff);
	usart_receive_begin();

	// Gyroscope settings
	L3GD20H_InitStruct gyroConfig;
	gyroConfig.fs_config = L3GD_FS_Config::MEDIUM;
	gyroConfig.hpcf_config = L3GD_HPCF_Config::THREE;
	gyroConfig.hpm_config = L3GD_HPM_Config::THREE;
	gyroConfig.odr_bw_config = L3GD_ODR_BW_Config::EIGHT;

	// Accelerometer settings
	LSM303D_InitStruct accelConfig;
	accelConfig.aodr_config = LSM_AODR_Config::SIX;
	accelConfig.abw_config = LSM_ABW_Config::FOUR;
	accelConfig.afs_config = LSM_AFS_Config::FOUR;
	accelConfig.modr_config = LSM_MODR_Config::SIX;
	accelConfig.mres_config = LSM_MRES_Config::HIGH;
	accelConfig.mfs_config = LSM_MFS_Config::FOUR;
	accelConfig.md_config = LSM_MD_Config::CONTINUOUS;

	// Initialize the IMU
	imu = new IMU(gyroConfig, accelConfig);

	/*
	 * Initialize member variables
	 */
	rxTimeout = 0;
	throttle_cmd = pitch_cmd = roll_cmd = yaw_cmd = 0.0f;
	pitch_y = roll_y = 0.0f;
	pitch_e = roll_e = 0.0f;
	u_pitch = u_roll = 0.0f;
	u_pitch_cmd = u_roll_cmd = 0.0f;
	front_s = rear_s = left_s = right_s = 0.0f;
}

/**
 * @brief Determine the desired mode of operation
 *
 * Toggle LEDs to indicate wait status. Waits until a mode command is received
 * and calls the appropriate routine.
 */
void DeathChopper9000::start() {
	uint8_t *buff = NULL;

	// Turn an LED on so they are opposites
	leds->turnOn(LED::RED);

	while (1) {
		// Toggle LEDs to indicate wait status
		leds->toggle(LED::GREEN);
		leds->toggle(LED::RED);

		// Check if a mode command has been received
		buff = usart_read();
		if (buff != NULL) {
			// Check if demo command
			if (buff[0] == DEMO_CMD && buff[1] == DEMO_CMD &&
				buff[2] == DEMO_CMD && buff[3] == DEMO_CMD &&
				buff[4] == DEMO_CMD && buff[5] == DEMO_CMD)
			{
				demo();
			}

			// Check if fly command
			else if (buff[0] == FLY_CMD && buff[1] == FLY_CMD &&
				buff[2] == FLY_CMD && buff[3] == FLY_CMD &&
				buff[4] == FLY_CMD && buff[5] == FLY_CMD)
			{
				fly();
			}
		}

		// Delay so LED blinking is visible
		HAL_Delay(100);
	}
}

/**
 * @brief Fly the quadcopter using remote control
 *
 * @note Runs forever unless error occurs
 *
 * Receives remote control commands via UART (XBee). Measures orientation
 * (acclerometer & gyro data is pre- and complementary filtered). Performs
 * PID feedback control and adjusts motor speeds. Transmits pitch, roll, height
 * and battery voltage back to the remote.
 */
void DeathChopper9000::fly() {
	uint8_t *readBuff = NULL;
	uint32_t iter = 0;

	// Turn all LEDs off to make sure only the running light blinks
	leds->turnOff(LED::BLUE);
	leds->turnOff(LED::GREEN);
	leds->turnOff(LED::ORANGE);
	leds->turnOff(LED::RED);

	// Run forever
	while (1) {
		// Toggle running light
		leds->toggle(LED::BLUE);

		// See if a new remote control command has arrived yet
		readBuff = usart_read();
		if (readBuff != NULL) {
			// Check for packet errors
			if (readBuff[0] != START || readBuff[5] != STOP) {
				Error_Handler(errDC9000::TIMEOUT_ERROR);
			}

			// Calculate desired commands
			throttle_cmd = (float)readBuff[1] / (253.0f / MAX_SPEED);
			pitch_cmd 	 = ((float)readBuff[2] - 127.0) / 127.0f * MAX_ANGLE;
			roll_cmd 	 = ((float)readBuff[3] - 127.0) / 127.0f * MAX_ANGLE;
			yaw_cmd 	 = ((float)readBuff[4] - 127.0) / 127.0f * MAX_RATE;

			rxTimeout = 0;
		}
#ifdef RX_TIMEOUT_ENABLE
		// If no remote control, eventually timeout
		else {
			rxTimeout++;
			if (rxTimeout >= TIMEOUT) {
				Error_Handler(errDC9000::REMOTE_CONTROL);
			}
		}
#endif
		// Measure the height
		float h = rangefinder.getDistIn();

		// Measure and calculate the battery voltage
		uint32_t vRaw = vSense.read();
		float v = (float)vRaw * 3.0f / 4096.0f / 63.69e-3f;

		// Measure the "output" angles
		imu->getRollPitch(&roll_y, &pitch_y);

		if (abs(roll_y) >= MAX_ANGLE || abs(pitch_y) >= MAX_ANGLE) {
			Error_Handler(errDC9000::FLIPPING);
		}

		// Calculate the errors
		pitch_e = pitch_cmd - pitch_y;
		roll_e  = roll_cmd - roll_y;

		// Calculate PID control outputs
//		u_pitch = pitch_pid.calculate(pitch_e);
//		u_roll  = roll_pid.calculate(roll_e);
		u_pitch = pitch_pid.calculate(pitch_e, LOOP_DELAY);
		u_roll  = roll_pid.calculate(roll_e, LOOP_DELAY);

		// Convert to motor commands
		u_pitch_cmd = u_pitch / PID_SCALE;
		u_roll_cmd  = u_roll  / PID_SCALE;

		// Calculate motor speeds
		front_s = throttle_cmd - u_pitch_cmd;// - yaw_cmd;
		rear_s  = throttle_cmd + u_pitch_cmd;// - yaw_cmd;
		right_s = throttle_cmd - u_roll_cmd;//  + yaw_cmd;
		left_s  = throttle_cmd + u_roll_cmd;//  + yaw_cmd;

		front.setSpeed(front_s);
		rear.setSpeed(rear_s);
		left.setSpeed(left_s);
		right.setSpeed(right_s);

		// Occasionally transmit information - avoids overwhelming UART port
		if (iter % 10 == 0) {
			char txBuff2[100];
//			sprintf(txBuff2, "%f\t%f\n\r", pitch_y, roll_y);
			sprintf(txBuff2, "%f %f %f %f\n", pitch_y, roll_y, h, v);
			usart_transmit((uint8_t *)txBuff2);
		}

		// Delay for fixed rate
		iter++;
		HAL_Delay(LOOP_DELAY);
	}
}

/**
 * @brief Demonstrate that the project meets contract
 *
 * @note Runs forever unless errors occur
 *
 * Measures the pitch and roll angles, height, and battery voltage.
 * Transmits measured quantities via UART/XBee. If motors are enabled (toggle
 * by pressing 'x' button on remote control), adjusts speed of left and right
 * motors based on roll angle.
 */
void DeathChopper9000::demo() {
	uint8_t *buff = NULL;
	uint32_t iter = 0;
	bool enableMotors = false;

	// Turn all LEDs off to make sure only the running light blinks
	leds->turnOff(LED::BLUE);
	leds->turnOff(LED::GREEN);
	leds->turnOff(LED::ORANGE);
	leds->turnOff(LED::RED);

	// Run forever
	while(1) {
		/*
		 * Check if data has been read from UART to see if motors should be
		 * enabled or not
		 */
		buff = usart_read();
		if (buff != NULL) {
			if (buff[0] == DEMO_MOTOR_TOGGLE && buff[1] == DEMO_MOTOR_TOGGLE &&
				buff[2] == DEMO_MOTOR_TOGGLE && buff[3] == DEMO_MOTOR_TOGGLE &&
				buff[4] == DEMO_MOTOR_TOGGLE && buff[5] == DEMO_MOTOR_TOGGLE)
			{
				enableMotors = !enableMotors;
			}
		}

		// Toggle flight mode running light
		leds->toggle(LED::ORANGE);

		// Measure the "output" angles
		imu->getRollPitch(&roll_y, &pitch_y);

		// Measure the height
		float height = rangefinder.getDistIn();

		// Measure and calculate the battery voltage
		uint32_t vRaw = vSense.read();
		float v = (float)vRaw * 3.0f / 4096.0f / 63.69e-3f;

		// Calculate speed of motors based on orientation
		float speed = (roll_y + 90.0f) / 180.0f * DEMO_MAX_SPEED;

		// Set the motor speeds if motors are enabled
		if (enableMotors == true) {
			right.setSpeed(speed);
			left.setSpeed(DEMO_MAX_SPEED - speed);
		} else {
			right.setSpeed(0.0f);
			left.setSpeed(0.0f);
		}

		// Occasionally transmit information to avoid overwhelming UART port
		if (iter % 10 == 0) {
			char txBuff[100];
			sprintf(txBuff, "%f %f %f %f\n", pitch_y, roll_y, height, v);
			usart_transmit((uint8_t *)txBuff);
		}

		// Delay for fixed rate
		iter++;
		HAL_Delay(LOOP_DELAY);
	}
}

/**
 * @brief Kill all motors
 *
 * This stops all motors instantly. This is to prevent an out of control
 * Death Chopper. To avoid damage when flying, this will eventually be
 * feedback controlled using the rangefinder to slowly lower the quadcopter.
 *
 * @todo Implement slow descent for auto-land
 */
void DeathChopper9000::abort() {
	front.setSpeed(0.0f);
	rear.setSpeed(0.0f);
	left.setSpeed(0.0f);
	right.setSpeed(0.0f);
	while(1);
}

/** @} Close DC9000 group */
