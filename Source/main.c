//NAME: 	"main.c"
//AUTHOR:	M.KRUK
//DATE 	:	XI.2017

#include "init.h"

// >>>>>>>>> VARIABLES
volatile uint8_t afe_rdc_rdy_flag;


// >>>>>>>>> MAIN
int main(void){
	
	HAL_Init();
	
	init();
	
	while(1)
	{
		//If AFE adc is ready
		if(afe_rdc_rdy_flag){
			
			//reset flag
			afe_rdc_rdy_flag = 0x00;
			
		}
	}
	
}

