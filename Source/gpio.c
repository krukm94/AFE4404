//NAME: 	"gpio.c"
//AUTHOR:	M.KRUK
//DATE:		XII.2016

#include "gpio.h"


/**
  * @brief  Gpios init
  */
void gpioInit(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//RCC ON
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	//LED1
	LL_GPIO_SetPinMode(LED1_PORT, LED1_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(LED1_PORT , LED1_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(LED1_PORT , LED1_PIN , LL_GPIO_PULL_DOWN);
	
		/**TIME MEAS PIN
	*/
	GPIO_InitStruct.Pin = TIM_MEAS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TIM_MEAS_PORT, &GPIO_InitStruct);
	
	/**AFE   RESETZ PIN
	*/
	GPIO_InitStruct.Pin = AFE_RESETZ_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(AFE_RESETZ_PORT, &GPIO_InitStruct);
	
	/**AFE   ADC RDY PIN 
	*/
	GPIO_InitStruct.Pin = AFE_ADC_RDY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(AFE_ADC_RDY_PORT, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, ADC_RDY_PRIORITY , 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
			/**I2C1 GPIO Configuration  I2C FOR AFE   
	*/
	GPIO_InitStruct.Pin = AFE_SCL_PIN|AFE_SDA_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(AFE_I2C_PORT, &GPIO_InitStruct);
	
	//SET LEDS OFF
	LL_GPIO_ResetOutputPin(LED1_PORT , LED1_PIN);
	
	//Rset Time meas Pin
	LL_GPIO_ResetOutputPin(TIM_MEAS_PORT , TIM_MEAS_PIN);
	
	//SET RESETZ HIGH
	LL_GPIO_SetOutputPin(AFE_RESETZ_PORT , AFE_RESETZ_PIN);
}

/**
  * @brief  ADC_RDY INT FUNC
  */
void adcRdyIntFunc(void){

}


/**
  * @brief  Set led ON
  */
void ledOn(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_ResetOutputPin(LED1_PORT , LED1_PIN);
			break;
//		case 3:
//			LL_GPIO_ResetOutputPin(LED3_PORT , LED3_PIN);
//			break;
//		case 4:
//			LL_GPIO_ResetOutputPin(LED4_PORT , LED4_PIN);
//			break;
	}
}

/**
  * @brief  Set led off
  */
void ledOff(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_SetOutputPin(LED1_PORT , LED1_PIN);
			break;
//		case 3:
//			LL_GPIO_SetOutputPin(LED3_PORT , LED3_PIN);
//			break;
//		case 4:
//			LL_GPIO_SetOutputPin(LED4_PORT , LED4_PIN);
//			break;
	}
}

/**
  * @brief  Toggle led
  */
void ledToggle(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_TogglePin(LED1_PORT , LED1_PIN);
			break;
//		case 3:
//			LL_GPIO_TogglePin(LED3_PORT , LED3_PIN);
//			break;
//		case 4:
//			LL_GPIO_TogglePin(LED4_PORT , LED4_PIN);
//			break;
	}
}


