/*
 * it.c
 *
 *  Created on: May 24, 2024
 *      Author: thaithinhtran
 */
#include "it.h"
#include "main.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
