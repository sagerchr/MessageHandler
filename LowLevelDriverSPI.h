/*
 * LowLevelDriverSPI.h
 *
 *  Created on: 2 juni 2021
 *      Author: es015742
 */

#ifndef MESSAGEHANDLER_LOWLEVELDRIVERSPI_H_
#define MESSAGEHANDLER_LOWLEVELDRIVERSPI_H_


void createChecksum();
int checkChecksum();
void resortReceived();
void createHeader();

char UARTDATA[RX_IN_SIZE];
//char UARTDATA_CHECKED[RX_IN_SIZE];
char UARTDATA_legacy[RX_IN_SIZE];
char UART_RECIVE_temp[RX_IN_SIZE];

char UART_RECIVE[RX_IN_SIZE];
char UARTDATA_CHECKED[RX_IN_SIZE];
char UARTDATA_READ_IN[RX_IN_SIZE];


char UART_DMA_IN[RX_IN_SIZE];
char UART_DMA_IN_TEMP[RX_IN_SIZE];
char UART_DMA_OUT [TX_OUT_SIZE];

#endif /* MESSAGEHANDLER_LOWLEVELDRIVERSPI_H_ */
