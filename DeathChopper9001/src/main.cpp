#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "DeathChopper9000.h"

static bool enableMotors = false;

void EXTILine0_Config(void);
extern "C" {
	void EXTI0_IRQHandler(void);
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
}

//void descend(float f, float b, float l, float r);

void main()
{
	// At this stage the system clock should have already been configured
	// at high speed.

	DeathChopper9000 *dc9000 = DeathChopper9000::instance();
	dc9000->fly();
}


// TODO: Make this feedback controlled based on rangefinder for the final flight controller
//void descend(float f, float b, float l, float r) {
//	char txBuff[] = "Error, no command received. Descending now";
//	usart_transmit((uint8_t *)txBuff);
//
//	while(1) {
//		f -= 0.01f;
//		b -= 0.01f;
//		l -= 0.01f;
//		r -= 0.01f;
//
//		front.setSpeed(f);
//		rear.setSpeed(b);
//		left.setSpeed(l);
//		right.setSpeed(r);
//
//		if (f <= 0.0f && b <= 0.0f && l <= 0.0f && r <= 0.0f) {
//			while(1);
//		}
//
//		HAL_Delay(50);
//	}
//}


void EXTILine0_Config(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == KEY_BUTTON_PIN) {
		enableMotors = enableMotors == true? false : true;
	}
}

void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}
