/*
 * main.h
 *
 *  Created on: May 24, 2024
 *      Author: thaithinhtran
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

#define SYS_CLOCK_FREQ_50MHZ	50
#define SYS_CLOCK_FREQ_84MHZ	84
#define SYS_CLOCK_FREQ_120MHZ	120
#define SYS_CLOCK_FREQ_168MHZ	168

#define LED1_PORT GPIOC
#define LED2_PORT GPIOC
#define LED3_PORT GPIOA
#define LED4_PORT GPIOA

#define LED1_PIN_NO GPIO_PIN_1
#define LED2_PIN_NO GPIO_PIN_3
#define LED3_PIN_NO GPIO_PIN_1
#define LED4_PIN_NO GPIO_PIN_7

#define TRUE 1
#define FALSE 0
#endif /* INC_MAIN_H_ */
