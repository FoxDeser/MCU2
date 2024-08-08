/*
 * msp.c
 *
 *  Created on: May 22, 2024
 *      Author: thaithinhtran
 */
#include "main.h"

void HAL_MspInit(void)
{
//Here will do low level processor specific inits.
	//1. set up the priority grouping of the cortex mx processor
//	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x07<<16; //usage faul , memory fault , bus fault system exceptions

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//Here we are going to do the low level inits of UART2 peripheral

	//1. Enable the clock for USART2 peripheral and GPIO pins
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2. Do the pin configurations
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA,&gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA,&gpio_uart);
	//3. Enabe the IRQ and set the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn,15,0);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	//1. Enable the Clock for TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. Setup the priority of TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2ch1_gpio;
	//1. Enable the peripheral clock for the timer2 and GPIOA
	__HAL_RCC_TIM2_CLK_ENABLE();
	//__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Configure a gpio to behave as timer2 channel 1
	tim2ch1_gpio.Pin = GPIO_PIN_0;
	tim2ch1_gpio.Mode= GPIO_MODE_AF_PP;
	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2ch1_gpio);

	//3. nvic settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
