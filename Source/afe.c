//NAME: 	"afe.c"
//AUTHOR:	M.KRUK
//DATE 	:	I.2018


#include "afe.h"

//Handles 
I2C_HandleTypeDef hi2c1;

//Global Variables

uint32_t afrRegVal[51];  // Table of Reg Values
uint8_t afrReg[51];			// Table of Reg address

// Extern function

/**
  * @brief  Initialization adxl
  */
void afeInit(void)
{
//	char print_buf[100];
//	int8_t 	 ret_val = 0x0A;
	
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
	// FILL REG TABLE
	reg_def();				//FILL REG ADDRESSES
	set_reg_val(1);		//FILL VALUE OF REGISTERS
	
	//Hardware Reset
	LL_GPIO_ResetOutputPin(AFE_RESETZ_PORT , AFE_RESETZ_PIN);
	delay_us_mk(30);
	LL_GPIO_SetOutputPin(AFE_RESETZ_PORT , AFE_RESETZ_PIN);
	
	//Wait for deviace ready
	HAL_Delay(20);
	
//	// Check communication with AFE device
//	ret_val = check_afe_communication(dev_addr);
//	
//	if(ret_val == 0) sprintf(print_buf , "$ AFE communication is OK! \r\n");
//	else sprintf(print_buf , "$ AFE communication FAIL, HAL status: %d \r\n" , ret_val);
//	serviceUartWriteS(print_buf);
	
	//Set default config
	set_def_conf(dev_addr);
	
	//Check settings of registers
	check_reg_val(dev_addr , 1);
}


/**
  * @brief  Afe read 
  */


/**
  * @brief  Set default config
  */
int8_t set_def_conf(uint16_t device_address){

	int8_t ret_val = 0x00;
	uint8_t loop_cnt = 1;		// start at secound register
	uint8_t one_reg[3];
	char buf[40];
	
	while(loop_cnt != 51){
	
			one_reg[2] =  afrRegVal[loop_cnt]	& 0x0000FF;
			one_reg[1] = (afrRegVal[loop_cnt] & 0x00FF00) >> 8;
			one_reg[0] = (afrRegVal[loop_cnt] & 0xFF0000) >> 16;
				
			ret_val = HAL_I2C_Mem_Write(&hi2c1, device_address, afrReg[loop_cnt] , 1, one_reg, 3, 100);   
		
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
int8_t check_reg_val(uint16_t device_address, uint8_t nrOfConf){

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
	set_reg_val(nrOfConf);
	
	while(loop_cnt != 51){
	
			ret_val = HAL_I2C_Mem_Read(&hi2c1, device_address, afrReg[loop_cnt] , 1 , one_reg, 3 , 100);
		
			if(ret_val == 0){
				
					check_val = (one_reg[0] << 16) | (one_reg[1] << 8) | one_reg[2];
					if(check_val != afrRegVal[loop_cnt]){
						
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

	
	return HAL_I2C_Mem_Write(&hi2c1, device_address, afrReg[0] , 1, reg_value ,3 , 100);
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
	
	return HAL_I2C_Mem_Write(&hi2c1, device_address, afrReg[0] , 1, reg_value, 3, 100);
}

/**
  * @brief  Check communication with AFE
  * @retval HAL status
  */
int8_t check_afe_communication(uint16_t device_address){
	
	return HAL_I2C_IsDeviceReady(&hi2c1 , device_address , 100 ,100);
}	


/**
  * @brief  Set def value of Afe registers
	* @param  nrOfConf Chose the nr of config to write
  */
void set_reg_val(uint8_t nrOfConf){
	
char print[60];
	
	switch(nrOfConf){
	
		case 0: //DEFAOULT VALUE FROM AFE4404 GUI
			afrRegVal[0] =  0x000000;//CONTROL0
			afrRegVal[1] =  0x000050;//LED2STC
			afrRegVal[2] =  0x00018F;//LED2ENDC
			afrRegVal[3] =  0x000320;//LED1LEDSTC
			afrRegVal[4] =  0x0004AF;//LED1LEDENDC
			afrRegVal[5] =  0x0001E0;//ALED2STC
			afrRegVal[6] =  0x00031F;//ALED2ENDC
			afrRegVal[7] =  0x000370;//LED1STC
			afrRegVal[8] =  0x0004AF;//LED1ENDC
			afrRegVal[9] =  0x000000;//LED2LEDSTC
			afrRegVal[10] = 0x00018F;//LED2LEDENDC
			afrRegVal[11] = 0x0004FF;//ALED1STC
			afrRegVal[12] = 0x00063E;//ALED1ENDC
			afrRegVal[13] = 0x000198;//LED2CONVST
			afrRegVal[14] = 0x0005BB;//LED2CONVEND
			afrRegVal[15] = 0x0005C4;//ALED2CONVST
			afrRegVal[16] = 0x0009E7;//ALED2CONVEND
			afrRegVal[17] = 0x0009F0;//LED1CONVST
			afrRegVal[18] = 0x000E13;//LED1CONVEND
			afrRegVal[19] = 0x000E1C;//ALED1CONVST
			afrRegVal[20] = 0x00123F;//ALED1CONVEND
			afrRegVal[21] = 0x000191;//ADCRSTSTCT0
			afrRegVal[22] = 0x000197;//ADCRSTENDCT0
			afrRegVal[23] = 0x0005BD;//ADCRSTSTCT1
			afrRegVal[24] = 0x0005C3;//ADCRSTENDCT1
			afrRegVal[25] = 0x0009E9;//ADCRSTSTCT2
			afrRegVal[26] = 0x0009EF;//ADCRSTENDCT2
			afrRegVal[27] = 0x000E15;//ADCRSTSTCT3
			afrRegVal[28] = 0x000E1B;//ADCRSTENDCT3
			afrRegVal[29] = 0x009C3E;//PRPCOUNT
			afrRegVal[30] = 0x000103;//CONTROL1
			afrRegVal[31] = 0x008003;//TIAGAIN
			afrRegVal[32] = 0x000003;//TIA_AMB_GAIN
			afrRegVal[33] = 0x0030CF;//LEDCNTRL
			afrRegVal[34] = 0x104018;//CONTROL2
			afrRegVal[35] = 0x000000;//CLKDIV1
			afrRegVal[36] = 0x000000;//LED2VAL
			afrRegVal[37] = 0x000000;//ALED2VAL
			afrRegVal[38] = 0x000000;//LED1VAL
			afrRegVal[39] = 0x000000;//ALED1VAL
			afrRegVal[40] = 0x000000;//LED2-ALED2VAL
			afrRegVal[41] = 0x000000;//LED1-ALED1VAL
			afrRegVal[42] = 0x000000;//CONTROL3
			afrRegVal[43] = 0x00155F;//PDNCYCLESTC
			afrRegVal[44] = 0x00991F;//PDNCYCLEENDC
			afrRegVal[45] = 0x000000;//PROG_TG_STC
			afrRegVal[46] = 0x000000;//PROG_TG_ENDC
			afrRegVal[47] = 0x000191;//LED3LEDSTC
			afrRegVal[48] = 0x00031F;//LED3LEDENDC
			afrRegVal[49] = 0x000000;//CLKDIV2
			afrRegVal[50] = 0x000000;//OFFDAC
			break;
		
		case 1: //SET FROM AFE 4044 GUI
			afrRegVal[0] =  0x000000;//CONTROL0
			afrRegVal[1] =  0x000064;//LED2STC
			afrRegVal[2] =  0x00018F;//LED2ENDC
			afrRegVal[3] =  0x000322;//LED1LEDSTC
			afrRegVal[4] =  0x0004B1;//LED1LEDENDC
			afrRegVal[5] =  0x0001F5;//ALED2STC
			afrRegVal[6] =  0x000320;//ALED2ENDC
			afrRegVal[7] =  0x000386;//LED1STC
			afrRegVal[8] =  0x0004B1;//LED1ENDC
			afrRegVal[9] =  0x000000;//LED2LEDSTC
			afrRegVal[10] = 0x00018F;//LED2LEDENDC
			afrRegVal[11] = 0x000517;//ALED1STC
			afrRegVal[12] = 0x000642;//ALED1ENDC
			afrRegVal[13] = 0x000199;//LED2CONVST
			afrRegVal[14] = 0x0005BC;//LED2CONVEND
			afrRegVal[15] = 0x0005C6;//ALED2CONVST
			afrRegVal[16] = 0x0009E9;//ALED2CONVEND
			afrRegVal[17] = 0x0009F3;//LED1CONVST
			afrRegVal[18] = 0x000E16;//LED1CONVEND
			afrRegVal[19] = 0x000E20;//ALED1CONVST
			afrRegVal[20] = 0x001243;//ALED1CONVEND
			afrRegVal[21] = 0x000191;//ADCRSTSTCT0
			afrRegVal[22] = 0x000197;//ADCRSTENDCT0
			afrRegVal[23] = 0x0005BE;//ADCRSTSTCT1
			afrRegVal[24] = 0x0005C4;//ADCRSTENDCT1
			afrRegVal[25] = 0x0009EB;//ADCRSTSTCT2
			afrRegVal[26] = 0x0009F1;//ADCRSTENDCT2
			afrRegVal[27] = 0x000E18;//ADCRSTSTCT3
			afrRegVal[28] = 0x000E1E;//ADCRSTENDCT3
			afrRegVal[29] = 0x009C3F;//PRPCOUNT
			afrRegVal[30] = 0x000103;//CONTROL1
			afrRegVal[31] = 0x008003;//TIAGAIN
			afrRegVal[32] = 0x000003;//TIA_AMB_GAIN
			afrRegVal[33] = 0x0030CB;//LEDCNTRL
			afrRegVal[34] = 0x104218;//CONTROL2
			afrRegVal[35] = 0x000000;//CLKDIV1
			afrRegVal[36] = 0x000000;//LED2VAL
			afrRegVal[37] = 0x000000;//ALED2VAL
			afrRegVal[38] = 0x000000;//LED1VAL
			afrRegVal[39] = 0x000000;//ALED1VAL
			afrRegVal[40] = 0x000000;//LED2-ALED2VAL
			afrRegVal[41] = 0x000000;//LED1-ALED1VAL
			afrRegVal[42] = 0x000020;//CONTROL3
			afrRegVal[43] = 0x001563;//PDNCYCLESTC
			afrRegVal[44] = 0x00991F;//PDNCYCLEENDC
			afrRegVal[45] = 0x000000;//PROG_TG_STC
			afrRegVal[46] = 0x000000;//PROG_TG_ENDC
			afrRegVal[47] = 0x000191;//LED3LEDSTC
			afrRegVal[48] = 0x000320;//LED3LEDENDC
			afrRegVal[49] = 0x000000;//CLKDIV2
			afrRegVal[50] = 0x000200;//OFFDAC
			break;
			
		default:
			sprintf(print , "$ Wrong Nr Of Confuguration!, FILE: %s LINE: %d \r\n" , __FILE__ , __LINE__);
			serviceUartWriteS(print);
	}

}	

/**
  * @brief  Set Address Value
  */
void reg_def(void){
	
	afrReg[0] = CONTROL0;
	afrReg[1] = LED2STC;
	afrReg[2] = LED2ENDC;
	afrReg[3] = LED1LEDSTC;
	afrReg[4] = LED1LEDENDC;
	afrReg[5] = ALED2STC;
	afrReg[6] = ALED2ENDC;
	afrReg[7] = LED1STC;
	afrReg[8] = LED1ENDC;
	afrReg[9] = LED2LEDSTC;
	afrReg[10] = LED2LEDENDC;
	afrReg[11] = ALED1STC;
	afrReg[12] = ALED1ENDC;
	afrReg[13] = LED2CONVST;
	afrReg[14] = LED2CONVEND;
	afrReg[15] = ALED2CONVST;
	afrReg[16] = ALED2CONVEND;
	afrReg[17] = LED1CONVST;
	afrReg[18] = LED1CONVEND;
	afrReg[19] = ALED1CONVST;
	afrReg[20] = ALED1CONVEND;
	afrReg[21] = ADCRSTSTCT0;
	afrReg[22] = ADCRSTENDCT0;
	afrReg[23] = ADCRSTSTCT1;
	afrReg[24] = ADCRSTENDCT1;
	afrReg[25] = ADCRSTSTCT2;
	afrReg[26] = ADCRSTENDCT2;
	afrReg[27] = ADCRSTSTCT3;
	afrReg[28] = ADCRSTENDCT3;
	afrReg[29] = PRPCOUNT;
	afrReg[30] = CONTROL1;
	afrReg[31] = TIAGAIN;
	afrReg[32] = TIA_AMB_GAIN;
	afrReg[33] = LEDCNTRL;
	afrReg[34] = CONTROL2;
	afrReg[35] = CLKDIV1;
	afrReg[36] = LED2VAL;
	afrReg[37] = ALED2VAL;
	afrReg[38] = LED1VAL;
	afrReg[39] = ALED1VAL;
	afrReg[40] = LED2_ALED2VAL;
	afrReg[41] = LED1_ALED1VAL;
	afrReg[42] = CONTROL3;
	afrReg[43] = PDNCYCLESTC;
	afrReg[44] = PDNCYCLEENDC;
	afrReg[45] = PROG_TG_STC;
	afrReg[46] = PROG_TG_ENDC;
	afrReg[47] = LED3LEDSTC;
	afrReg[48] = LED3LEDENDC;
	afrReg[49] = CLKDIV2;
	afrReg[50] = OFFDAC;
}
