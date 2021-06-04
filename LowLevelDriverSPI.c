/*
 * LowLevelDriverSPI.c
 *
 *  Created on: 2 juni 2021
 *      Author: es015742
 */
#include "main.h"
#include "MessageHandlerTask.h"

void createHeader(){
	UART_DMA_OUT[0] = '#';
	UART_DMA_OUT[1] = 's';
	UART_DMA_OUT[2] = 't';
	UART_DMA_OUT[3] = 'a';
}

void createChecksum(){
	uint32_t createdChecksum = 0;
	for(int i = 0; i < 195; i++) {
		createdChecksum += UART_DMA_OUT[i];
	}

	UART_DMA_OUT[195]= createdChecksum & 0xFF;
	UART_DMA_OUT[196]= (createdChecksum >> 8)  & 0xFF;
	UART_DMA_OUT[197]= (createdChecksum >> 16) & 0xFF;
	UART_DMA_OUT[198]= (createdChecksum >> 24) & 0xFF;
}

int checkChecksum(){

	uint32_t checksumCalculated = 0;
	uint32_t checksumReceived_DMA  = 0;

	for(int i = 0; i < 195; i++) {
		checksumCalculated += UART_DMA_IN[i];
	}

	checksumReceived_DMA = (UART_DMA_IN[198] << 24) | (UART_DMA_IN[197] << 16) | (UART_DMA_IN[196] << 8) | UART_DMA_IN[195];

	if (checksumCalculated == checksumReceived_DMA){
		checksumOK++;
		return 1;
	}
	else{
		checksumERROR++;
		return 0;
	}
}


void resortReceived(){
	int index=0;
	for(int i=0; i<10; i++){
		if(UART_DMA_IN[i] == '#' && UART_DMA_IN[i+1] == 's' && UART_DMA_IN[i+2] == 't' && UART_DMA_IN[i+3] == 'a'){
			index = i;
			break;
		}
	}

	for (int i = 0; i< 200;i++){
		UART_DMA_IN[i] = UART_DMA_IN[i+index];
	}

}



#ifdef DISPLAY
extern SPI_HandleTypeDef hspi2;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){
	resortReceived(); // If needed resort incomming Message
	//################################<<<<CHECK THE CHECKSUM>>>>###########################//
	//#####################################################################################//
	if (checkChecksum()){
		for (int i = 0; i< 200;i++){
			  UARTDATA_CHECKED[i] = UART_DMA_IN[i];
		}
		getMessageToReciveStack();
	}
	//#####################################################################################//
	//#####################################################################################//

	if(UARTDATA_CHECKED[6] > maxval1){maxval1 = UARTDATA_CHECKED[6];}
	if(UARTDATA_CHECKED[7] > maxval2){maxval2 = UARTDATA_CHECKED[7];}
	if(UARTDATA_CHECKED[8] > maxval3){maxval3 = UARTDATA_CHECKED[8];}
	if(UARTDATA_CHECKED[9] > maxval4){maxval4 = UARTDATA_CHECKED[9];}
	if(UARTDATA_CHECKED[10] > maxval5){maxval5 = UARTDATA_CHECKED[10];}
	if(UARTDATA_CHECKED[11] > maxval6){maxval6 = UARTDATA_CHECKED[11];}
	if(UARTDATA_CHECKED[60] > maxval7){maxval7 = UARTDATA_CHECKED[60];}
	if(UARTDATA_CHECKED[61] > maxval8){maxval8 = UARTDATA_CHECKED[61];}

	HAL_SPI_DMAPause(&hspi2);
	createHeader();
	createChecksum();
	HAL_SPI_DMAResume(&hspi2);
	HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){

	if (hspi->Instance == SPI2) {

	HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);

	}
}



#else

extern SPI_HandleTypeDef hspi2;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){
  	HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_SET); //CS_PIN of display HIGH

	//##############<<<<CHECK THE CHECKSUM PUT TO UARTDATA_CHECKED IF OK>>>>###############//
	//#####################################################################################//
	if (checkChecksum()){
		for (int i = 0; i< 200;i++){
			  UARTDATA_CHECKED[i] = UART_DMA_IN[i];
		}
		getMessageToReciveStack();//Put incoming to the ReceiveStack
	}
	//#####################################################################################//
	//#####################################################################################//
}

#endif










