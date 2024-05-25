/*
 * main.c
 *
 *  Created on: May 22, 2024
 *      Author: thaithinhtran
 */


#include "main.h"
#include "stm32f4xx_hal_conf.h"

#include "string.h"

void SystemClockConfig(void);
void UART2_Init(void);

static void Error_handler(void);
static uint8_t conver_to_capital(uint8_t data);

char *user_data = "The application is running. \r\n";

UART_HandleTypeDef huart2;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	HAL_UART_Transmit(&huart2,(uint8_t*)user_data,strlen(user_data),HAL_MAX_DELAY);

	uint8_t rcvd_data;
	uint8_t data_buffer[100];
	uint32_t count=0;

	while(1)
	{
		HAL_UART_Receive(&huart2,&rcvd_data,1,HAL_MAX_DELAY);
		if(rcvd_data == '\r')
		{
			break;
		}else
		{
			data_buffer[count++] = conver_to_capital(rcvd_data);
		}
	}
	data_buffer[count++] = '\r';
	HAL_UART_Transmit(&huart2,(uint8_t*)data_buffer,count,HAL_MAX_DELAY);

	while(1);

	return 0;
}

void SystemClockConfig(void)
{
	;
}

uint8_t conver_to_capital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data = data - ('a' - 'A');
	}
	return data;
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
