//NAME: 	"timer.h"
//AUTHOR:	M.KRUK
//DATE 	:	I.2018

#ifndef __timer_H_
#define __timer_H_

#include "init.h"

#define APB1_FREQ			80000000
#define LSB						((float) 12.5)

#define DISPLAY_MEAS_RES  ((uint8_t) 1)
#define NOT_DISPLAT_RES		((uint8_t) 1)

void init_timers(void);

void tim_2_init(void);
void tim_3_init(void);

void start_tim_meas(void);
float get_tim_meas(uint8_t display_result);

void delay_us_mk(uint32_t delay_us);

#endif  //__timer_H_

