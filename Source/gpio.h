//NAME: 	"gpio.h"
//AUTHOR:	M.KRUK
//DATE:		XII.2016

#ifndef __gpio_H__
#define __gpio_H__

#include "init.h"

void 		gpioInit(void);

void 		ledOn(uint8_t led_nr);
void 		ledOff(uint8_t led_nr);
void		ledToggle(uint8_t led_nr);

void spi2cs_timeMeasPinInit(void);
void ts3Sel_usbDetectPinInit(void);
void userButtonInit(void);

void timeMeasPinHigh(void);
void timeMeasPinLow(void);
void timeMeasPinToggle(void);

void userButtonFunc(void);


#endif  //__gpio_H__
