/**
 * @file I2C_Msp.c
 *
 * @brief MSP functions and interrupts for I2C
 *
 * @author Jeremiah Simonsen
 *
 * @date Sep 29, 2015
 *
 */
#if 0

#include "I2C_Msp.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

I2C_HandleTypeDef i2cHandle;
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;

///////////////////////////////////////////////////////////////////////
//                     Function Pre-Declarations                     //
///////////////////////////////////////////////////////////////////////
void initI2C(int scl, int sda);
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void I2C1_MspInit(I2C_HandleTypeDef *hi2c);
void I2C2_MspInit(I2C_HandleTypeDef *hi2c);
void I2C3_MspInit(I2C_HandleTypeDef *hi2c);

void deInitI2C(int scl, int sda);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);
void I2C1_MspDeInit(void);
void I2C2_MspDeInit(void);
void I2C3_MspDeInit(void);

extern "C" {
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

void I2C1_DMA_RX_IRQHandler(void);
void I2C1_DMA_TX_IRQHandler(void);

void I2C2_DMA_RX_IRQHandler(void);
void I2C2_DMA_TX_IRQHandler(void);

void I2C3_DMA_RX_IRQHandler(void);
void I2C3_DMA_TX_IRQHandler(void);
};

void I2C_MemWrite(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size) {
	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Mem_Write(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size, 1000) != HAL_OK) {
		while (1);
	}
}

void I2C_Mem_Read(uint16_t devAddr, uint16_t memAddr, uint8_t *pData, uint16_t size) {
	while(HAL_I2C_GetState(&i2cHandle) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Mem_Read(&i2cHandle, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT, pData, size, 1000) != HAL_OK) {
		while(1);
	}
}

///////////////////////////////////////////////////////////////////////
//                      Initialization Routines                      //
///////////////////////////////////////////////////////////////////////

void initI2C(int scl, int sda) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_TypeDef *i2cPinToGPIO_TD[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOH};
	uint16_t i2cPinToGpioPin[] = {GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8};
	I2C_TypeDef *i2cPinToI2C_TD[] = {I2C3, I2C1, I2C1, I2C1, I2C1, I2C2, I2C2, I2C3, I2C2, I2C2, I2C2, I2C2, I2C3, I2C3};

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
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);

	i2cHandle.Instance = I2Cx;
	i2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.ClockSpeed      = 100000;
	i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	i2cHandle.Init.OwnAddress1     = 0xFE;
	i2cHandle.Init.OwnAddress2     = 0xFE;

	if (HAL_I2C_Init(&i2cHandle) != HAL_OK) {
		// TODO: ErrorHandler();
		while(1);
	}
}

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

	HAL_DMA_Init(&hdma_tx);

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
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_Init(&hdma_rx);

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C1_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C1_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C1_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_DMA_RX_IRQn);
}

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

	HAL_DMA_Init(&hdma_tx);

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
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_Init(&hdma_rx);

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C2_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C2_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C2_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C2_DMA_RX_IRQn);
}

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

	HAL_DMA_Init(&hdma_tx);

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
	hdma_rx.Init.Mode                = DMA_NORMAL;
	hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_Init(&hdma_rx);

	// Associate the initialized DMA handle to the I2C handle
	__HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

	// Configure NVIC for DMA
	HAL_NVIC_SetPriority(I2C3_DMA_TX_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(I2C3_DMA_TX_IRQn);

	HAL_NVIC_SetPriority(I2C3_DMA_RX_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C3_DMA_RX_IRQn);
}

///////////////////////////////////////////////////////////////////////
//                     DeInitialization Routines                     //
///////////////////////////////////////////////////////////////////////

void deInitI2C(int scl, int sda) {
	GPIO_TypeDef *i2cPinToGPIO_TD[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOF, GPIOF, GPIOH, GPIOH, GPIOH, GPIOH};
	uint16_t i2cPinToGpioPin[] = {GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8};

	GPIO_TypeDef *SCL_PORT = i2cPinToGPIO_TD[(int)scl];
	GPIO_TypeDef *SDA_PORT = i2cPinToGPIO_TD[(int)sda];
	uint16_t SCL_PIN = i2cPinToGpioPin[(int)scl];
	uint16_t SDA_PIN = i2cPinToGpioPin[(int)sda];

	if (HAL_I2C_DeInit(&i2cHandle) != HAL_OK) {
		// Error Handling
	}

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(SCL_PORT, SCL_PIN);
	HAL_GPIO_DeInit(SDA_PORT, SDA_PIN);
}

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

void I2C1_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C1_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C1_DMA_RX_IRQn);
}

void I2C2_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C2_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C2_DMA_RX_IRQn);
}

void I2C3_MspDeInit() {
	// Reset peripheral
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	// GPIO DeInit by I2C::deInitI2C()

	// De-Initialize the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the DMA TX/RX Interrupts
	HAL_NVIC_DisableIRQ(I2C3_DMA_TX_IRQn);
	HAL_NVIC_DisableIRQ(I2C3_DMA_RX_IRQn);
}

////////////////////////////////////////////////////////////////////////
//                         Callback functions                         //
////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	// TODO: Implement
}
#pragma GCC diagnostic pop

// TODO: Refactor these ISRs
///////////////////////////////////////////////////////////////////////
//                               ISRs                                //
///////////////////////////////////////////////////////////////////////
void I2C1_DMA_RX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmarx);
}

void I2C1_DMA_TX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmatx);
}

void I2C2_DMA_RX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmarx);
}

void I2C2_DMA_TX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmatx);
}

void I2C3_DMA_RX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmarx);
}

void I2C3_DMA_TX_IRQHandler(void) {
	HAL_DMA_IRQHandler(i2cHandle.hdmatx);
}

#endif
