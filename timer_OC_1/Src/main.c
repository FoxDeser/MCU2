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
static void SystemClock_Config(uint8_t clock_freq);
static void GPIO_Init(void);
static void TIMER2_Init(void);
static void UART2_Init(void);

TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

volatile uint32_t pulse1_value = 10000; //to produce 500Hz
volatile uint32_t pulse2_value = 5000; //to produce 1000HZ
volatile uint32_t pulse3_value = 2500;  //to produce 2000Hz
volatile uint32_t pulse4_value = 1250;  //to produce 4000Hz

volatile uint32_t ccr_content;

int main(void)
{
	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();

	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4);

	while(1)
	{

	}

	return 0;
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

static void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef tim2_OC_init;

	htim2.Instance = TIM2;
	htim2.Init.Period = 0xFFFFFFFF;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Prescaler = 4;
	if(HAL_TIM_OC_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	tim2_OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2_OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2_OC_init.Pulse = pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htim2, &tim2_OC_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
	tim2_OC_init.Pulse = pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htim2, &tim2_OC_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}
	tim2_OC_init.Pulse = pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htim2, &tim2_OC_init, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}
	tim2_OC_init.Pulse = pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htim2, &tim2_OC_init, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}

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

	HAL_UART_Init(&huart2);
}

void SystemClock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint8_t Flatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
	osc_init.HSEState = RCC_HSE_ON;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	switch (clock_freq)
	{
		case SYS_CLOCK_FREQ_50MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100; //PLLN from 50 to 432
			osc_init.PLL.PLLP = 2;

			osc_init.PLL.PLLQ = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168; //PLLN from 50 to 432
			osc_init.PLL.PLLP = 2;

			osc_init.PLL.PLLQ = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_2WS;
			break;
		}
		case SYS_CLOCK_FREQ_120MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240; //PLLN from 50 to 432
			osc_init.PLL.PLLP = 2;

			osc_init.PLL.PLLQ = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_3WS;
			break;
		}
		case SYS_CLOCK_FREQ_168MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 336; //PLLN from 50 to 432
			osc_init.PLL.PLLP = 2;

			osc_init.PLL.PLLQ = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			Flatency = FLASH_ACR_LATENCY_5WS;
			break;
		}
		default :
			return;
	}
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, Flatency)!= HAL_OK)
	{
		Error_handler();
	}

	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {
		ccr_content  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
	 }
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	 {
		ccr_content  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);
	 }
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	 {
		ccr_content  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);
	 }
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	 {
		ccr_content  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);
	 }
}

void Error_handler(void)
{
	;
}
