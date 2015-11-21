#include "uart.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//UART_HandleTypeDef UartHandle;
static __IO ITStatus UartReady = RESET;
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;

///////////////////////////////////////////////////////////////////////
//                     Function Pre-Declarations                     //
///////////////////////////////////////////////////////////////////////
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void USART1MspInit(void);
void USART2MspInit(void);
void USART3MspInit(void);
void UART4MspInit(void);
void UART5MspInit(void);
void USART6MspInit(void);

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
void USART1MspDeInit(void);
void USART2MspDeInit(void);
void USART3MspDeInit(void);
void UART4MspDeInit(void);
void UART5MspDeInit(void);
void USART6MspDeInit(void);

#ifdef __cplusplus
extern "C" {
#endif
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle);

//void USART1_TX_DMA_IRQHandler(void);
//void USART1_RX_DMA_IRQHandler(void);
void USART1_IRQHandler(void);

//void USART2_TX_DMA_IRQHandler(void);
//void USART2_RX_DMA_IRQHandler(void);
void USART2_IRQHandler(void);

//void USART3_TX_DMA_IRQHandler(void);
//void USART3_RX_DMA_IRQHandler(void);
void USART3_IRQHandler(void);

//void UART4_TX_DMA_IRQHandler(void);
//void UART4_RX_DMA_IRQHandler(void);
void UART4_IRQHandler(void);

//void UART5_TX_DMA_IRQHandler(void);
//void UART5_RX_DMA_IRQHandler(void);
void UART5_IRQHandler(void);

//void USART6_TX_DMA_IRQHandler(void);
//void USART6_RX_DMA_IRQHandler(void);
void USART6_IRQHandler(void);
#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////
//                      Initialization Routines                      //
///////////////////////////////////////////////////////////////////////

/**
* @brief Initializes the U(S)ART port specified by @uart_num
* @details Initializes the U(S)ART port specified by @uart_num to settings
* 		 specified by optional arguments. 
* 		 Usage is:
* 		 	init_USART(uart_num, num_args[, baud_rate[, wordlength[, stop_bits[, parity[, hw_flow_ctrl[, mode[, oversampling]]]]]]])
* 
* @param uart_num 		Number of the U(S)ART port to initialize
* @param num_args 		Number of arguments to the function
*/
void init_USART(int uart_num, int num_args, ...)
{
	va_list ap;
	va_start(ap, num_args);

	switch (uart_num) {
		case 1: UartHandle.Instance = USART1; break;
		case 2: UartHandle.Instance = USART2; break;
		case 3: UartHandle.Instance = USART3; break;
		case 4: UartHandle.Instance = UART4; break;
		case 5: UartHandle.Instance = UART5; break;
		case 6: UartHandle.Instance = USART6; break;
		default: UartHandle.Instance = UART4; break;
	}

	// Default configuration settings
	UartHandle.Init.BaudRate     = 9600;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	// Change settings according to number of arguments
	switch (num_args) {
		case 3: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				break;
		case 4: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				break;
		case 5: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				UartHandle.Init.StopBits 		= va_arg(ap, uint32_t);
				break;
		case 6: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				UartHandle.Init.StopBits 		= va_arg(ap, uint32_t);
				UartHandle.Init.Parity 			= va_arg(ap, uint32_t);
				break;
		case 7: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				UartHandle.Init.StopBits 		= va_arg(ap, uint32_t);
				UartHandle.Init.Parity 			= va_arg(ap, uint32_t);
				UartHandle.Init.HwFlowCtl 		= va_arg(ap, uint32_t);
				break;
		case 8: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				UartHandle.Init.StopBits 		= va_arg(ap, uint32_t);
				UartHandle.Init.Parity 			= va_arg(ap, uint32_t);
				UartHandle.Init.HwFlowCtl 		= va_arg(ap, uint32_t);
				UartHandle.Init.Mode 			= va_arg(ap, uint32_t);
				break;
		case 9: UartHandle.Init.BaudRate 		= va_arg(ap, uint32_t);
				UartHandle.Init.WordLength 		= va_arg(ap, uint32_t);
				UartHandle.Init.StopBits 		= va_arg(ap, uint32_t);
				UartHandle.Init.Parity 			= va_arg(ap, uint32_t);
				UartHandle.Init.HwFlowCtl 		= va_arg(ap, uint32_t);
				UartHandle.Init.Mode 			= va_arg(ap, uint32_t);
				UartHandle.Init.OverSampling 	= va_arg(ap, uint32_t);
				break;
		default: break;
	}

	va_end(ap);

	// Note that HAL_UART_Init() calls HAL_UART_MspInit() (MPU-specific initialization)
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		//TODO: Change to global error handler
		BSP_LED_On(LED5);
		while(1);
	}

	UartReady = SET;
}

/*
 * @brief UART MSP Initialization 
 *        This function configures the hardware resources used: 
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration  
 *           - DMA configuration for transmission request by peripheral 
 *           - NVIC configuration for DMA interrupt request enable
 * 
 * @param huart UART handle pointer
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	uint32_t inst = (uint32_t)huart->Instance;

	// Call the appropriate helper function based on the UART instance being initialized
	switch (inst) {
		case (uint32_t)USART1: USART1MspInit();
					 	 	   break;
		case (uint32_t)USART2: USART2MspInit();
					 	 	   break;
		case (uint32_t)USART3: USART3MspInit();
					 	 	   break;
		case (uint32_t)UART4:  UART4MspInit();
					 	 	   break;
		case (uint32_t)UART5:  UART5MspInit();
					 	 	   break;
		case (uint32_t)USART6: USART6MspInit();
					 	 	   break;
		default:	 		   break;
	}
}

//                       Initialization Helpers                      //
/**
 * @brief Helper function. MspInit routine for USART1
 */
void USART1MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	USART1_TX_GPIO_CLK_ENABLE();
	USART1_RX_GPIO_CLK_ENABLE();
	USART1_CLK_ENABLE();
	USART1_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= USART1_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= USART1_TX_AF;

	HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= USART1_RX_PIN;
	GPIO_InitStruct.Alternate 	= USART1_RX_AF;

	HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = USART1_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = USART1_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Receive process
	hdma_rx.Instance                 = USART1_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = USART1_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (USART1_TX)
	HAL_NVIC_SetPriority(USART1_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USART1_RX)
	HAL_NVIC_SetPriority(USART1_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/**
 * @brief Helper function. MspInit routine for USART2
 */
void USART2MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	USART2_TX_GPIO_CLK_ENABLE();
	USART2_RX_GPIO_CLK_ENABLE();
	USART2_CLK_ENABLE();
	USART2_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= USART2_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= USART2_TX_AF;

	HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= USART2_RX_PIN;
	GPIO_InitStruct.Alternate 	= USART2_RX_AF;

	HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = USART2_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = USART2_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Transmission process
	hdma_rx.Instance                 = USART2_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = USART2_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (USART2_TX)
	HAL_NVIC_SetPriority(USART2_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USART2_RX)
	HAL_NVIC_SetPriority(USART2_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * @brief Helper function. MspInit routine for USART3
 */
void USART3MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	USART3_TX_GPIO_CLK_ENABLE();
	USART3_RX_GPIO_CLK_ENABLE();
	USART3_CLK_ENABLE();
	USART3_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= USART3_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= USART3_TX_AF;

	HAL_GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= USART3_RX_PIN;
	GPIO_InitStruct.Alternate 	= USART3_RX_AF;

	HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = USART3_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = USART3_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Transmission process
	hdma_rx.Instance                 = USART3_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = USART3_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (USART3_TX)
	HAL_NVIC_SetPriority(USART3_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USART3_RX)
	HAL_NVIC_SetPriority(USART3_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/**
 * @brief Helper function. MspInit routine for UART4
 */
void UART4MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	UART4_TX_GPIO_CLK_ENABLE();
	UART4_RX_GPIO_CLK_ENABLE();
	UART4_CLK_ENABLE();
	UART4_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= UART4_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= UART4_TX_AF;

	HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= UART4_RX_PIN;
	GPIO_InitStruct.Alternate 	= UART4_RX_AF;

	HAL_GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = UART4_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = UART4_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Transmission process
	hdma_rx.Instance                 = UART4_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = UART4_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (UART4_TX)
	HAL_NVIC_SetPriority(UART4_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(UART4_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (UART4_RX)
	HAL_NVIC_SetPriority(UART4_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART4_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
}

/**
 * @brief Helper function. MspInit routine for UART5
 */
void UART5MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	UART5_TX_GPIO_CLK_ENABLE();
	UART5_RX_GPIO_CLK_ENABLE();
	UART5_CLK_ENABLE();
	UART5_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= UART5_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= UART5_TX_AF;

	HAL_GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= UART5_RX_PIN;
	GPIO_InitStruct.Alternate 	= UART5_RX_AF;

	HAL_GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = UART5_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = UART5_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Transmission process
	hdma_rx.Instance                 = UART5_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = UART5_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (UART5_TX)
	HAL_NVIC_SetPriority(UART5_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(UART5_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (UART5_RX)
	HAL_NVIC_SetPriority(UART5_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
}

/**
 * @brief Helper function. MspInit routine for USART6
 */
void USART6MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable GPIO and peripheral clocks
	USART6_TX_GPIO_CLK_ENABLE();
	USART6_RX_GPIO_CLK_ENABLE();
	USART6_CLK_ENABLE();
	USART6_DMA_CLK_ENABLE();

	// Configure peripheral GPIO
	GPIO_InitStruct.Pin 		= USART6_TX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= USART6_TX_AF;

	HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= USART6_RX_PIN;
	GPIO_InitStruct.Alternate 	= USART6_RX_AF;

	HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);

	// Configure the DMA streams
	hdma_tx.Instance                 = USART6_TX_DMA_STREAM;

	hdma_tx.Init.Channel             = USART6_TX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);

	// Configure the DMA handler for Transmission process
	hdma_rx.Instance                 = USART6_RX_DMA_STREAM;

	hdma_rx.Init.Channel             = USART6_RX_DMA_CHANNEL;
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

	// Associate the initialized DMA handle to the the UART handle
	__HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);

	// Configure the NVIC for DMA
	// NVIC configuration for DMA transfer complete interrupt (USART6_TX)
	HAL_NVIC_SetPriority(USART6_TX_DMA_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART6_TX_DMA_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USART6_RX)
	HAL_NVIC_SetPriority(USART6_RX_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART6_RX_DMA_IRQn);

	/* NVIC configuration for USART TC interrupt */
	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
}

///////////////////////////////////////////////////////////////////////
//                     DeInitialization Routines                     //
///////////////////////////////////////////////////////////////////////

/**
 * @brief UART MSP De-Initialization 
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO, DMA and NVIC configuration to their default state
 * @param huart UART handle pointer
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	uint32_t inst = (uint32_t)huart->Instance;

	switch (inst) {
		case (uint32_t)USART1: 	USART1MspDeInit();
					 	 	 	break;
		case (uint32_t)USART2: 	USART2MspDeInit();
					 	 	 	break;
		case (uint32_t)USART3: 	USART3MspDeInit();
					 	 	 	break;
		case (uint32_t)UART4:  	UART4MspDeInit();
					 	 	 	break;
		case (uint32_t)UART5:  	UART5MspDeInit();
					 	 	 	break;
		case (uint32_t)USART6: 	USART6MspDeInit();
					 	 	 	break;
		default:	 			break;
	}
}

//                      DeInitialization Helpers                     //

/**
 * @brief Helper function. MspDeInit routine for USART1
 */
void USART1MspDeInit()
{
	// Reset peripherals
	USART1_FORCE_RESET();
	USART1_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(USART1_TX_GPIO_PORT, USART1_TX_PIN);
	HAL_GPIO_DeInit(USART1_RX_GPIO_PORT, USART1_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(USART1_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(USART1_RX_DMA_IRQn);
}

/**
 * @brief Helper function. MspDeInit routine for USART2
 */
void USART2MspDeInit()
{
	// Reset peripherals
	USART2_FORCE_RESET();
	USART2_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
	HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(USART2_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(USART2_RX_DMA_IRQn);
}

/**
 * @brief Helper function. MspDeInit routine for USART3
 */
void USART3MspDeInit()
{
	// Reset peripherals
	USART3_FORCE_RESET();
	USART3_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(USART3_TX_GPIO_PORT, USART3_TX_PIN);
	HAL_GPIO_DeInit(USART3_RX_GPIO_PORT, USART3_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(USART3_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(USART3_RX_DMA_IRQn);
}

/**
 * @brief Helper function. MspDeInit routine for UART4
 */
void UART4MspDeInit()
{
	// Reset peripherals
	UART4_FORCE_RESET();
	UART4_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(UART4_TX_GPIO_PORT, UART4_TX_PIN);
	HAL_GPIO_DeInit(UART4_RX_GPIO_PORT, UART4_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(UART4_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(UART4_RX_DMA_IRQn);
}

/**
 * @brief Helper function. MspDeInit routine for UART5
 */
void UART5MspDeInit()
{
	// Reset peripherals
	UART5_FORCE_RESET();
	UART5_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(UART5_TX_GPIO_PORT, UART5_TX_PIN);
	HAL_GPIO_DeInit(UART5_RX_GPIO_PORT, UART5_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(UART5_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(UART5_RX_DMA_IRQn);
}

/**
 * @brief Helper function. MspDeInit routine for USART6
 */
void USART6MspDeInit()
{
	// Reset peripherals
	USART6_FORCE_RESET();
	USART6_RELEASE_RESET();

	// Disable peripherals and GPIO clocks
	HAL_GPIO_DeInit(USART6_TX_GPIO_PORT, USART6_TX_PIN);
	HAL_GPIO_DeInit(USART6_RX_GPIO_PORT, USART6_RX_PIN);

	// Disable the DMA streams
	HAL_DMA_DeInit(&hdma_tx);
	HAL_DMA_DeInit(&hdma_rx);

	// Disable the NVIC for DMA
	HAL_NVIC_DisableIRQ(USART6_TX_DMA_IRQn);
	HAL_NVIC_DisableIRQ(USART6_RX_DMA_IRQn);
}

////////////////////////////////////////////////////////////////////////
//                         Callback functions                         //
////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UartReady = SET;
	BSP_LED_On(LED6);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UartReady = SET;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// TODO: Change to Error_Handler() after refactoring to error.h
	/* Turn LED5 on */
	  BSP_LED_On(LED5);
	  while(1)
	  {
	  }
}

#pragma GCC diagnostic pop

///////////////////////////////////////////////////////////////////////
//                               ISRs                                //
///////////////////////////////////////////////////////////////////////

#if 0
void USART1_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void USART1_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

#if 0
void USART2_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void USART2_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

#if 0
void USART3_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void USART3_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

#if 0
void UART4_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void UART4_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void UART4_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

#if 0
void UART5_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void UART5_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void UART5_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

#if 0
void USART6_TX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

void USART6_RX_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}
#endif

void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

///////////////////////////////////////////////////////////////////////
//                         Transmit function                         //
///////////////////////////////////////////////////////////////////////
void usart_transmit(uint8_t *s)
{
	uint16_t len;

	while (UartReady != SET);
	UartReady = RESET;

	len = strlen((char *)s);

	if (HAL_UART_Transmit_DMA(&UartHandle, s, len) != HAL_OK)
	{
		// TODO: Error Handler();
	}
}

void usart_receive(uint8_t *s, uint32_t size) {
	while (UartReady != SET);
	UartReady = RESET;

	if (HAL_UART_Receive_DMA(&UartHandle, s, size)) {
		// TODO: Error_Handler();
	}
}





