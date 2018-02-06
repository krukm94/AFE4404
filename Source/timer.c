//NAME	: "timer.c"
//AUTHOR:	M.KRUK
//DATE 	:	I.2018

#include "timer.h"

TIM_HandleTypeDef			tim3;
TIM_HandleTypeDef			tim2;

volatile uint16_t tim2_updates;
volatile uint32_t delay_cnt;

/**
  * @brief Init all timers 
*/
void init_timers(void)
{
	tim_2_init();	
	tim_3_init();
	
	HAL_Delay(10);
	
}

/**
  * @brief  Timer 2 Init function
*/
void tim_2_init(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	tim2.Instance = TIM2;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Prescaler = 0;
	tim2.Init.Period = 	80000000 - 1;									//Przerwania co 1 s
	
	HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_NVIC_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	if(HAL_TIM_Base_Init(&tim2) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}
	
	__HAL_TIM_ENABLE(&tim2);
	
	__HAL_TIM_ENABLE_IT(&tim2, TIM_IT_UPDATE);
	
	serviceUartWriteS("$ TIM2 INIT OK \r\n");
}

/**
  * @brief  TIM2 Interrupt function
*/
void TIM2_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&tim2, TIM_SR_UIF))
	{
		__HAL_TIM_CLEAR_FLAG(&tim2, TIM_SR_UIF);	
	
		tim2_updates++;
	}
}

/**
  * @brief  Start time measurment function
*/
void start_tim_meas(void)
{																							
	tim2_updates = 0;
	TIM2 -> CNT = 0;
}		

/**
  * @brief  Function get time measurment result
  * @param  Flag display, if 1 then function prints result on service UART
  * @retval Time measurment result
*/
float get_tim_meas(uint8_t display_result)
{
	float time_meas;

	float cnt_quantity;
	uint32_t tim2_cnt = 0;
	
	TIM2 -> CR1 |= 0x00000000;
	
	time_meas = 0.0;
		
	tim2_cnt = TIM2 -> CNT;

	
	cnt_quantity = (float)(LSB * (TIM2 -> CNT));
	
	time_meas =   cnt_quantity;
	
	TIM2 -> CNT = 0;
	tim2_updates = 0;
	
	TIM2 -> CR1 |= 0x00000001;
	
	if(display_result)
	{
		char tim_measurment_buf[40];
		sprintf(tim_measurment_buf , "\r\n\r\n Time measurment result: %f [ns] CNT: %d " , time_meas , tim2_cnt);
		serviceUartWriteS(tim_measurment_buf);
	}
	
	return time_meas;
}

/**
  * @brief  Timer 3 Init function
*/
void tim_3_init(void)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	tim3.Instance = TIM3;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Prescaler = 0;
	tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim3.Init.Period = 	(80 - 1);									//Przerwania co 1 us
	
	HAL_NVIC_SetPriority(TIM3_IRQn, TIM3_NVIC_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	if(HAL_TIM_Base_Init(&tim3) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}
	
	TIM3 -> CR1 |= 0x00000001;
	
	__HAL_TIM_ENABLE_IT(&tim3, TIM_IT_UPDATE);
	
	serviceUartWriteS("$ TIM3 INIT OK \r\n");
}

/**
  * @brief  TIM3 Interrupt function
*/
void TIM3_IRQHandler(void)
{	
	if(__HAL_TIM_GET_FLAG(&tim3, TIM_SR_UIF))
	{
		__HAL_TIM_CLEAR_FLAG(&tim3, TIM_SR_UIF);	
		
		delay_cnt++;
	}
}

/**
  * @brief  Delay Funck
* @param  delay_us: Is your delay in micro seconds [us]
*/

void delay_us_mk(uint32_t delay_us){

	delay_cnt = 0;

	while(delay_cnt < delay_us);

}
