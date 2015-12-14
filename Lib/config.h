/**
 * @file
 *
 * @brief Quadcopter configuration
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 13, 2015
 *
 */

/** @addtogroup DC9000
 *  @{
 */

/** @defgroup Config Quadcopter configuration
 *  @brief Global configuration options
 *  @{
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//#define DEATH_CHOPPER
#define DISCOVERY_BOARD
#define RX_TIMEOUT_ENABLE

//#define USE_LIDARLITE
#define USE_ULTRASONIC

/*
 * Dev board specific configuration
 */
#if defined DEATH_CHOPPER

#define BOARD Board::DEATH_CHOPPER_9000
#define MOTOR_FRONT_PIN TimerPin::PD14	// PCB P3
#define MOTOR_REAR_PIN  TimerPin::PD12	// PCB P5
#define MOTOR_LEFT_PIN  TimerPin::PC8	// PCB P7
#define MOTOR_RIGHT_PIN TimerPin::PC6	// PCB P9

#ifndef RX_TIMEOUT_ENABLE
#define RX_TIMEOUT_ENABLE
#endif

#elif defined DISCOVERY_BOARD

#define BOARD Board::STM32F4_DISCOVERY
#define MOTOR_FRONT_PIN TimerPin::PD14	// PCB P3
#define MOTOR_REAR_PIN  TimerPin::PD12	// PCB P5
#define MOTOR_LEFT_PIN  TimerPin::PC8	// PCB P7
#define MOTOR_RIGHT_PIN TimerPin::PC6	// PCB P9

#else
#error "No development board defined"
#endif

#define VSENSE_PIN AdcPin::PA2

/*
 * Flight Parameters
 */
#define MAX_ANGLE 20.0f					// Maximum pitch & roll angle [deg]
#define MAX_RATE  180.0f				// Maximum yaw rate [deg/s]
#define V_MIN 0.2f

#define PITCH_KP 6.0f
#define PITCH_KI 0.0f
#define PITCH_KD 0.0f

#define ROLL_KP  6.0f
#define ROLL_KI  0.0f
#define ROLL_KD  0.0f

#define PID_SCALE 55.0f

/*
 * UART RX parameters
 */
#define LOOP_DELAY 10	// Main loop delay in ms
#define TIMEOUT ((int)2.0f / ((float)LOOP_DELAY / (float)1000))

#endif

/** @} Close Config group */
/** @} Close DC9000 group */
