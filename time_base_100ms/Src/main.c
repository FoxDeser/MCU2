/*
 * main.c
 *
 *  Created on: May 22, 2024
 *      Author: thaithinhtran
 */


#include "main.h"
#include "stdio.h"
#include "string.h"


static void Error_handler(void);
static void SystemClock_Config(void);
static void TIMER6_Init (void);
static void GPIO_Init(void);

TIM_HandleTypeDef htim6;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	TIMER6_Init();

	//Let start the timer
	HAL_TIM_Base_Start(&htim6);
	while(1)
	{
		/*Loop until the update event flag is set*/
		while( !(TIM6->SR) &TIM_SR_UIF );
		/*The required time delay has been elapsed
		 * User code can be excuted */
		TIM6->SR &= ~(TIM_SR_UIF);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}

	while(1);
	return 0;
}

void TIMER6_Init (void)
{
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 24;
	htim6.Init.Period = (64000 -1);
	if (HAL_TIM_Base_Init (&htim6) != HAL_OK)
	{
		Error_handler();
	}

}
void GPIO_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef ledgpio;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pin = GPIO_PIN_12;
	ledgpio.Pull= GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD, &ledgpio);
}

void SystemClock_Config(void)
{
	;
}
void Error_handler(void)
{
	;
}
