/**
 * @file
 *
 * @brief Class for interacting with the HC-SR04 ultrasonic rangefinder
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 10, 2015
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup HCSR04
 *  @{
 */

#include "diag/Trace.h"

#include "PwmTimer.h"

#ifndef HCSR04_H_
#define HCSR04_H_

/**
 * @brief Class for measuring distance using the HC-SR04 ultrasonic rangefinder
 *
 * The sensor is triggered by generating a &ge 10 &mu s pulse on the TRIG pin.
 * This is done using PWM at a frequency of 50 Hz to obtain a constant sample
 * rate.
 *
 * The measured distance is proportional to the width of a pulse on the ECHO pin.
 * PWM input capture with two (2) channels - one triggering on rising edges, the
 * other on falling - is used to measure this waveform. The distance calculation
 * is performed in an ISR and multiple samples are stored in a circular data
 * buffer to allow averaging.
 */
class HCSR04 {
public:
	HCSR04(void);

	float getDistRaw(void);
	float getDistIn(void);
};

#endif

/** @} Close HCSR04 group */
/** @} Close Sensors Group */
