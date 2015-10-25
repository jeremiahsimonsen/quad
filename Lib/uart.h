/**
 * @file 	uart.h
 * @author 	J. Simonsen
 * @date 	13-July-2015
 * @brief	Definitions for UART communication
 */

#ifndef _UART_H_
#define _UART_H_

#include <stdarg.h>
#include "stm32f4xx_hal.h"
#include "stm32_hal_legacy.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4_discovery.h"

#include "DMA_IT.h"

//extern UART_HandleTypeDef UartHandle;

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
void init_USART(int uart_num, int num_args, ...);

void usart_transmit(uint8_t *s);


//////////////////////////////////////////////////////////////////////
//                      Definitions for USART1                      //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////
// USART1 GPIO mapping:			//
// 		CK  - PA8				//
// 		TX  - PA9	-OR-	PB6	//
// 		RX  - PA10	-OR-	PB7	//
// 		CTS - PA11				//
//		RTS - PA12				//
//////////////////////////////////

// Clock 
#define USART1_CLK_ENABLE()				__HAL_RCC_USART1_CLK_ENABLE();
#define USART1_DMA_CLK_ENABLE()			__HAL_RCC_DMA2_CLK_ENABLE();
#define USART1_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();
#define USART1_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();

#define USART1_FORCE_RESET()			__HAL_RCC_USART1_FORCE_RESET();
#define USART1_RELEASE_RESET()			__HAL_RCC_USART1_RELEASE_RESET();

// GPIO
#define USART1_TX_GPIO_PORT				GPIOA
#define USART1_TX_PIN					GPIO_PIN_9
#define USART1_TX_AF					GPIO_AF7_USART1
#define USART1_RX_GPIO_PORT				GPIOA
#define USART1_RX_PIN 					GPIO_PIN_10
#define USART1_RX_AF					GPIO_AF7_USART1

// DMA
#define USART1_TX_DMA_STREAM			DMA2_Stream7
#define USART1_TX_DMA_CHANNEL			DMA_CHANNEL_4
#define USART1_RX_DMA_STREAM			DMA2_Stream5	// OR DMA2_Stream2
#define USART1_RX_DMA_CHANNEL			DMA_CHANNEL_4

// NVIC
#define USART1_TX_DMA_IRQn				DMA2_Stream7_IRQn
#define USART1_RX_DMA_IRQn				DMA2_Stream5_IRQn
#define USART1_TX_DMA_IRQHandler		DMA2_Stream7_IRQHandler
#define USART1_RX_DMA_IRQHandler		DMA2_Stream5_IRQHandler

//////////////////////////////////////////////////////////////////////
//                      Definitions for USART2                      //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////
// USART2 GPIO mapping:			//
// 		CK  - PA4	-OR-	PD7	//
// 		TX  - PA2	-OR-	PD5 //
// 		RX  - PA3	-OR-	PD6 //
// 		CTS - PA0	-OR-	PD3	//
//		RTS - PA1	-OR-	PD4	//
//////////////////////////////////

// Clock 
#define USART2_CLK_ENABLE()				__HAL_RCC_USART2_CLK_ENABLE();
#define USART2_DMA_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();
#define USART2_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();

#define USART2_FORCE_RESET()			__HAL_RCC_USART2_FORCE_RESET();
#define USART2_RELEASE_RESET()			__HAL_RCC_USART2_RELEASE_RESET();

// GPIO
#define USART2_TX_GPIO_PORT				GPIOA
#define USART2_TX_PIN					GPIO_PIN_2
#define USART2_TX_AF					GPIO_AF7_USART2
#define USART2_RX_GPIO_PORT				GPIOA
#define USART2_RX_PIN 					GPIO_PIN_3
#define USART2_RX_AF					GPIO_AF7_USART2

// DMA
#define USART2_TX_DMA_STREAM			DMA1_Stream6
#define USART2_TX_DMA_CHANNEL			DMA_CHANNEL_4
#define USART2_RX_DMA_STREAM			DMA1_Stream5	// OR DMA2_Stream2
#define USART2_RX_DMA_CHANNEL			DMA_CHANNEL_4

// NVIC
#define USART2_TX_DMA_IRQn				DMA1_Stream6_IRQn
#define USART2_RX_DMA_IRQn				DMA1_Stream5_IRQn
#define USART2_TX_DMA_IRQHandler		DMA1_Stream6_IRQHandler
#define USART2_RX_DMA_IRQHandler		DMA1_Stream5_IRQHandler

//////////////////////////////////////////////////////////////////////
//                      Definitions for USART3                      //
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
// USART3 GPIO mapping:						  //
// 		CK  - PB12	-OR-	PD10	-OR- PC12 //
// 		TX  - PB10	-OR-	PD8		-OR- PC10 //
// 		RX  - PB11	-OR-	PD9		-OR- PC11 //
// 		CTS - PB13	-OR-	PD11			  //
//		RTS - PB14	-OR-	PD12			  //
////////////////////////////////////////////////

// Clock 
#define USART3_CLK_ENABLE()				__HAL_RCC_USART3_CLK_ENABLE();
#define USART3_DMA_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE();
#define USART3_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE();
#define USART3_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE();

#define USART3_FORCE_RESET()			__HAL_RCC_USART3_FORCE_RESET();
#define USART3_RELEASE_RESET()			__HAL_RCC_USART3_RELEASE_RESET();

// GPIO
#define USART3_TX_GPIO_PORT				GPIOB
#define USART3_TX_PIN					GPIO_PIN_10
#define USART3_TX_AF					GPIO_AF7_USART3
#define USART3_RX_GPIO_PORT				GPIOB
#define USART3_RX_PIN 					GPIO_PIN_11
#define USART3_RX_AF					GPIO_AF7_USART3

// DMA
#define USART3_TX_DMA_STREAM			DMA1_Stream3	// OR DMA1_Stream4
#define USART3_TX_DMA_CHANNEL			DMA_CHANNEL_4	// OR DMA_CHANNEL7
#define USART3_RX_DMA_STREAM			DMA1_Stream1
#define USART3_RX_DMA_CHANNEL			DMA_CHANNEL_4

// NVIC
#define USART3_TX_DMA_IRQn				DMA1_Stream3_IRQn
#define USART3_RX_DMA_IRQn				DMA1_Stream1_IRQn
#define USART3_TX_DMA_IRQHandler		DMA1_Stream3_IRQHandler
#define USART3_RX_DMA_IRQHandler		DMA1_Stream1_IRQHandler

//////////////////////////////////////////////////////////////////////
//                      Definitions for UART4                       //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////
// UART4 GPIO mapping:				//
// 		CK  - ******************	//
// 		TX  - PA0	-OR-	PC10	//
// 		RX  - PA1	-OR-	PC11	//
// 		CTS - ******************	//
//		RTS - ******************	//
//////////////////////////////////////

// Clock 
#define UART4_CLK_ENABLE()				__HAL_RCC_UART4_CLK_ENABLE();
#define UART4_DMA_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE();
#define UART4_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();
#define UART4_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();

#define UART4_FORCE_RESET()				__HAL_RCC_UART4_FORCE_RESET();
#define UART4_RELEASE_RESET()			__HAL_RCC_UART4_RELEASE_RESET();

// GPIO
#define UART4_TX_GPIO_PORT				GPIOC
#define UART4_TX_PIN					GPIO_PIN_10
#define UART4_TX_AF						GPIO_AF8_UART4
#define UART4_RX_GPIO_PORT				GPIOC
#define UART4_RX_PIN 					GPIO_PIN_11
#define UART4_RX_AF						GPIO_AF8_UART4

// DMA
#define UART4_TX_DMA_STREAM				DMA1_Stream4
#define UART4_TX_DMA_CHANNEL			DMA_CHANNEL_4
#define UART4_RX_DMA_STREAM				DMA1_Stream2
#define UART4_RX_DMA_CHANNEL			DMA_CHANNEL_4

// NVIC
#define UART4_TX_DMA_IRQn				DMA1_Stream4_IRQn
#define UART4_RX_DMA_IRQn				DMA1_Stream2_IRQn
#define UART4_TX_DMA_IRQHandler			DMA1_Stream4_IRQHandler
#define UART4_RX_DMA_IRQHandler			DMA1_Stream2_IRQHandler

//////////////////////////////////////////////////////////////////////
//                      Definitions for UART5                       //
//////////////////////////////////////////////////////////////////////

//////////////////////////
// UART5 GPIO mapping:	//
// 		CK  - ****		//
// 		TX  - PC12		//
// 		RX  - PD2		//
// 		CTS - ****		//
//		RTS - ****		//
//////////////////////////

// Clock 
#define UART5_CLK_ENABLE()				__HAL_RCC_UART5_CLK_ENABLE();
#define UART5_DMA_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE();
#define UART5_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOD_CLK_ENABLE();
#define UART5_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();

#define UART5_FORCE_RESET()				__HAL_RCC_UART5_FORCE_RESET();
#define UART5_RELEASE_RESET()			__HAL_RCC_UART5_RELEASE_RESET();

// GPIO
#define UART5_TX_GPIO_PORT				GPIOC
#define UART5_TX_PIN					GPIO_PIN_12
#define UART5_TX_AF						GPIO_AF8_UART5
#define UART5_RX_GPIO_PORT				GPIOD
#define UART5_RX_PIN 					GPIO_PIN_2
#define UART5_RX_AF						GPIO_AF8_UART5

// DMA
#define UART5_TX_DMA_STREAM				DMA1_Stream7
#define UART5_TX_DMA_CHANNEL			DMA_CHANNEL_4
#define UART5_RX_DMA_STREAM				DMA1_Stream0
#define UART5_RX_DMA_CHANNEL			DMA_CHANNEL_4

// NVIC
#define UART5_TX_DMA_IRQn				DMA1_Stream7_IRQn
#define UART5_RX_DMA_IRQn				DMA1_Stream0_IRQn
#define UART5_TX_DMA_IRQHandler			DMA1_Stream7_IRQHandler
#define UART5_RX_DMA_IRQHandler			DMA1_Stream0_IRQHandler

//////////////////////////////////////////////////////////////////////
//                      Definitions for USART6                      //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////
// USART6 GPIO mapping:				//
// 		CK  - PC8	-OR-	PG7 	//
// 		TX  - PC6 	-OR-	PG14	//
// 		RX  - PC7 	-OR-	PG9		//
// 		CTS - PG13 	-OR-	PG15	//
//		RTS - PG8 	-OR-	PG12	//
//////////////////////////////////////

// Clock 
#define USART6_CLK_ENABLE()				__HAL_RCC_USART6_CLK_ENABLE();
#define USART6_DMA_CLK_ENABLE()			__HAL_RCC_DMA2_CLK_ENABLE();
#define USART6_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();
#define USART6_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();

#define USART6_FORCE_RESET()			__HAL_RCC_USART6_FORCE_RESET();
#define USART6_RELEASE_RESET()			__HAL_RCC_USART6_RELEASE_RESET();

// GPIO
#define USART6_TX_GPIO_PORT				GPIOC
#define USART6_TX_PIN					GPIO_PIN_6
#define USART6_TX_AF					GPIO_AF8_USART6
#define USART6_RX_GPIO_PORT				GPIOC
#define USART6_RX_PIN 					GPIO_PIN_7
#define USART6_RX_AF					GPIO_AF8_USART6

// DMA
#define USART6_TX_DMA_STREAM			DMA2_Stream6	// OR DMA2_Stream7
#define USART6_TX_DMA_CHANNEL			DMA_CHANNEL_5
#define USART6_RX_DMA_STREAM			DMA2_Stream1	// OR DMA2_Stream2
#define USART6_RX_DMA_CHANNEL			DMA_CHANNEL_5

// NVIC
#define USART6_TX_DMA_IRQn				DMA2_Stream6_IRQn
#define USART6_RX_DMA_IRQn				DMA2_Stream1_IRQn
#define USART6_TX_DMA_IRQHandler		DMA2_Stream6_IRQHandler
#define USART6_RX_DMA_IRQHandler		DMA2_Stream1_IRQHandler


#endif
