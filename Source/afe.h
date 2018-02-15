//NAME: 	"afe.h"
//AUTHOR:	M.KRUK
//DATE 	:	I.2018

#ifndef __afe_H_
#define __afe_H_

#include "init.h"

// >>>>>>>>> Registers
#define CONTROL0							((uint8_t) 0x00)
#define LED2STC								((uint8_t) 0x01)
#define LED2ENDC							((uint8_t) 0x02)
#define LED1LEDSTC						((uint8_t) 0x03)
#define LED1LEDENDC						((uint8_t) 0x04)
#define ALED2STC							((uint8_t) 0x05)
#define ALED2ENDC							((uint8_t) 0x06)
#define LED1STC								((uint8_t) 0x07)
#define LED1ENDC							((uint8_t) 0x08)
#define LED2LEDSTC						((uint8_t) 0x09)
#define LED2LEDENDC						((uint8_t) 0x0A)
#define ALED1STC							((uint8_t) 0x0B)
#define ALED1ENDC							((uint8_t) 0x0C)
#define LED2CONVST						((uint8_t) 0x0D)
#define LED2CONVEND						((uint8_t) 0x0E)
#define ALED2CONVST						((uint8_t) 0x0F)

#define ALED2CONVEND					((uint8_t) 0x10)
#define LED1CONVST						((uint8_t) 0x11)
#define LED1CONVEND						((uint8_t) 0x12)
#define ALED1CONVST						((uint8_t) 0x13)
#define ALED1CONVEND					((uint8_t) 0x14)
#define ADCRSTSTCT0						((uint8_t) 0x15)
#define ADCRSTENDCT0					((uint8_t) 0x16)
#define ADCRSTSTCT1						((uint8_t) 0x17)
#define ADCRSTENDCT1					((uint8_t) 0x18)
#define ADCRSTSTCT2						((uint8_t) 0x19)
#define ADCRSTENDCT2					((uint8_t) 0x1A)
#define ADCRSTSTCT3						((uint8_t) 0x1B)
#define ADCRSTENDCT3					((uint8_t) 0x1C)
#define PRPCOUNT							((uint8_t) 0x1D)
#define CONTROL1							((uint8_t) 0x1E)

#define TIAGAIN								((uint8_t) 0x20)
#define TIA_AMB_GAIN					((uint8_t) 0x21)
#define LEDCNTRL							((uint8_t) 0x22)
#define CONTROL2							((uint8_t) 0x23)
#define CLKDIV1								((uint8_t) 0x29)
#define LED2VAL								((uint8_t) 0x2A)
#define ALED2VAL							((uint8_t) 0x2B)
#define LED1VAL								((uint8_t) 0x2C)
#define ALED1VAL							((uint8_t) 0x2D)
#define LED2_ALED2VAL					((uint8_t) 0x2E)
#define LED1_ALED1VAL					((uint8_t) 0x2F)

#define CONTROL3							((uint8_t) 0x31)
#define PDNCYCLESTC						((uint8_t) 0x32)
#define PDNCYCLEENDC					((uint8_t) 0x33)
#define PROG_TG_STC						((uint8_t) 0x34)
#define PROG_TG_ENDC					((uint8_t) 0x35)
#define LED3LEDSTC						((uint8_t) 0x36)
#define LED3LEDENDC						((uint8_t) 0x37)
#define CLKDIV2								((uint8_t) 0x39)
#define OFFDAC								((uint8_t) 0x3A)


// >>>>>>>>> functions
void afeInit(void);

int8_t check_afe_communication(uint16_t device_address);

int8_t send_reset_cmd(uint16_t device_address);
int8_t set_reg_read(uint16_t device_address , uint8_t reg_read_value);
int8_t set_def_conf(uint16_t device_address);
int8_t check_reg_val(uint16_t device_addres);

int32_t afeReadLedCur(uint8_t nrOfLed , int8_t* sign , uint8_t* inRangeFlag);

void sre(float *wynik);
void sre2(int32_t *wynik);

#endif //__afe_H_

