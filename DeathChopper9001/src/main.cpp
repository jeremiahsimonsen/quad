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

#include "DeathChopper9000.h"

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
	DeathChopper9000 *dc9000 = DeathChopper9000::instance();
	dc9000->start();
}
