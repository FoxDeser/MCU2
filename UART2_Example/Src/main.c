/*
 * main.c
 *
 *  Created on: May 22, 2024
 *      Author: thaithinhtran
 */


#include "main.h"
#include "stm32f4xx_hal_conf.h"

void SystemClockConfig(void);
void UART2_Init(void);

static void Error_handler(void);

UART_HandleTypeDef huart2;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	return 0;
}

void SystemClockConfig(void)
{
	;
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_8;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//there is a problem
		Error_handler();
	}
}

static void Error_handler(void)
{
	while (1);
}
