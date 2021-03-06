/**
 * @file
 *
 * @brief Low level i2c class and code for interfacing with HAL
 *
 * @author Jasmine Despres
 * @author Jeremiah Simonsen
 *
 * @date Sep 27, 2015
 *
 * This file contains functions for interfacing with the STM32Cube_HAL, namely
 * the (de)initialization and callback functions. It also contains an I2C class
 * for additional abstraction. The I2C class uses a singleton pattern to ensure
 * that only one I2C instance is created.
 *
 * All transfers are done via DMA to reduce CPU load. For memory reads, the DMA
 * transfers are double-buffered (aka "ping-pong buffered") so that one buffer
 * can be read while the other is being written.
 *
 */

/** @addtogroup Peripherals
 *  @{
 */

/** @defgroup I2C I2C
 *  @brief Module for communication via i2c - required for interfacing with sensors.
 *  @{
 */

#include "I2C.h"
#include "DMA_IT.h"
#include "errDC9000.h"

#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f407xx.h"

// Global static pointer used to ensure a single instance
I2C* I2C::i2cInstance = NULL;

// Global variables needed by interrupts
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;

// Pin test functions
bool isSclPin(i2cPin p);
bool isSdaPin(i2cPin p);

// Initialization functions
void initI2C(int scl, int sda);
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void I2C1_MspInit(I2C_HandleTypeDef *hi2c);
void I2C2_MspInit(I2C_HandleTypeDef *hi2c);
void I2C3_MspInit(I2C_HandleTypeDef *hi2c);

// De-initialization functions
void deInitI2C(int scl, int sda);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);
void I2C1_MspDeInit(void);
void I2C2_MspDeInit(void);
void I2C3_MspDeInit(void);

// Callbacks and ISRs
#ifdef __cplusplus
extern "C" {
#endif
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

/** @defgroup I2C_Class I2C class
 *  @brief Low-level i2c abstraction
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
 *
 *  @{
 */

/**
 * @brief This function is called to create/get an instance of the class
 * @param cl i2cPin to be used for the clock if the instance has not been created
 * @param da i2cPin to be used for the data if the instance has not been created
 * @return Pointer to a I2C instance
 */
I2C* I2C::Instance(i2cPin cl, i2cPin da) {
	if (i2cInstance == NULL) {
		i2cInstance = new I2C(cl,da);
	}

	return i2cInstance;
}

/**
 * @brief Constructs an I2C object
 */
I2C::I2C() {
	scl = i2cPin::PB8;
	sda = i2cPin::PB9;

	initI2C((int)scl, (int)sda);
}

/**
 * @brief Constructs an I2C object
 * @param cl i2cPin to use for the clock
 * @param da i2cPin to use for the data
 */
I2C::I2C(i2cPin cl, i2cPin da) {
	if (!isSclPin(cl) || !isSdaPin(da)) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Initialize member variables
	scl = cl;
	sda = da;

	// Initialize the i2c peripheral
	initI2C((int)scl, (int)sda);
}

/**
 * @brief Member function to perform generic i2c writes as master
 * @param devAddr i2c slave address of the device to write to (left-justified)
 * @param pData   Pointer to array of bytes to write
 * @param size    Number of bytes to write
 * @return 0 on success, -1 on error
 */
int8_t I2C::write(uint16_t devAddr, uint8_t *pData, uint16_t size) {
	// Must wait until the I2C peripheral is ready, i.e., for any previous transfers to finish
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);

	// Initiate a write
	if (HAL_I2C_Master_Transmit_DMA(&i2cHandle, devAddr, pData, size) != HAL_OK) {
		return -1;
	}

	return 0;
}


/**
 * @brief Member function to perform generic i2c reads as master
 * @param devAddr i2c slave address of the device to read from (left-justified)
 * @param pData   Pointer to array to store read bytes
 * @param size    Number of bytes to read
 * @return 0 on success, -1 on error
 */
int8_t I2C::read(uint16_t devAddr, uint8_t *pData, uint16_t size) {
	// Must wait until the I2C peripheral is ready, i.e., for any previous transfers to finish
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);

	// Initiate a read
	if (HAL_I2C_Master_Receive_DMA(&i2cHandle, devAddr, pData, size) != HAL_OK) {
		return -1;
	}

	return 0;
}

/**
 * @brief Member function to write to a register on an i2c slave device
 * @param devAddr i2c slave address of the device to write to (left-justified)
 * @param memAddr Address of the device register to begin writes
 * @param pData   Pointer to array of bytes to write to slave device memory
 * @param size    Number of bytes to write
 * @return 0 on success, -1 on error
 */
int8_t I2C::memWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size) {
	// Must wait until the I2C peripheral is ready, i.e., for any previous transfers to finish
	while (HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);

	// Initiate a memory write
	if (HAL_I2C_Mem_Write_DMA(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size) != HAL_OK) {
		return -1;
	}

	return 0;
}

/**
 * @brief Member function to read a register from an i2c slave device
 * @param devAddr i2c slave address of the device to read from (left-justified)
 * @param memAddr Address of the device register to read
 * @param pData1  Pointer to data buffer
 * @param size    Number of bytes to be read
 * @return		  0 on success
 * 				  -1 if HAL_Error returned
 */
int8_t I2C::memRead(uint16_t devAddr, uint16_t memAddr, uint8_t *pData1, uint16_t size) {
	// Must wait until the I2C peripheral is ready, i.e., for any previous transfers to finish
	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);

	// Initiate a memory read
	if (HAL_I2C_Mem_Read_DMA(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData1, size) != HAL_OK) {
		return -1;
	}

	return 0;
}

/**
 * @brief Function to block until the peripheral is ready
 */
void I2C::readyWait(void) {
	// Must wait until the I2C peripheral is ready, i.e., for any previous transfers to finish
	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
}

/** @} Close I2C_Class group */

/** @defgroup I2C_Functions Functions
 *  @brief ST HAL required functions, interrupts, helpers
 *
 *  To use i2c, a pointer to the I2C singleton is fetched using I2C::Instance().
 *  This calls the I2C constructor if the singleton has not already been
 *  initialized. The constructors call the initI2C() helper function, which
 *  handles setting up GPIO and the correct i2c hardware peripheral.
 *
 *  I/O is then performed using the I2C::write(), I2C::read(), I2C::memWrite(),
 *  and I2C::memRead() functions.
 *
 *  I2C::readyWait() is exposed to allow code that depends on i2c transfers to
 *  complete to wait for the peripheral to be idle, meaning that data is ready.
 *
 *  @{
 */

/** @addtogroup I2C_Functions_Init Initialization Routines
 *  @brief Functions to initialize the i2c hardware peripherals
 *  @{
 */

/**
 * @brief Function to initialize I2C on a given set of pins
 * @param scl i2cPin to use for the clock
 * @param sda i2cPin to use for the data
 *
 * Initializes GPIO for I2C peripheral and calls HAL_I2C_Init()
 */
void initI2C(int scl, int sda) {
	GPIO_InitTypeDef GPIO_InitStruct;

	// Arrays to map i2cPin variables to HAL-compatible values
	GPIO_TypeDef *i2cPinToGPIO_TD[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOH};
	uint16_t i2cPinToGpioPin[] = {GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8};
	I2C_TypeDef *i2cPinToI2C_TD[] = {I2C3, I2C1, I2C1, I2C1, I2C1, I2C2, I2C2, I2C3, I2C2, I2C2, I2C2, I2C2, I2C3, I2C3};

	// Convert types
	GPIO_TypeDef *SCL_PORT = i2cPinToGPIO_TD[(int)scl];
	GPIO_TypeDef *SDA_PORT = i2cPinToGPIO_TD[(int)sda];
	uint16_t SCL_PIN = i2cPinToGpioPin[(int)scl];
	uint16_t SDA_PIN = i2cPinToGpioPin[(int)sda];
	I2C_TypeDef *I2Cx = i2cPinToI2C_TD[(int)scl];

	// Enable the appropriate GPIO clocks
	switch((uint32_t)SCL_PORT) {
	case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
	case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
	case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
	case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
	case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
	case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
	case (uint32_t)GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
	case (uint32_t)GPIOI: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
	default: break;
	}
	if (SCL_PORT != SDA_PORT) {
		switch((uint32_t)SDA_PORT) {
		case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
		case (uint32_t)GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
		case (uint32_t)GPIOI: __HAL_RCC_GPIOI_CLK_ENABLE(); break;
		default: break;
		}
	}

	// Configure GPIO pins as Alternate Function, Open Drain
	GPIO_InitStruct.Pin = SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;	// same for all 3 peripherals
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);	// SCL Pin

	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);	// SDA Pin

	// Setup the i2c peripheral struct
	i2cHandle.Instance 			   = I2Cx;
	i2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.ClockSpeed      = 400000;
	i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	i2cHandle.Init.OwnAddress1     = 0xFE;
	i2cHandle.Init.OwnAddress2     = 0xFE;

	// Initialize the I2C
	if (HAL_I2C_Init(&i2cHandle) != HAL_OK) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	i2cHandle.Instance->CR2 |= 1<<8;	// Interrupt error enable
	HAL_NVIC_SetPriority(I2C1_ER_IRQn,2,0);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
}

/**
 * @brief Module-specific initialization; called by HAL_I2C_Init()
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
	uint32_t I2Cx = (uint32_t)hi2c->Instance;

	// Initialize the appropriate I2C peripheral
	switch(I2Cx) {
	case (uint32_t)I2C1: I2C1_MspInit(hi2c); break;
	case (uint32_t)I2C2: I2C2_MspInit(hi2c); break;
	case (uint32_t)I2C3: I2C3_MspInit(hi2c); break;
	default: break;
	}
}

//                       Initialization Helpers                      //

/**
 * @brief Helper function for initializing I2C1
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C1_MspInit(I2C_HandleTypeDef *hi2c) {
	// Enable the I2C peripheral clock
	__HAL_RCC_I2C1_CLK_ENABLE();

	// Enable the DMA clock
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Configure DMA for tx
	hdma_tx.Instance				 = I2C1_TX_DMA_STREAM;
	hdma_tx.Init.Channel 			 = I2C1_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

	// Configure DMA for rx
	hdma_rx.Instance                 = I2C1_RX_DMA_STREAM;
	hdma_rx.Init.Channel             = I2C1_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode				 = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C1_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C1_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C1_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_DMA_RX_IRQn);
}

/**
 * @brief Helper function for initializing I2C2
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C2_MspInit(I2C_HandleTypeDef *hi2c) {
	__HAL_RCC_I2C2_CLK_ENABLE();

	// Enable the DMA clock
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Configure DMA for tx
	hdma_tx.Instance				 = I2C2_TX_DMA_STREAM;
	hdma_tx.Init.Channel 			 = I2C2_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

	// Configure DMA for rx
	hdma_rx.Instance                 = I2C2_RX_DMA_STREAM;
	hdma_rx.Init.Channel             = I2C2_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode				 = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C2_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C2_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C2_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C2_DMA_RX_IRQn);
}

/**
 * @brief Helper function for initializing I2C3
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C3_MspInit(I2C_HandleTypeDef *hi2c) {
	__HAL_RCC_I2C3_CLK_ENABLE();

	// Enable the DMA clock
	__HAL_RCC_DMA1_CLK_ENABLE();

	// Configure DMA for tx
	hdma_tx.Instance				 = I2C3_TX_DMA_STREAM;
	hdma_tx.Init.Channel 			 = I2C3_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

	// Configure DMA for rx
	hdma_rx.Instance                 = I2C3_RX_DMA_STREAM;
	hdma_rx.Init.Channel             = I2C3_RX_DMA_CHANNEL;
	hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode				 = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	// Initialize DMA
	if ( HAL_DMA_Init(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_INIT_ERROR);
	}

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C3_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C3_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C3_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C3_DMA_RX_IRQn);
}

/** @} Close I2C_Functions_Init group */

/** @addtogroup I2C_Functions_DeInit De-Initialization Routines
 *  @brief Functions to de-initialize the i2c hardware peripherals
 *  @{
 */

/**
 * @brief Function to de-initialize I2C on a given set of pins
 * @param scl i2cPin used for the clock
 * @param sda i2cPin used for the data
 *
 * De-initializes GPIO for I2C peripheral and calls HAL_I2C_DeInit()
 */
void deInitI2C(int scl, int sda) {
	// Mapping arrays
	GPIO_TypeDef *i2cPinToGPIO_TD[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOH};
	uint16_t i2cPinToGpioPin[] = {GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8};

	// Convert data types
	GPIO_TypeDef *SCL_PORT = i2cPinToGPIO_TD[(int)scl];
	GPIO_TypeDef *SDA_PORT = i2cPinToGPIO_TD[(int)sda];
	uint16_t SCL_PIN = i2cPinToGpioPin[(int)scl];
	uint16_t SDA_PIN = i2cPinToGpioPin[(int)sda];

	// De-initialize I2C peripheral
	if (HAL_I2C_DeInit(&i2cHandle) != HAL_OK) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(SCL_PORT, SCL_PIN);
	HAL_GPIO_DeInit(SDA_PORT, SDA_PIN);
}

/**
 * @brief Module-specific de-initialization; called by HAL_I2C_DeInit()
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c){
	uint32_t I2Cx = (uint32_t)hi2c->Instance;

	// Initialize the appropriate I2C peripheral
	switch(I2Cx) {
	case (uint32_t)I2C1: I2C1_MspDeInit(); break;
	case (uint32_t)I2C2: I2C2_MspDeInit(); break;
	case (uint32_t)I2C3: I2C3_MspDeInit(); break;
	default: break;
	}
}

//                      DeInitialization Helpers                     //
/**
 * @brief Helper function for de-initializing I2C1
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C1_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	if ( HAL_DMA_DeInit(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}
	if ( HAL_DMA_DeInit(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C1_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C1_DMA_RX_IRQn);
}

/**
 * @brief Helper function for de-initializing I2C2
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C2_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	if ( HAL_DMA_DeInit(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}
	if ( HAL_DMA_DeInit(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C2_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C2_DMA_RX_IRQn);
}

/**
 * @brief Helper function for de-initializing I2C3
 * @param hi2c I2C_HandleTypeDef * i2c configuration
 */
void I2C3_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	if ( HAL_DMA_DeInit(&hdma_tx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}
	if ( HAL_DMA_DeInit(&hdma_rx) != HAL_OK ) {
		Error_Handler(errDC9000::I2C_DEINIT_ERROR);
	}

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C3_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C3_DMA_RX_IRQn);
}

/** @} Close I2C_Functions_DeInit group */

/** @addtogroup I2C_Functions_Callbacks Interrupt Callbacks
 *  @brief Functions called on interrupts after flags and errors have been handled
 *  @{
 */

// Silence some warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * @brief Function called when master transmit is complete
 *
 * Currently does nothing. Can be used to signal that a Master write has
 * completed successfully.
 *
 * @param hi2c i2c configuration
 *
 * @todo Implement to add functionality
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when master receive is complete
 *
 * Currently does nothing. Can be used to signal that a Master read has
 * completed successfully.
 *
 * @param hi2c i2c configuration
 *
 * @todo Implement to add functionality
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when slave transmit is complete
 *
 * Currently does nothing. Can be used to signal that a Slave write has
 * completed successfully. Only useful if processor acts as slave.
 *
 * @param hi2c i2c configuration
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when slave receive is complete
 *
 * Currently does nothing. Can be used to signal that a Slave read has
 * completed successfully. Only useful if processor acts as slave.
 *
 * @param hi2c i2c configuration
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when master memory transmit is complete
 *
 * Currently does nothing. Can be used to signal that a Master write to
 * slave memory has completed successfully.
 *
 * @param hi2c i2c configuration
 *
 * @todo Implement to add functionality
 */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when master memory receive is complete
 *
 * Currently does nothing. Can be used to signal that a Master read from
 * slave memory has completed successfully.
 *
 * @param hi2c i2c configuration
 *
 * @todo Implement to add functionality
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {

}

/**
 * @brief Function called when transmission error occurs
 *
 * Currently does nothing. Can be used to signal when an i2c error occurs
 *
 * @param hi2c i2c configuration
 *
 * @todo Implement to add functionality
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	Error_Handler(errDC9000::I2C_IO_ERROR);
}
#pragma GCC diagnostic pop

/** @} Close I2C_Functions_Callbacks group */

/** @addtogroup I2C_Functions_Helpers Helper functions
 *  @brief Convenience functions for utilizing the i2cPin abstraction
 *  @{
 */

/**
 * @brief Helper function to determine if an i2cPin is a clock pin
 * @param p Pin to check
 * @return True if p is an i2c clock pin, false otherwise
 */
bool isSclPin(i2cPin p) {
	// Array corresponding to the i2cPin enum class
	bool map[14] = {true, true, false, true, false, true, false, false, false, true, true, false, true, false};

	return map[(int)p];
}

/**
 * @brief Helper function to determine if an i2cPin is a data pin
 * @param p Pin to check
 * @return True if p is an i2c data pin, false otherwise
 */
bool isSdaPin(i2cPin p) {
	// Array corresponding to the i2cPin enum class
	bool map[14] = {false, false, true, false, true, false, true, true, true, false, false, true, false, true};

	return map[(int)p];
}

/** @} Close I2C_Functions_Helpers group */

/** @} Close I2C_Functions group */

/** @} Close I2C group */
/** @} Close Peripherals Group */
