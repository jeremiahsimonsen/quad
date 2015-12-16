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

/** @addtogroup DC9000
 *  @{
 */

#ifndef __DEATHCHOPPER9000__
#define __DEATHCHOPPER9000__

#include "config.h"
#include "Adc.h"
#include "uart.h"
#include "Motor.h"
#include "IMU.h"
#include "LidarLite.h"
#include "HCSR04.h"
#include "pid.h"
#include "pid2.h"
#include "led.h"

/**
 * @brief Global variables
 */
extern led *leds;						///< LED controller

#define FLY_CMD 1
#define DEMO_CMD 2
#define DEMO_MOTOR_TOGGLE 3

/**
 * @brief Quadcopter abstraction
 *
 * This class abstracts everything involved with flying into an extremely
 * simple interface.
 */
class DeathChopper9000 {
private:
	Adc vSense;					///< Adc for sensing battery voltage

	IMU *imu;					///< Inertial measurement unit for orientation sensing

	Motor front;				///< Front motor
	Motor rear;					///< Rear motor
	Motor left;					///< Left motor
	Motor right;				///< Right motor

	pid2 pitch_pid;				///< PID controller for pitch angle
	pid2 roll_pid;				///< PID controller for roll angle

// Rangefinder
#if defined USE_LIDARLITE
	LidarLite rangefinder;		///< Rangefinder for sensing height
#elif defined USE_ULTRASONIC
	HCSR04 rangefinder;			///< Rangefinder for sensing height
#endif

	uint32_t rxTimeout;		///< UART RX timeout counter

	float throttle_cmd;			///< Throttle command from remote control
	float pitch_cmd;			///< Pitch angle command from remote control
	float roll_cmd;				///< Roll angle command from remote control
	float yaw_cmd;				///< Yaw rate command from remote control

	float pitch_y;				///< Measured "output" pitch angle
	float roll_y;				///< Measured "output" roll angle

	float pitch_e;				///< Pitch angle error from command
	float roll_e;				///< Roll angle error from command

	float u_pitch;				///< Output of pitch PID controller
	float u_roll;				///< Output of roll PID controller

	float u_pitch_cmd;			///< Pitch PID controller motor command/throttle
	float u_roll_cmd;			///< Roll PID controller motor command/throttle

	float front_s;				///< Front motor speed
	float rear_s;				///< Rear motor speed
	float left_s;				///< Left motor speed
	float right_s;				///< Right motor speed

	// Private constructors for singleton pattern
	DeathChopper9000();
	DeathChopper9000(DeathChopper9000 const&);
	DeathChopper9000& operator=(DeathChopper9000 const&);

	static DeathChopper9000 *dc9000Instance;

	void fly(void);
	void demo(void);

public:
	static DeathChopper9000* instance();

	void start(void);

	void abort(void);
};

#endif

/** @} Close DC9000 group */
