/**
 * @file DMA_IT.c
 *
 * @brief Direct Memory Access (DMA) interrupt service routines
 *
 * @author Jeremiah Simonsen
 * @author Jasmine Despres
 *
 * @date Oct 24, 2015
 *
 */

#include "uart.h"
//#include "I2C.h"
#include "DMA_IT.h"

UART_HandleTypeDef UartHandle;
I2C_HandleTypeDef i2cHandle;

/**
 * Handles DMA interrupt requests for:
 * 		UART5_RX
 */
void DMA1_Stream0_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
 * Handles DMA interrupt requests for:
 * 		USART3_RX
 */
void DMA1_Stream1_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C3_RX
 * 		UART4_RX
 */
void DMA1_Stream2_IRQHandler(void) {
	if (i2cHandle.hdmarx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmarx);
	}

	if (UartHandle.hdmarx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmarx);
	}
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C2_RX
 * 		USART3_TX
 */
void DMA1_Stream3_IRQHandler(void) {
	if (i2cHandle.hdmarx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmarx);
	}

	if (UartHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmatx);
	}
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C3_TX
 * 		UART4_TX
 */
void DMA1_Stream4_IRQHandler(void) {
	if (i2cHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmatx);
	}

	if (UartHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmatx);
	}
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C1_RX
 * 		USART2_RX
 */
void DMA1_Stream5_IRQHandler(void) {
	if (i2cHandle.hdmarx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmarx);
	}

	if (UartHandle.hdmarx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmarx);
	}
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C1_TX
 * 		USART2_TX
 */
void DMA1_Stream6_IRQHandler(void) {
	if (i2cHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmatx);
	}

	if (UartHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmatx);
	}
}

/**
 * Handles DMA interrupt requests for:
 * 		I2C2_TX
 * 		UART5_TX
 */
void DMA1_Stream7_IRQHandler(void) {
	if (i2cHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(i2cHandle.hdmatx);
	}

	if (UartHandle.hdmatx->State != HAL_DMA_STATE_READY) {
		HAL_DMA_IRQHandler(UartHandle.hdmatx);
	}
}

/**
 * Unused
 */
void DMA2_Stream0_IRQHandler(void) {

}

/**
 * Handles DMA interrupt requests for:
 * 		USART6_RX
 */
void DMA2_Stream1_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
 * Unused
 */
void DMA2_Stream2_IRQHandler(void) {

}

/**
 * Unused
 */
void DMA2_Stream3_IRQHandler(void) {

}

/**
 * Unused
 */
void DMA2_Stream4_IRQHandler(void) {

}

/**
 * Handles DMA interrupt requests for:
 * 		USART1_RX
 */
void DMA2_Stream5_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
 * Handles DMA interrupt requests for:
 * 		USART6_TX
 */
void DMA2_Stream6_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
 * Handles DMA interrupt requests for:
 * 		USART1_TX
 */
void DMA2_Stream7_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}
