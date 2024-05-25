/*
 * it.c
 *
 *  Created on: May 24, 2024
 *      Author: thaithinhtran
 */
#include "it.h"
#include "stm32f4xx_hal.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
