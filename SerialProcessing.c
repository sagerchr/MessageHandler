/*
 * UART_IO.c
 *
 *  Created on: 16.11.2019
 *      Author: christiansager
 */
#include "MessageHandlerTask.h"
UART_HandleTypeDef huart6;
void UARTRECIVE(){
	char checksum;
	uint16_t checksum16;
	int CheckSumOK = 0;

	HAL_UART_DMAPause(&huart6);
	 for(int i = 0; i<RX_IN_SIZE;i++){
		 UARTDATA[i]	= 0x00;
		 UART_RECIVE_temp[i] = UART_DMA_IN[i];
		 UART_RECIVE[i]=0;
	 }

	 HAL_UART_DMAResume(&huart6);

	 int start = 0;

	   	    	     for(int i = 0; i<RX_IN_SIZE;i++){
	   	    	    	 if (UART_RECIVE_temp[i] == '#' && UART_RECIVE_temp[i+1] == 's'&& UART_RECIVE_temp[i+2] == 't' &&UART_RECIVE_temp[i+3] == 'a'){
	   	    	    		   start = i; //found start index @start
	   	    	    		   break;
	   	    	    	 }
	   	    	    	 if (UART_RECIVE_temp[i] == '#' && UART_RECIVE_temp[i+1] == 's'&& UART_RECIVE_temp[i+2] == 't' &&UART_RECIVE_temp[i-RX_IN_SIZE+3] == 'a'){
	   	    	    		   start = i; //found start index @start
	   	    	    		   break;
	   	    	    	 }
	   	    	    	 if (UART_RECIVE_temp[i] == '#' && UART_RECIVE_temp[i+1] == 's'&& UART_RECIVE_temp[i-RX_IN_SIZE+2] == 't' &&UART_RECIVE_temp[i-RX_IN_SIZE+3] == 'a'){
	   	    	    		   start = i; //found start index @start
	   	    	    		   break;
	   	    	    	 }
	   	    	    	 if (UART_RECIVE_temp[i] == '#' && UART_RECIVE_temp[i-RX_IN_SIZE+1] == 's'&& UART_RECIVE_temp[i-RX_IN_SIZE+2] == 't' &&UART_RECIVE_temp[i-RX_IN_SIZE+3] == 'a'){
	   	    	    		   start = i; //found start index @start
	   	    	    		   break;
	   	    	    	 }

	   	    	     }


	   	    	    for (int i = 0; i< 200;i++){
	   	    	    		UARTDATA[i] = UART_RECIVE_temp[i+start];
	   	    	    }

	   	    	    checksum = 0;
	   	    	    checksum16 = 0;
	   	    	    CheckSumOK = 0;

	   	    	    for(int i = 0; i < 198; i++) {
	   	    	      	checksum += UARTDATA[i];
	   	    	      	checksum16 += UARTDATA[i];
	   	    	      }

	   	    	      if(((checksum16 & 0x00FF) == UARTDATA[198]) && ((checksum16 >> 8) == UARTDATA[199])){
	   	    	      	CheckSumOK = 1;
	   	    	      }
	   	    	      else{
	   	    	      	CheckSumOK = 0;

	   	    	      }

	   	    		if(CheckSumOK){

	   	    		  	 for (int i = 0; i< 200;i++){
	   	    		  		UARTDATA_CHECKED[i] = UARTDATA[i];
	   	    		 }
	   	    		}

}


void UARTSEND(){
	HAL_UART_DMAPause(&huart6);



	uint8_t checksum = 0;
	uint16_t checksum16 = 0;
	for(int i = 0; i < 198; i++) {
			checksum += UART_DMA_OUT[i];
			checksum16 += UART_DMA_OUT[i];
		  }
	  UART_DMA_OUT[198]=checksum16 & 0x00FF; //low byte
	  UART_DMA_OUT[199]=checksum16 >> 8; //high byte



	HAL_UART_DMAResume(&huart6);

}



