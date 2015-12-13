/**
 * @file
 *
 * @brief Class for interacting with the LIDAR Lite laser rangefinder
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Oct 15, 2015
 *
 */

/** @addtogroup Sensors
 *  @{
 */

/** @addtogroup LIDAR
 *  @{
 */

#include "I2C.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"

#ifndef LIDARLITE_H
#define LIDARLITE_H

/**
 * @brief Class for reading from the LIDAR Lite laser rangefinder
 *
 * The LIDAR Lite has two interfaces - i2c and PWM. i2c is buggy for the v1,
 * so PWM is used. A pull-down resistor on the MODE pin causes the line to be
 * idle low, telling the LIDAR to take a measurement. It then pulses MODE high
 * for 10 &mu s / cm.
 *
 * The pulse is measured using input capture on PB3. PB5 is connected to the
 * pull-down resistor to allow for starting and stopping of distance measurements.
 */
class LidarLite {
public:
	LidarLite(void);

	float getDistRaw(void);
	float getDistIn(void);
};

#endif // LIDARLITE_H

/** @} Close LIDAR group */
/** @} Close Sensors Group */
