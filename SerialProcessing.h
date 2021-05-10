/*
 * UART_IO.h
 *
 *  Created on: 16.11.2019
 *      Author: christiansager
 */
#include "main.h"
#include "types.h"

#ifndef APP_UART_IO_H_
#define APP_UART_IO_H_

void UARTRECIVE();
void UARTSEND();
char UARTDATA[RX_IN_SIZE];
//char UARTDATA_CHECKED[RX_IN_SIZE];
char UARTDATA_legacy[RX_IN_SIZE];
char UART_RECIVE_temp[RX_IN_SIZE];

char UART_RECIVE[RX_IN_SIZE];
char UARTDATA_CHECKED[RX_IN_SIZE];

char UART_DMA_IN[RX_IN_SIZE];
char UART_DMA_OUT [TX_OUT_SIZE];


#endif /* APP_UART_IO_H_ */


