//NAME: 	"init.h"
//AUTHOR:	M.KRUK
//DATE 	:	XI.2017


#ifndef __INIT_H_
#define __INIT_H_

#include "stm32l4xx.h"

#include "stm32l4xx_hal.h"

#include "stm32l4xx_ll_gpio.h"

#include "stdio.h"
#include <stdlib.h>
#include <cstdio>


#include "gpio.h"
#include "service_uart.h"
#include "afe.h"
#include "timer.h"

// >>>>>>>>>> NVIC PRIORITETS
#define SYSTIC_NVIC_PRIOTITY       		 0

#define TIM3_NVIC_PRIORITY						 1
#define TIM2_NVIC_PRIORITY						 2

#define ADC_RDY_PRIORITY							 3

// >>>>>>>>>>>>>> DEFINES

// >>>>>>>>> TIME MEAS PIN
#define TIM_MEAS_PIN						GPIO_PIN_8
#define TIM_MEAS_PORT						GPIOC

// LED
#define LED1_PIN	 							GPIO_PIN_13
#define LED1_PORT	 							GPIOB

// >>>>>>>>> SERVICE UART
#define SERVICE_UART_INSTANCE		USART2

#define SERVICE_UART_BAUDRATE 	2000000 // Uart nie dziala na (STM32L452RET6P) kiedy zródlem PLL jest HSI

#define SERVICE_UART_TX_PIN 		GPIO_PIN_2
#define SERVICE_UART_TX_PORT		GPIOA

#define SERVICE_UART_RX_PIN 		GPIO_PIN_3
#define SERVICE_UART_RX_PORT		GPIOA


// >>>>>>>>> AFE

#define AFE_I2C_ADDRESS				((uint16_t) 0x58)

// >>>>>>>>> definitions
#define AFE_I2C_INSTANCE 			I2C1

#define AFE_SDA_PIN						GPIO_PIN_10
#define AFE_SCL_PIN						GPIO_PIN_9
#define AFE_RESETZ_PIN				GPIO_PIN_0
#define AFE_ADC_RDY_PIN				GPIO_PIN_11

#define AFE_I2C_PORT 					GPIOA
#define AFE_RESETZ_PORT				GPIOB
#define AFE_ADC_RDY_PORT			GPIOB


// >>>>>>>>>>>>>> FUNCTIONS

void SystemClock_Config(void);

int8_t init(void);
void HAL_MspInit(void);
void _Error_Handler(char * file, int line);


#endif //__INIT_H_

