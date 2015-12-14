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

#include "LidarLite.h"
#include "PwmTimer.h"
#include "errDC9000.h"

#include "diag/Trace.h"

#ifdef __cplusplus
extern "C" {
#endif
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
void TIM2_IRQHandler(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

// Number of samples to average
#define NUM_SAMPLES 15

static TIM_HandleTypeDef Tim2Handle;
static TIM_IC_InitTypeDef sConfig;
static volatile uint32_t pWidthBuff[NUM_SAMPLES];
static HAL_LockTypeDef pWidthLock = HAL_UNLOCKED;

// Locking Macros
#define _GET_LOCK_RETURN(__LOCK__)                                           \
							do{                                        \
								if((__LOCK__) == HAL_LOCKED)   \
								{                                      \
								   return HAL_BUSY;                    \
								}                                      \
								else                                   \
								{                                      \
								   (__LOCK__) = HAL_LOCKED;    \
								}                                      \
							  }while (0)
#define _GET_LOCK_NORETURN(__LOCK__)                                           \
							do{                                        \
								if((__LOCK__) == HAL_LOCKED)   \
								{                                      \
								   return;                    \
								}                                      \
								else                                   \
								{                                      \
								   (__LOCK__) = HAL_LOCKED;    \
								}                                      \
							  }while (0)
#define _UNLOCK(__LOCK__)												\
							do{											\
								(__LOCK__) = HAL_UNLOCKED;				\
							} while (0)

/** @addtogroup LIDAR_Class Class for reading from the LIDAR Lite
 *  @{
 */

/**
 * @brief Create a LidarLite object
 *
 * This initializes the GPIO pins and configures TIM2_CH1/2 for input capture,
 * Samples are automatically taken via interrupt.
 */
LidarLite::LidarLite() {
	// TIM configuration settings
	Tim2Handle.Instance = TIM2;
	Tim2Handle.Init.Period = 0xFFFFFFFF;
	Tim2Handle.Init.Prescaler = 0;
	Tim2Handle.Init.ClockDivision = 0;
	Tim2Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim2Handle.Init.RepetitionCounter = 0;
	Tim2Handle.State = HAL_TIM_STATE_RESET;

	// Initialize the TIM
	if (HAL_TIM_IC_Init(&Tim2Handle) != HAL_OK) {
		Error_Handler(errDC9000::LIDAR_INIT_ERROR);
	}

	// Channel specific configuration
	// TIM2_CH1 detects rising edges, TIM2_CH2 detects falling edges so pulse width can be measured
	sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sConfig.ICFilter = 0;
	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler(errDC9000::LIDAR_INIT_ERROR);
	}

	sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&Tim2Handle, &sConfig, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler(errDC9000::LIDAR_INIT_ERROR);
	}

	// Use PB5 as Trigger
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the GPIO clock
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Setup the GPIO
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Pull low for continuous measurements
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

	// Start input capture with interrupts
	if (HAL_TIM_IC_Start_IT(&Tim2Handle, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler(errDC9000::LIDAR_INIT_ERROR);
	}
	if (HAL_TIM_IC_Start_IT(&Tim2Handle, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler(errDC9000::LIDAR_INIT_ERROR);
	}
}

/**
 * @brief Calculate raw pulse width
 * @return The average pulse width in TIM counts
 */
float LidarLite::getDistRaw() {
	// Locking since interrupt changes pWidthBuff
	_GET_LOCK_RETURN(pWidthLock);

	uint32_t pWidthSum = 0;

	// Accumulate samples for averaging
	for (int i = 0; i < NUM_SAMPLES; i++) {
		pWidthSum += pWidthBuff[i];
	}

	// Give the lock back so the ISR can take new samples
	_UNLOCK(pWidthLock);

	// Calculate the average
	float dist = (float)pWidthSum / (float)NUM_SAMPLES;

	return dist;
}

/**
 * @brief Calculate the measured distance
 * @return The distance calculated from the average pulse width over NUM_SAMPLES
 */
float LidarLite::getDistIn() {
	// Distance = (high time) / (10 us/cm or 25.4 us/in)
	float dist = getDistRaw() / 84e6f / 25.4e-6f;

	return dist;
}

/** @} Close LIDAR_Class group */

#ifdef USE_LIDARLITE

/** @addtogroup LIDAR_Functions HAL and ISRs
 *  @{
 */

// Silence some warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * @brief MCU-specific initialization for input capture
 *
 * Sets up input capture on TIM2_CH1 and TIM2_CH2 on PB3
 *
 * @param htim
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the TIM2 and GPIOB clocks
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure the GPIO for input capture
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Enable interrupts
	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

#pragma GCC diagnostic pop

/**
 * @brief Timer2 interrupt service routine
 */
void TIM2_IRQHandler() {
	HAL_TIM_IRQHandler(&Tim2Handle);
}

/**
 * @brief Input Capture Edge Detection callback
 * @param htim Pointer to Tim2Handle
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	static uint32_t cntValBuff[4];
	static uint8_t cntValBuffIndex = 0;
	static uint8_t pWidthBuffIndex = 0;

	/* If falling edge; Only need to calculate new width on falling because we are
	 * concerned with positive width and both rise and fall values are latched */
	if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		// Locking
		_GET_LOCK_NORETURN(pWidthLock);

		// Read and store the CCR values
		cntValBuff[cntValBuffIndex++] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);		// Rising edge CNT value
		cntValBuff[cntValBuffIndex++] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);		// Falling edge CNT value

		// Reset circular buffer index
		if (cntValBuffIndex == 4)
			cntValBuffIndex = 0;

		// Calculate new positive pulse width and store in circular history buffer
		if (cntValBuffIndex == 2) {
			if (cntValBuff[1] >= cntValBuff[0]) {
				pWidthBuff[pWidthBuffIndex++] = cntValBuff[1] - cntValBuff[0];
			} else {
//				pWidthBuff[pWidthBuffIndex++] = 4294967296 - cntValBuff[0] + cntValBuff[1];
			}
		} else if (cntValBuffIndex == 0) {
			if (cntValBuff[3] >= cntValBuff[2]) {
				pWidthBuff[pWidthBuffIndex++] = cntValBuff[3] - cntValBuff[2];
			} else {
//				pWidthBuff[pWidthBuffIndex++] = 4294967296 - cntValBuff[2] + cntValBuff[3];
			}
		}

		// Reset circular buffer index
		if (pWidthBuffIndex == NUM_SAMPLES)
			pWidthBuffIndex = 0;

		// Unlocking
		_UNLOCK(pWidthLock);
	}
}

/** @} Close LIDAR_Functions group */

#endif

/** @} Close LIDAR group */
/** @} Close Sensors Group */

