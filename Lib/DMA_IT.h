/**
 * @file
 *
 * @brief Direct Memory Access (DMA) interrupt service routines
 *
 * @author Jeremiah Simonsen
 * @author Jasmine Despres
 *
 * @date Oct 24, 2015
 *
 * The DMA_IT module is responsible for handling all DMA interrupts. This is necessary
 * since multiple peripherals (UART, i2c) use DMA to perform transfers.
 *
 */

/** @addtogroup Peripherals
 *  @{
 */

/** @addtogroup UART
 *  @{
 */

#ifndef _DMA_IT_H_
#define _DMA_IT_H_

extern UART_HandleTypeDef UartHandle;
extern I2C_HandleTypeDef i2cHandle;

void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream7_IRQHandler(void);

void DMA2_Stream0_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void DMA2_Stream2_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);
void DMA2_Stream5_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);

/** @} Close UART group */
/** @} Close Peripherals Group */
#endif
