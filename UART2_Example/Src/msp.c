/*
 * msp.c
 *
 *  Created on: May 22, 2024
 *      Author: thaithinhtran
 */
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
//Here will do low level processor specific inits.
	//1. set up the priority grouping of the cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x07<<16; //usage faul , memory fault , bus fault system exceptions

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}
