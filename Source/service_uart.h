//NAME: 	"service_uart.h"
//AUTHOR:	M.KRUK
//DATE 	:	XI.2017


#ifndef __service_uart_H_
#define __service_uart_H_

#include "init.h"

void serviceUartInit(void);
void serviceUartWrite(char data);
void serviceUartWriteS(char *s);


#endif
