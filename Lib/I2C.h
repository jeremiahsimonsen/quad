/**
 * @file
 *
 * @brief Low level i2c class
 *
 * @author Jeremiah Simonsen
 * @author Jasmine Despres
 *
 * @date Sep 27, 2015
 *
 */

/** @addtogroup Peripherals
 *  @{
 */

/** @addtogroup I2C
 *  @{
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

/** @addtogroup I2C_Defines Definitions
 *  @brief I2C DMA attribute definitions
 *  @{
 */

/** @addtogroup I2C_Defines_I2C1 I2C1 Defines
 *  @brief DMA attributes for the i2c1 hardware peripheral
 *  @{
 */

/*
 * Definitions for I2C1
 */
#define I2C1_TX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2C1_TX_DMA_STREAM              DMA1_Stream6
#define I2C1_RX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2C1_RX_DMA_STREAM              DMA1_Stream5

#define I2C1_DMA_TX_IRQn                DMA1_Stream6_IRQn
#define I2C1_DMA_RX_IRQn                DMA1_Stream5_IRQn
#define I2C1_DMA_TX_IRQHandler          DMA1_Stream6_IRQHandler
#define I2C1_DMA_RX_IRQHandler          DMA1_Stream5_IRQHandler

/** @} Close I2C_Defines_I2C1 group */

/** @addtogroup I2C_Defines_I2C2 I2C2 Defines
 *  @brief DMA attributes for the i2c2 hardware peripheral
 *  @{
 */

/*
 * Definitions for I2C2
 */
#define I2C2_TX_DMA_CHANNEL             DMA_CHANNEL_7
#define I2C2_TX_DMA_STREAM              DMA1_Stream7
#define I2C2_RX_DMA_CHANNEL             DMA_CHANNEL_7
#define I2C2_RX_DMA_STREAM              DMA1_Stream3

#define I2C2_DMA_TX_IRQn                DMA1_Stream7_IRQn
#define I2C2_DMA_RX_IRQn                DMA1_Stream3_IRQn
#define I2C2_DMA_TX_IRQHandler          DMA1_Stream7_IRQHandler
#define I2C2_DMA_RX_IRQHandler          DMA1_Stream3_IRQHandler

/** @} Close I2C_Defines_I2C2 group */

/** @addtogroup I2C_Defines_I2C3 I2C3 Defines
 *  @brief DMA attributes for the i2c3 hardware peripheral
 *  @{
 */

/*
 * Definitions for I2C3
 */
#define I2C3_TX_DMA_CHANNEL             DMA_CHANNEL_3
#define I2C3_TX_DMA_STREAM              DMA1_Stream4
#define I2C3_RX_DMA_CHANNEL             DMA_CHANNEL_3
#define I2C3_RX_DMA_STREAM              DMA1_Stream2

#define I2C3_DMA_TX_IRQn                DMA1_Stream4_IRQn
#define I2C3_DMA_RX_IRQn                DMA1_Stream2_IRQn
#define I2C3_DMA_TX_IRQHandler          DMA1_Stream4_IRQHandler
#define I2C3_DMA_RX_IRQHandler          DMA1_Stream2_IRQHandler

/** @} Close I2C_Defines_I2C3 group */

//void initI2C(int scl, int sda);
//void deInitI2C(int scl, int sda);

/**
 * @brief GPIO pins with i2c functionality
 */
enum class i2cPin {
	PA8 = 0,// I2C3_SCL
	PB6,	// I2C1_SCL
	PB7,	// I2C1_SDA
	PB8, 	// I2C1_SCL
	PB9,	// I2C1_SDA
	PB10,	// I2C2_SCL
	PB11,	// I2C2_SDA
	PC9,	// I2C3_SDA
	PF0,	// I2C2_SDA
	PF1,	// I2C2_SCL
	PH4,	// I2C2_SCL
	PH5,	// I2C2_SDA
	PH7,	// I2C3_SCL
	PH8,	// I2C3_SDA
};

/// @todo Move to config
#define I2C_SCL_PIN (i2cPin::PB6)
#define I2C_SDA_PIN (i2cPin::PB9)

/** @} Close I2C_Defines group */

/**
 * @brief Class for low-level i2c operations
 *
 * This class is responsible for low-level i2c reads and writes. It uses a
 * singleton design pattern so that it is impossible to create more than one
 * instance. This is because i2c transfers are done via DMA. DMA completion
 * ISRs require the i2cHandle to be global.
 *
 * To use the class, an I2C* must be fetched using I2C::Instance(). Reads and
 * writes (general and memory) can then be freely performed using the member
 * functions. If the i2c hardware peripheral is busy performing an I/O operation,
 * it will block until it is available to perform the next operation.
 */
class I2C {
private:
	// Constructors are private so it can't be called from outside code

	I2C();
	I2C(i2cPin cl, i2cPin da);
	I2C(I2C const&);
	I2C& operator=(I2C const&);

	i2cPin scl;					///< i2c clock GPIO pin
	i2cPin sda;					///< i2c data GPIO pin

	static I2C *i2cInstance;	///< Pointer to the singleton instance

public:
	static I2C* Instance(i2cPin cl, i2cPin da);

	int8_t write(uint16_t devAddr, uint8_t *pData, uint16_t size);
	int8_t read(uint16_t devAddr, uint8_t *pData, uint16_t size);

	int8_t memWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size);

	int8_t memRead(uint16_t devAddr, uint16_t memAddr, uint8_t *pData1, uint16_t size);

	void readyWait(void);
};


#endif

/** @} Close I2C group */
/** @} Close Peripherals Group */
