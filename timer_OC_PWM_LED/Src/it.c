/*
 * it.c
 *
 *  Created on: May 24, 2024
 *      Author: thaithinhtran
 */
#include "it.h"
#include "main.h"

extern TIM_HandleTypeDef htim2,htim6;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&htim2);
}
