/**
 * @file HCSR04.h
 *
 * @brief Class for interacting with the HC-SR04 ultrasonic rangefinder
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Dec 10, 2015
 *
 */

#include "diag/Trace.h"

#include "PwmTimer.h"

#ifndef HCSR04_H_
#define HCSR04_H_

class HCSR04 {
private:
//	TIM_HandleTypeDef TimHandle;

public:
	HCSR04(void);

	float getDistRaw(void);
	float getDistIn(void);
};

#endif
