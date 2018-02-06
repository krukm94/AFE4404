//NAME: 	"service_uart.c"
//AUTHOR:	M.KRUK
//DATE 	:	XI.2017

#include "service_uart.h"

UART_HandleTypeDef  service_uart;

volatile uint8_t ret_value;

/**
  * @brief  Init Service Uart (USART1)
  */
void serviceUartInit(void)
{
	GPIO_InitTypeDef gpio;
	

	//RCC ON
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//GPIO INIT
	gpio.Pin = SERVICE_UART_TX_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF7_USART2 ;
	HAL_GPIO_Init(SERVICE_UART_TX_PORT , &gpio);
	
//	gpio.Pin = SERVICE_UART_RX_PIN;
//	gpio.Mode = GPIO_MODE_AF_PP;
//	gpio.Pull = GPIO_PULLUP;
//	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	gpio.Alternate = GPIO_AF7_USART2;
//	HAL_GPIO_Init(SERVICE_UART_RX_PORT , &gpio);
	
	service_uart.Instance = SERVICE_UART_INSTANCE;
	service_uart.Init.BaudRate = SERVICE_UART_BAUDRATE;
	service_uart.Init.StopBits = UART_STOPBITS_1;
	service_uart.Init.Parity = UART_PARITY_NONE;
	service_uart.Init.Mode = UART_MODE_TX;
	service_uart.Init.WordLength = UART_WORDLENGTH_8B;
	service_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	service_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	service_uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;

	ret_value = HAL_UART_Init(&service_uart);
	if(ret_value != 0)
	{
			_Error_Handler(__FILE__, __LINE__);
	}
	
	__HAL_UART_ENABLE(&service_uart);

}

/**
  * @brief  Wrtie char on service Uart Tx 
	* @param  char to send
  */
void serviceUartWrite(char data)
{	
	while(!(__HAL_USART_GET_FLAG(&service_uart, UART_FLAG_TXE)));	
	SERVICE_UART_INSTANCE ->TDR = data;
}

/**
  * @brief  Write string on service Uart Tx
  * @param  Pointer to string
  */
void serviceUartWriteS(char *s)
{
	while(*s) serviceUartWrite(*s++);
}

