/*
 * it.c
 *
 *  Created on: May 24, 2024
 *      Author: thaithinhtran
 */
#include "it.h"
#include "main.h"

extern TIM_HandleTypeDef htim6;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
}
