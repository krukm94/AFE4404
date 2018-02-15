//NAME: 	"afe.c"
//AUTHOR:	M.KRUK
//DATE 	:	I.2018


#include "afe.h"

//Handles 
I2C_HandleTypeDef hi2c1;

//Global Variables

uint32_t afrReg_Val[51][2] = {
{0x00, 0x000000}, /*CONTROL0*/
{0x01, 0x00000D}, /*LED2STC*/
{0x02, 0x000031}, /*LED2ENDC*/
{0x03, 0x000066}, /*LED1LEDSTC*/
{0x04, 0x000097}, /*LED1LEDENDC*/
{0x05, 0x000040}, /*ALED2STC*/
{0x06, 0x000064}, /*ALED2ENDC*/
{0x07, 0x000073}, /*LED1STC*/
{0x08, 0x000097}, /*LED1ENDC*/
{0x09, 0x000000}, /*LED2LEDSTC*/
{0x0A, 0x000031}, /*LED2LEDENDC*/
{0x0B, 0x0000A6}, /*ALED1STC*/
{0x0C, 0x0000CA}, /*ALED1ENDC*/
{0x0D, 0x000036}, /*LED2CONVST*/
{0x0E, 0x000066}, /*LED2CONVEND*/
{0x0F, 0x00006B}, /*ALED2CONVST*/
{0x10, 0x00009B}, /*ALED2CONVEND*/
{0x11, 0x0000A0}, /*LED1CONVST*/
{0x12, 0x0000D0}, /*LED1CONVEND*/
{0x13, 0x0000D5}, /*ALED1CONVST*/
{0x14, 0x000105}, /*ALED1CONVEND*/
{0x15, 0x000033}, /*ADCRSTSTCT0*/
{0x16, 0x000034}, /*ADCRSTENDCT0*/
{0x17, 0x000068}, /*ADCRSTSTCT1*/
{0x18, 0x000069}, /*ADCRSTENDCT1*/
{0x19, 0x00009D}, /*ADCRSTSTCT2*/
{0x1A, 0x00009E}, /*ADCRSTENDCT2*/
{0x1B, 0x0000D2}, /*ADCRSTSTCT3*/
{0x1C, 0x0000D3}, /*ADCRSTENDCT3*/
{0x1D, 0x001387}, /*PRPCOUNT*/
{0x1E, 0x000103}, /*CONTROL1*/
{0x20, 0x008005}, /*TIAGAIN*/
{0x21, 0x000005}, /*TIA_AMB_GAIN*/
{0x22, 0x000007}, /*LEDCNTRL*/
{0x23, 0x104218}, /*CONTROL2*/
{0x29, 0x000000}, /*CLKDIV1*/
{0x2A, 0x000000}, /*LED2VAL*/
{0x2B, 0x000000}, /*ALED2VAL*/
{0x2C, 0x000000}, /*LED1VAL*/
{0x2D, 0x000000}, /*ALED1VAL*/
{0x2E, 0x000000}, /*LED2-ALED2VAL*/
{0x2F, 0x000000}, /*LED1-ALED1VAL*/
{0x31, 0x000020}, /*CONTROL3*/
{0x32, 0x000169}, /*PDNCYCLESTC*/
{0x33, 0x001323}, /*PDNCYCLEENDC*/
{0x34, 0x000000}, /*PROG_TG_STC*/
{0x35, 0x000000}, /*PROG_TG_ENDC*/
{0x36, 0x000033}, /*LED3LEDSTC*/
{0x37, 0x000064}, /*LED3LEDENDC*/
{0x39, 0x000006}, /*CLKDIV2*/
{0x3A, 0x000000}, /*OFFDAC*/
}; 


// >>>>>>>>>>> Variables for Avreage data
float srednia;
int32_t srednia2;
uint16_t dt = 20;

/**
  * @brief  Initialization adxl
  */
void afeInit(void)
{

	uint16_t dev_addr = (AFE_I2C_ADDRESS << 1);
	
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	hi2c1.Instance = AFE_I2C_INSTANCE;
  hi2c1.Init.Timing = 0x80601419;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	//Hardware Reset
	LL_GPIO_ResetOutputPin(AFE_RESETZ_PORT , AFE_RESETZ_PIN);
	delay_us_mk(30);
	LL_GPIO_SetOutputPin(AFE_RESETZ_PORT , AFE_RESETZ_PIN);
	
	//Wait for deviace ready
	HAL_Delay(20);
	
	//Set REG_READ val to 0
	set_reg_read(dev_addr , 0);
	
	//Set default config
	set_def_conf(dev_addr);
	
	//Check settings of registers
	check_reg_val(dev_addr);
}


/**
  * @brief  Afe read LED Current
	* @param  nrOfLed 	Specify Led number ( 1 or 2)
	* @retval HAL status
  */

int32_t afeReadLedCur(uint8_t nrOfLed , int8_t* sign , uint8_t* inRangeFlag){
	
	int32_t ledCurVal;
	uint32_t u_ledCurVal;
	uint16_t devAddr = (AFE_I2C_ADDRESS << 1 | 1);			// Specify read addres
	uint16_t memAddr;		
	int8_t ret_val = 0x00;
	uint8_t one_reg[3];
	
	*sign = 0x02;
	
	char* printbuf = malloc(100);	// Alloc mem for Print Buf
		
		if(nrOfLed == 1) memAddr = (uint16_t)LED1VAL; 	//Check nrOfLed and set proper registwr address 
		else if(nrOfLed == 2) memAddr = (uint16_t)LED2VAL;
		else {
			sprintf(printbuf , "$ Wrong nr of led! \r\n$ File: %s\r\n$ Line: %d\r\n" , __FILE__ , __LINE__);
			serviceUartWriteS(printbuf);
			
			return 0x00; 
		}
		
		
	//Set REG_READ val to 1
	set_reg_read(devAddr , 1);
		
	//Read AFE Register
	ret_val = HAL_I2C_Mem_Read(&hi2c1, devAddr , memAddr , 1 , one_reg, 3 , 100);
		
	if(ret_val != 0){
		sprintf(printbuf , "$ Read AFE register error! \r\n$ File: %s\r\n$ Line: %d\r\n" , __FILE__ , __LINE__);
		serviceUartWriteS(printbuf);
			
		return 0x00; 
	}
	else{
		//Save Current Value in variable
		u_ledCurVal = (one_reg[0] << 16) | (one_reg[1] << 8) | one_reg[2];

		*sign = ((u_ledCurVal & 0x600000) >> 24);
		*inRangeFlag = ((u_ledCurVal & 0x100000) >> 21);
		
		ledCurVal = u_ledCurVal & 0x1FFFFF;
		
		if(*sign == 0x03){
			ledCurVal *= -1;
				if(*inRangeFlag == 0) ledCurVal = 0;
		}
		else if(*sign == 0x00) {
				if(*inRangeFlag == 1) ledCurVal = 0;
		}
	}
		
	//Set REG_READ val to 0
	set_reg_read(devAddr , 0);
			
	free(printbuf);
		
	return ledCurVal;
}


/**
  * @brief  Avregaging
  */
void sre(float *wynik){

		srednia = srednia * dt;
		srednia = srednia + *wynik;
		srednia = srednia /(dt+1);
}

/**
  * @brief  Avregaging
  */
void sre2(int32_t *wynik){

		srednia2 = srednia2 * dt;
		srednia2 = srednia2 + *wynik;
		srednia2 = srednia2 /(dt+1);
}


/**
  * @brief  Set default config
  */
int8_t set_def_conf(uint16_t device_address){

	int8_t ret_val = 0x00;
	uint8_t loop_cnt = 1;		// start at secound register
	uint8_t one_reg[3];

	char buf[40];
	
	while(loop_cnt != 51){
	
			one_reg[2] =  afrReg_Val[loop_cnt][1]	& 0x0000FF;
			one_reg[1] = (afrReg_Val[loop_cnt][1]  & 0x00FF00) >> 8;
			one_reg[0] = (afrReg_Val[loop_cnt][1]  & 0xFF0000) >> 16;
				
			ret_val = HAL_I2C_Mem_Write(&hi2c1, device_address, afrReg_Val[loop_cnt][0] , 1, one_reg, 3, 100);   
		
			if(ret_val != 0){
				sprintf(buf, "$ Write failed, Reg nr: %d, Ret_val: %d\r\n" , loop_cnt , ret_val);
				serviceUartWriteS(buf);
			}
			
		loop_cnt++;
	}
	
	return ret_val;
}

/**
  * @brief  Set default config
	* @param  nrOfConf Chose configuration which you want to check
  */
int8_t check_reg_val(uint16_t device_address){

	//Variables
	uint16_t dev_addr = (AFE_I2C_ADDRESS << 1 | 1);
	uint8_t loop_cnt = 0;
	int8_t ret_val = 0x00;
	uint8_t one_reg[3];
	uint32_t check_val;
	char buf[40];
	
	//Set REG_READ val to 1
	set_reg_read(dev_addr , 1);
	
	//Fill table with selected values
	//set_reg_val(nrOfConf);
	
	while(loop_cnt != 51){
	
			ret_val = HAL_I2C_Mem_Read(&hi2c1, device_address, afrReg_Val[loop_cnt][0] , 1 , one_reg, 3 , 100);
		
			if(ret_val == 0){
				
					check_val = (one_reg[0] << 16) | (one_reg[1] << 8) | one_reg[2];
					if(check_val != afrReg_Val[loop_cnt][1]){
						
						sprintf(buf , "$ Wrong! Adr nr: %d , ret_val: %d\r\n" , loop_cnt, ret_val);
						serviceUartWriteS(buf);
					}						
			}
			else{
				
				sprintf(buf , "$ Read error Adr nr: %d , ret_val: %d\r\n" , loop_cnt , ret_val);
				serviceUartWriteS(buf);
			}
			loop_cnt++;
	}
	
	//Set REG_READ val to 1
	set_reg_read(dev_addr , 0);
	
	return 0;
}
	

/**
  * @brief  Sned reset command
  * @retval HAL status
  */
int8_t send_reset_cmd(uint16_t device_address){
	
	uint8_t  reg_value[3];
	
	reg_value[0] = 0x00;
	reg_value[1] = 0x00;
	reg_value[2] = 0x08;

	
	return HAL_I2C_Mem_Write(&hi2c1, device_address, CONTROL0, 1, reg_value, 3, 100);
}
/**
  * @brief  Set REG_READ 
  * @retval HAL status
  */
int8_t set_reg_read(uint16_t device_address , uint8_t reg_read_value){

	uint8_t  reg_value[3];
	
	reg_value[0] = 0x00;
	reg_value[1] = 0x00;
	reg_value[2] = reg_read_value;
	
	return HAL_I2C_Mem_Write(&hi2c1, device_address, CONTROL0 , 1, reg_value, 3, 100);
}

/**
  * @brief  Check communication with AFE
  * @retval HAL status
  */
int8_t check_afe_communication(uint16_t device_address){
	
	return HAL_I2C_IsDeviceReady(&hi2c1 , device_address , 100 ,100);
}	

