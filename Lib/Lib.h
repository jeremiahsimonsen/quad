/**
 * @file
 *
 * @brief Death Chopper 9000 Interface
 *
 * @author Jeremiah Simonsen
 * @author Jasmine Despres
 *
 * @date Dec, 2015
 *
 *   @mainpage Death Chopper 9000 library for ECE 405/6/3 Senior Design Project
 *
 *   This library defines an interface that allows a user to use the
 *   STM32F4-Discovery or Death Chopper 9000 development boards to interface with:
 *   	- Pololu AltIMU-10 v4 Inertial Measurement Unit (IMU)
 *   		- https://www.pololu.com/product/2470
 *   		- 3-axis Gyroscope (L3GD20H)
 *   			- https://www.pololu.com/file/0J731/L3GD20H.pdf
 *   		- 3-axis Accelerometer & 3-axis Magnetometer (LSM303D)
 *   			- https://www.pololu.com/file/0J703/LSM303D.pdf
 *   		- Altimeter (LPS25H)
 *   			- https://www.pololu.com/file/0J761/LPS25H.pdf
 *   	- XBee Pro Series 1 wireless radio module
 *   		- https://www.sparkfun.com/products/8742
 *   	- LIDAR Lite v1 Laser Rangefinder
 *   		- http://pulsedlight3d.com/
 *   	- HC-SR04 Ultrasonic Rangefinder
 *   		- http://www.micropik.com/PDF/HCSR04.pdf
 *   	- Turnigy Plush 18A (or similar) Brushless DC (BLDC) Electronic Speed Controller (ESC)
 *   		- http://www.hobbyking.com/hobbyking/store/__68533__TURNIGY_Plush_18amp_Speed_Controller_AR_Warehouse_.html
 *
 *   The user interface is as follows:
 *
 *   	-# Required include: @code #include "DeathChopper9000.h" @endcode
 *		-# A DeathChopper9000 is created using: @code DeathChopper9000 *dc9000 = DeathChopper9000::instance(); @endcode
 *		-# The quadcopter control code is run by calling: @code dc9000->start(); @endcode
 *
 */

/**
 * @page low_level_peripherals Low-level Peripherals
 *
 * This page gives an overview of the low-level peripherals supported by the Death Chopper 9000 library.
 * It is divided into the following sections:
 * 		- @subpage peripheral_UART
 * 		- @subpage peripheral_I2C
 * 		- @subpage peripheral_ADC
 * 		- @subpage peripheral_PWM
 *
 */

/**
 * @page peripheral_UART UART Configuration
 *
 * The STM32F407 has six (6) hardware USART/UART peripherals. Each port can be connected to several pins. However,
 * the library currently only supports specific pin connections. The supported pin connections are tabulated below.
 *
 * U(S)ART Pin Mappings
 * --------------------
 * | U(S)ARTx |  TX  |  RX  |
 * | -------: | :-:  | :-:  |
 * | USART1   | PA9  | PA10 |
 * | USART2   | PA2  | PA3  |
 * | USART3   | PB10 | PB11 |
 * | UART4    | PC10 | PC11 |
 * | UART5    | PC12 | PD2  |
 * | USART6   | PC6  | PC7  |
 *
 * UART communication is performed via DMA to reduce CPU load. The DMA mappings are tabulated below.
 *
 * U(S)ART DMA Mappings
 * --------------------
 * | U(S)ARTx | Pin | DMA | Stream | Channel |
 * | -------: | :-: | :-: | :----: | :-----: |
 * | USART1   | TX  | 2   |   7    |   4     |
 * |          | RX  | 2   |   5    |   4     |
 * | USART2   | TX  | 1   |   6    |   4     |
 * |          | RX  | 1   |   5    |   4     |
 * | USART3   | TX  | 1   |   3    |   4     |
 * |          | RX  | 1   |   1    |   4     |
 * | UART4    | TX  | 1   |   4    |   4     |
 * |          | RX  | 1   |   2    |   4     |
 * | UART5    | TX  | 1   |   7    |   4     |
 * |          | RX  | 1   |   0    |   4     |
 * | USART6   | TX  | 2   |   6    |   5     |
 * |          | RX  | 2   |   1    |   5     |
 *
 * UART is used to communicate wirelessly via XBee radios. See @ref sensors_XBEE for more information.
 *
 */

/**
 * @page peripheral_I2C I2C Configuration
 *
 * The STM32F407 has 3 hardware i2c peripherals. Each peripheral can be connected to several pins.
 * The supported pin connections are tabulated below.
 *
 * I2C Pin Mappings
 * ----------------
 * | I2Cx | Function | Pin Options |
 * | :--: | :------: | ----------: |
 * | I2C1 |   SCL    |  PB6, PB8   |
 * |      |   SDA	 |  PB7, PB9   |
 * | I2C2 |   SCL    |  PB10, PF1, PH4 |
 * |      |   SDA    |  PB11, PF0, PH5 |
 * | I2C3 |   SCL    |  PA8, PH7   |
 * |      |   SDA    |  PC9, PH8   |
 *
 * I2C communication is performed via DMA to reduce CPU load. The DMA mappings supported are tabulated below.
 *
 * I2C DMA Mappings
 * ----------------
 * | I2Cx | Data Direction | DMA | Stream | Channel |
 * | :--: | :------------: | :-: | :----: | :-----: |
 * | I2C1 |      TX        |  1  |   6    |    1    |
 * |      |      RX        |  1  |   5    |    1    |
 * | I2C2 |      TX        |  1  |   7    |    7    |
 * |      |      RX        |  1  |   3    |    7    |
 * | I2C3 |      TX        |  1  |   4    |    3    |
 * |      |      RX        |  1  |   2    |    3    |
 *
 *
 * I2C is used to read and write from the AltIMU10-v4. See @ref sensors_IMU for more information.
 *
 * @note The LIDAR Lite supports i2c for more advanced configuration and I/O. However, the LIDAR Lite v1 is
 * buggy and does not function properly with the STM32F4, causing i2c bus errors. As such, PWM is used instead.
 * In the future, the library will be made to support LIDAR Lite i2c for using the v2.
 */

/**
 * @page peripheral_ADC ADC Configuration
 *
 * The STM32F407 has 3 12-bit successive approximation ADCs. Each has 19 channels, 16 of which can measure an
 * external signal via GPIO. The channel-GPIO pin mappings are tabulated below.
 *
 * ADC Pin Mappings
 * ----------------
 * | ADC Channel | ADC1 Pin | ADC2 Pin | ADC3 Pin |
 * | :---------: | -------: | -------: | -------: |
 * |     0       |   PA0    |   PA0    |   PA0    |
 * |     1       |   PA1    |   PA1    |   PA1    |
 * |     2       |   PA2    |   PA2    |   PA2    |
 * |     3       |   PA3    |   PA3    |   PA3    |
 * |     4       |   PA4    |   PA4    |   PF6    |
 * |     5       |   PA5    |   PA5    |   PF7    |
 * |     6       |   PA6    |   PA6    |   PF8    |
 * |     7       |   PA7    |   PA7    |   PF9    |
 * |     8       |   PB0    |   PB0    |   PF10   |
 * |     9       |   PB1    |   PB1    |   PF3    |
 * |     10      |   PC0    |   PC0    |   PC0    |
 * |     11      |   PC1    |   PC1    |   PC1    |
 * |     12      |   PC2    |   PC2    |   PC2    |
 * |     12      |   PC3    |   PC3    |   PC3    |
 * |     14      |   PC4    |   PC4    |   PF4    |
 * |     15      |   PC5    |   PC5    |   PF5    |
 *
 * The ADC is used to measure the battery voltage (and current coming soon).
 */

/**
 * @page peripheral_PWM PWM Using General Purpose Timers
 *
 * The STM32F407 has several timers that can be used to generate PWM signals.
 * The timer pin mappings are tabulated below.
 *
 * TIM Pin Mappings
 * ----------------
 * | TIM | CH | Pin Option 1 | Pin Option 2 | Pin Option 3 |
 * | :-: | :-:| ----------:  | -----------: | -----------: |
 * |  1  |  1 | PA8          | PE9          |              |
 * |     |  2 | PA9 		 | PE11 	 	| 			   |
 * |     |  3 | PA10 		 | PE13			|  			   |
 * |     |  4 | PA11		 | PE14 		|  			   |
 * |  2  |  1 | PA15		 |      		|  			   |
 * |     |  2 | PA1 		 | PB3	 		|  			   |
 * |     |  3 | PA2			 | PB10			| 			   |
 * |     |  4 | PA3			 | PB11			|				|
 * |  3  |  1 | PA6			 | PB4			|		PC6		|
 * |     |  2 | PA7			 | PB5			|		PC7		|
 * |     |  3 | 			 | PB0			|		PC8		|
 * |     |  4 | 			 | PB1			|		PC9		|
 * |  4  |  1 | PB6			 | PD12			|				|
 * |     |  2 | PB7			 | PD13			|				|
 * |     |  3 | PB8			 | PD14			|				|
 * |     |  4 | PB9			 | PD15			|				|
 * |  5  |  1 | PA0			 | PH10			|				|
 * |     |  2 | PA1			 | PH11			|				|
 * |     |  3 | PA2			 | PH12			|				|
 * |     |  4 | PA3			 | PI0			|				|
 * |  8  |  1 | PC6			 | PI5			|				|
 * |     |  2 | PC7			 | PI6			|				|
 * |     |  3 | PC8			 | PI7			|				|
 * |     |  4 | PC9			 | PI2			|				|
 * |  9  |  1 | PA2			 | PE5			|				|
 * |     |  2 | PA3			 | PE6			|				|
 * | 10  |  1 | PB8			 | PF6			|				|
 * | 11  |  1 | PB9			 | PF7			|				|
 *
 * The general purpose timers are used for:
 * 		- @ref PwmTimer "PWM" generation for interfacing with @ref Motor "ESCs"
 * 		- PWM input capture for the @ref LIDAR and @ref HCSR04
 */

/**
 * @page sensors High-level components
 *
 * This page gives an overview of the high-level sensors and components that the Death Chopper 9000 uses.
 * It is divided into the following sections:
 * 		- @subpage sensors_IMU
 * 		- @subpage sensors_LIDAR
 * 		- @subpage sensors_HCSR04
 * 		- @subpage sensors_ESC
 * 		- @subpage sensors_XBEE
 */

/**
 * @page sensors_IMU Pololu AltIMU10-v4
 *
 * The Death Chopper 9000 uses a Pololu AltIMU10-v4 10DOF inertial measurement unit for measuring orientation.
 * Useful reference links are below:
 *
 * - Pololu AltIMU-10 v4 Inertial Measurement Unit (IMU)
 *   	- https://www.pololu.com/product/2470
 *   	- 3-axis Gyroscope (L3GD20H)
 *   		- https://www.pololu.com/file/0J731/L3GD20H.pdf
 *   	- 3-axis Accelerometer & 3-axis Magnetometer (LSM303D)
 *   		- https://www.pololu.com/file/0J703/LSM303D.pdf
 *   	- Altimeter (LPS25H)
 *   		- https://www.pololu.com/file/0J761/LPS25H.pdf
 * - Orientation calculation
 * 		- Accelerometer based angle calculation: http://www.hobbytronics.co.uk/accelerometer-info
 * 		- Accelerometer and gyro integration: http://www.hobbytronics.co.uk/accelerometer-gyro
 * 		- Complementary filtering
 * 			- http://www.instructables.com/id/PCB-Quadrotor-Brushless/?ALLSTEPS#step15
 * 			- http://www.etotheipiplusone.net/?p=1081
 * 			- http://dspace.mit.edu/handle/1721.1/17007
 *
 * The angles relative to the X and Y axes of the accelerometer are calculated as:
 * 		\f[
 * 			\theta_x = tan^{-1}\Bigg(\frac{a_x}{\sqrt{a_y^2 + a_z^2}}\Bigg)~~~~~~~and~~~~~~~\theta_y = tan^{-1}\Bigg(\frac{a_y}{\sqrt{a_x^2 + a_z^2}}\Bigg)
 * 		\f]
 * where \f$ \theta_x \f$ and \f$ \theta_y \f$ are the angles relative to the X and Y axes of the accelerometer, respectively,
 * \f$ a_x \f$, \f$ a_y \f$, and \f$ a_z \f$ are the accelerations in the X, Y, and Z directions measured by the accelerometer.
 *
 * @image html feedback_with_sensor.png "Angle calculation filtering/feedback diagram"
 * @image latex feedback_with_sensor.eps "Angle calculation filtering/feedback diagram"
 */

/**
 * @page sensors_LIDAR LIDAR Lite laser rangefinder
 *
 * The Death Chopper 9000 library supports using a LIDAR Lite laser rangefinder for measuring
 * distance/height/altitude. The LIDAR Lite is available at http://pulsedlight3d.com/
 */

/**
 * @page sensors_HCSR04 HC-SR04 Ultrasonic rangefinder
 *
 * The Death Chopper 9000 library supports using an @ref HCSR04 for measuring distance/height/altitude.
 * The datasheet can be found at http://www.micropik.com/PDF/HCSR04.pdf
 */

/**
 * @page sensors_ESC Turnigy Plush BLDC ESC
 *
 * The Death Chopper 9000 uses Turnigy Plush 18A Brushless DC (BLDC) Electronic Speed Controllers (ESC)
 * for motor control. Any similar ESC will also work with the library. The ESC used is available at
 * http://www.hobbyking.com/hobbyking/store/__68533__TURNIGY_Plush_18amp_Speed_Controller_AR_Warehouse_.html
 */

/**
 * @page sensors_XBEE XBee Pro Series 1
 *
 * The Death Chopper 9000 uses two XBee Pro Series 1 wireless radio modules for remote control. They operate
 * at 2.4 GHz using the Zigbee protocol. The specific model used is available at
 * https://www.sparkfun.com/products/8742
 */


