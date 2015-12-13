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
 *   
 */
 
/**
 * @page low_level_peripherals Low-level Peripherals
 *
 * This page gives an overview of the low-level peripherals supported by the Death Chopper 9000 library.
 * It is divided into the following sections:
 * 		- @subpage peripheral_UART
 *
 */

/**
 * @page peripheral_UART UART Configuration
 *
 * The STM32F407 has six (6) hardware USART/UART peripherals. Each port can be connected to several pins. However,
 * The library currently only supports specific pin connections. The supported pin connections are tabulated below.
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
 */

#ifndef __DEATHCHOPPER9000__
#define __DEATHCHOPPER9000__

//#define USE_LIDARLITE
#define USE_ULTRASONIC

// TODO: Add includes

#endif
