/**
 * @file
 *
 * @brief Main program
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Nov 21, 2015
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "led.h"

/**
 * @brief Main program
 *
 * Creates a DeathChopper9000 instance and starts it up
 */
void main()
{
	// At this stage the system clock should have already been configured
	// at high speed.

	// Delay to let the IMU initialize
	HAL_Delay(500);

	// Start the quadcopter
	led leds(Board::DEATH_CHOPPER_9000);

	while(1) {
		leds.turnOn(LED::BLUE);
		leds.turnOn(LED::GREEN);
		leds.turnOn(LED::ORANGE);
		leds.turnOn(LED::RED);

//		HAL_Delay(500);
	}
}
