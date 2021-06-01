/* MessageHandlerTask.c */
#include "main.h"
#include "MessageHandlerTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"



uint16_t calcIncomingChecksum(){
	uint16_t checksum16 = 0;

	for(int i = 0; i < 198; i++) {
	    checksum16 += UART_DMA_IN[i];
	}
	return checksum16;
}

uint16_t calcIOutgoingChecksum(){
	uint16_t checksum16 = 0;

	for(int i = 0; i < 198; i++) {
	    checksum16 += UART_DMA_OUT[i];
	}
	return checksum16;
}

int compareChecksum(){

	uint16_t checksum16 = 0;

	checksum16 = calcIncomingChecksum();

 	 if(((checksum16 & 0x00FF) == UART_DMA_IN[199]) && ((checksum16 >> 8) == UART_DMA_IN[198])){
 	      	return 1;
 	      }
 	      else{
 	      	return 0;
 	      }
}

extern void MessageHandlerTask(void *argument);


#ifdef DISPLAY
extern xQueueHandle messageFORHandler; //Queue for Incoming Messages
extern xQueueHandle messageFROMHandler; //Queue for Incoming Messages
extern xQueueHandle messageAudioStream; //Queue for Send AudioStream to Display
struct Message MessageINTOHandler;
struct Message MessageFROMHandler;
extern UART_HandleTypeDef huart6; //UART Handle for Transport
int MessageHandlerInitDone;
extern SPI_HandleTypeDef hspi2;


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){



	UART_DMA_OUT[0] = '#';
	UART_DMA_OUT[1] = 's';
	UART_DMA_OUT[2] = 't';
	UART_DMA_OUT[3] = 'a';

	int index=0;
	for(int i=0; i<10; i++){
		if(UART_DMA_IN[i] == '#'){
			index = i;
			break;
		}
	}


	for (int i = 0; i< 200;i++){
		UART_DMA_IN_TEMP[i] = UART_DMA_IN[i+index];
	}


	//################################<<<<CHECK THE CHECKSUM>>>>###########################//
	//#####################################################################################//

	checksumCalculated = 0;
	for(int i = 0; i < 195; i++) {
		checksumCalculated += UART_DMA_IN_TEMP[i];
	}

	checksumIncoming = (UART_DMA_IN_TEMP[198] << 24) | (UART_DMA_IN_TEMP[197] << 16) | (UART_DMA_IN_TEMP[196] << 8) | UART_DMA_IN_TEMP[195];

	if (checksumCalculated == checksumIncoming){
		for (int i = 0; i< 200;i++){
			  UARTDATA_CHECKED[i] = UART_DMA_IN_TEMP[i];
		}
		checksumOK++;
	}
	else{
		checksumERROR++;
	}
	//#####################################################################################//
	//#####################################################################################//

	  	getMessageToReciveStack();
	  	if(UARTDATA_CHECKED[6] > maxval1){maxval1 = UARTDATA_CHECKED[6];}
		if(UARTDATA_CHECKED[7] > maxval2){maxval2 = UARTDATA_CHECKED[7];}
		if(UARTDATA_CHECKED[8] > maxval3){maxval3 = UARTDATA_CHECKED[8];}
		if(UARTDATA_CHECKED[9] > maxval4){maxval4 = UARTDATA_CHECKED[9];}
		if(UARTDATA_CHECKED[10] > maxval5){maxval5 = UARTDATA_CHECKED[10];}
		if(UARTDATA_CHECKED[11] > maxval6){maxval6 = UARTDATA_CHECKED[11];}

		HAL_SPI_DMAPause(&hspi2);
		checksumOutgoing = 0;
		for(int i = 0; i < 195; i++) {
		    checksumOutgoing += UART_DMA_OUT[i];
		}

		UART_DMA_OUT[195]= checksumOutgoing & 0xFF;
		UART_DMA_OUT[196]= (checksumOutgoing >> 8)  & 0xFF;
		UART_DMA_OUT[197]= (checksumOutgoing >> 16) & 0xFF;
		UART_DMA_OUT[198]= (checksumOutgoing >> 24) & 0xFF;
		HAL_SPI_DMAResume(&hspi2);
		HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);

}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){

	if (hspi->Instance == SPI2) {

	HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);

	}
}



#else
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){




	//################################<<<<CHECK THE CHECKSUM>>>>###########################//
	//#####################################################################################//
	checksumCalculated = 0;
	for(int i = 0; i < 195; i++) {
		checksumCalculated += UART_DMA_IN[i];
	}


	checksumIncoming = (UART_DMA_IN[198] << 24) | (UART_DMA_IN[197] << 16) | (UART_DMA_IN[196] << 8) | UART_DMA_IN[195];

	if (checksumCalculated == checksumIncoming){
		for (int i = 0; i< 200;i++){
			  UARTDATA_CHECKED[i] = UART_DMA_IN[i];
		}
		checksumOK++;
	}
	else{
		checksumERROR++;
	}
	//#####################################################################################//
	//#####################################################################################//

	  	getMessageToReciveStack();
	  	HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_SET);
}
extern SPI_HandleTypeDef hspi2;

xQueueHandle messageFROM_OSCHandler;
xQueueHandle messageFOR_OSCHandler;
xQueueHandle messageFORHandler; //Queue for Incoming Messages
xQueueHandle messageFROMHandler; //Queue for Incoming Messages
xQueueHandle ControlMessage;
struct Message MessageINTOHandler;
struct Message MessageFROMHandler;
extern UART_HandleTypeDef huart6; //UART Handle for Transport
#endif

//######################<<<<ENQUEUE AUDIOSTREAM TO MODEL>>>>###########################//
//#####################################################################################//
//status
//	5  = new Message in the SendBuffer
//	10 = new Message was put on the UART
//	40 = confirmation by the Slave that Message was successfully read
//
//
//-------------------------------------------------------------------------------------
//	80 = Meassage was Enqueud by the slave
//	99 = Message was not used by the slave
//
//-------------------------------------------------------------------------------------
#ifdef DISPLAY

#else
#endif

void MessageHandlerTask(void *argument)
{

	checksumERROR = 0;
	checksumOK = 0;
	//HAL_UART_Receive_DMA(&huart6, UART_DMA_IN, RX_IN_SIZE);
	//HAL_UART_Transmit_DMA(&huart6, UART_DMA_OUT, TX_OUT_SIZE);
	//Display Buffers of AudioStream
    p_Bufferd = 0.001;
    p_MAXBufferd = 0.0001;
    //init the MessageHandler
    InitMeassageHandler();
    //Intervall of UART sending
	#ifdef DISPLAY
    UARTsendIntervall =0;
    HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);
	#else
    UARTsendIntervall = 0;
    UART_DMA_OUT[0] = '#';
    UART_DMA_OUT[1] = 's';
    UART_DMA_OUT[2] = 't';
    UART_DMA_OUT[3] = 'a';

    HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_SET);
    HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);
	#endif
    //Watchdog
    uint32_t watchdog = 0;
    uint16_t countWatchdogIntervall = 0;
    uint16_t watchdogMessageIntervall = 200;
    uint8_t DMA_ID=0;
    int sendStackFree = 0;
    MessageHandlerInitDone = 1;

  for(;;)
  {
	//increment counter
	count++;

	//######################<<<<ENQUEUE AUDIOSTREAM TO MODEL>>>>###########################//
	//#####################################################################################//
	#ifdef DISPLAY
	DecodeAudioStream(); //DecodeThe AudioStream and send into Queue.
	xQueueSend(messageAudioStream, &AudioStreamToModel, 0);//Fill the Queue of AudioStream




	#else
	//if(DisplayUpdate == 0){HAL_UART_DMAPause(&huart6);}

	#endif
	//#####################################################################################//
	//#####################################################################################//

	//######################<<<<Sending Queued IN Message to slave>>>>#####################//
	//#####################################################################################//
	#ifdef DISPLAY
	if(xQueueReceive(messageFORHandler, &MessageINTOHandler, 0) == pdTRUE)
	{
		sendMessage(MessageINTOHandler.MESSAGE, MessageINTOHandler.payload);
	}
	#else

	sendStackFree = 0;

	for(int i=0; i<MAXSTACK;i++){
		if(SendMessageStack[i].status == 40){
			sendStackFree = 1;
			break;
		}
		sendStackFree = 0;
	}

	if(sendStackFree)
	{
		if(xQueueReceive(messageFORHandler, &MessageINTOHandler, 0) == pdTRUE)
		{
			sendMessage(MessageINTOHandler.MESSAGE, MessageINTOHandler.payload);
		}
	}


	#endif
	//#####################################################################################//
	//#####################################################################################//


	//##########################<<<<Enqueue Message to Process>>>>#########################//
	//#####################################################################################//
	#ifdef DISPLAY

	for(int i=0; i<MAXSTACK; i++)
		{
			if(ReceiveMessageStack[i].status == 99){
			ReceiveMessageStack[i].status = 80;

			MessageFROMHandler = ReceiveMessageStack[i];

			xQueueSend(messageFROMHandler, &MessageFROMHandler,0);
			}
		}

	#else
	for(int i=0; i<MAXSTACK; i++)
		{
			if(ReceiveMessageStack[i].status == 99){
			ReceiveMessageStack[i].status = 80;

			MessageFROMHandler = ReceiveMessageStack[i];

			xQueueSend(messageFROMHandler, &MessageFROMHandler,0);
			}
		}
	#endif
	//#####################################################################################//
	//#####################################################################################//


	//######################<<<<Sending WatchdogMessage to slave>>>>#######################//
	//#####################################################################################//
	countWatchdogIntervall++;

	if(countWatchdogIntervall>watchdogMessageIntervall){
		watchdog++;
		countWatchdogIntervall=0;
		#ifdef DISPLAY
		sendMessage("WatchdogDisplay", watchdog);
		#else
		sendMessage("WatchdogMainEngine", watchdog);
		#endif
	}
	//#####################################################################################//
	//#####################################################################################//

	//######################<<<<SENDING UART TO PHYSICAL LAYER>>>>#########################//
	//#####################################################################################//


	if(count>UARTsendIntervall){
		count = 0;
		#ifdef DISPLAY
		#else
		resetMax = 1; //Resetting the AudioStream buffer
		EncodeAudioStream();
		#endif
	}

	popFromMessageQueue();
	//UARTSEND();	//Send UART to physical OUT
	//#####################################################################################//
	//#####################################################################################//

	#ifdef DISPLAY

	#else
	//if(DisplayUpdate == 0){HAL_UART_DMAResume(&huart6);}


	DMA_ID++;
	HAL_SPI_DMAPause(&hspi2);
	UART_DMA_OUT[0] = '#';
	UART_DMA_OUT[1] = 's';
	UART_DMA_OUT[2] = 't';
	UART_DMA_OUT[3] = 'a';


	checksumOutgoing = 0;
	HAL_SPI_DMAPause(&hspi2);
	for(int i = 0; i < 195; i++) {
	    checksumOutgoing += UART_DMA_OUT[i];
	}
	HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_RESET);
	UART_DMA_OUT[195]= checksumOutgoing & 0xFF;
	UART_DMA_OUT[196]= (checksumOutgoing >> 8)  & 0xFF;
	UART_DMA_OUT[197]= (checksumOutgoing >> 16) & 0xFF;
	UART_DMA_OUT[198]= (checksumOutgoing >> 24) & 0xFF;

	HAL_SPI_DMAResume(&hspi2);
	HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);

	#endif


	vTaskDelay(1);
  }
}




