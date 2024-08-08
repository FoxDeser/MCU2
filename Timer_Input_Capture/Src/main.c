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
static void TIMER6_Init (void);
static void LSI_Configuration(void);
static void UART2_Init(void);

uint32_t input_capture[2] = {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;

TIM_HandleTypeDef htim2,htim6;
UART_HandleTypeDef huart2;

int main(void)
{
	uint32_t capture_difference = 0;
	double timer2_cnt_freq = 0;
	double timer2_cnt_res = 0;
	double user_signal_time_period = 0;
	double user_signal_freq =0;

	char *user_data = "The application is running. \r\n";
	char usr_msg[100];

	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50MHZ);
	GPIO_Init();

	UART2_Init();
	TIMER2_Init();
	TIMER6_Init();
//	LSI_Configuration();

	HAL_UART_Transmit(&huart2,(uint8_t*)user_data,strlen(user_data),HAL_MAX_DELAY);

	//Let start the timer in IT mode
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	while(1)
	{
		if (is_capture_done)
		{
			if (input_capture[1] > input_capture[0])
				capture_difference = input_capture[1]-input_capture[0];
			else
				capture_difference = ((0xFFFFFFFF)-input_capture[0])+ input_capture[1];
			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq()*2) / (htim2.Init.Prescaler+1);
			timer2_cnt_res = 1/timer2_cnt_freq;
			user_signal_time_period = capture_difference*timer2_cnt_res;
			user_signal_freq = 1/user_signal_time_period;

			sprintf(usr_msg,"Frequency of the signal applied = %.2f HZ\r\n",user_signal_freq);
			HAL_UART_Transmit(&huart2, (uint8_t*)usr_msg, strlen(usr_msg), HAL_MAX_DELAY);

			is_capture_done = FALSE;
		}
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

void TIMER6_Init (void)
{
	htim6.Instance = TIM6;
	//Generate the frequency ~30Khz
	htim6.Init.Prescaler = 6;
	htim6.Init.Period = (119 -1);
	if (HAL_TIM_Base_Init (&htim6) != HAL_OK)
	{
		Error_handler();
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}

static void TIMER2_Init(void)
{
	TIM_IC_InitTypeDef timer2IC_Config;

	htim2.Instance=TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0xFFFFFFFF;
	htim2.Init.Prescaler = 1;

	if( HAL_TIM_IC_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	timer2IC_Config.ICFilter 	= 0;
	timer2IC_Config.ICPolarity 	= TIM_ICPOLARITY_RISING;
	timer2IC_Config.ICPrescaler	= TIM_ICPSC_DIV1;
	timer2IC_Config.ICSelection	= TIM_ICSELECTION_DIRECTTI;

	if(HAL_TIM_IC_ConfigChannel(&htim2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK)
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


void LSI_Configuration(void)
{
//	RCC_OscInitTypeDef osc_init;
//
//	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
//	osc_init.LSEState = RCC_LSE_ON;
//	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK )
//	{
//		Error_handler();
//	}
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSE,RCC_MCODIV_4);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(!is_capture_done)
	{
		if(count == 1)
		{
			input_capture[0] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
			count++;
		}
		else if (count == 2)
		{
			input_capture[1] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
			count =1;
			is_capture_done = TRUE;
		}
	}
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
void Error_handler(void)
{
	;
}
