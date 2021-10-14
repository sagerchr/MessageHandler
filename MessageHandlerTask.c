/* MessageHandlerTask.c */
#include "main.h"
#include "MessageHandlerTask.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#ifdef DISPLAY
#include "shared_params.h"
#else
#endif
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
#else
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

	checksumERROR = 0; //DEBUG helper
	checksumOK = 0; //DEBUG helper

    p_Bufferd = 0.005; //Display Buffers of AudioStream
    p_MAXBufferd = 0.0001; //Display Buffers of AudioStream

    InitMeassageHandler();//init the MessageHandler

	#ifdef DISPLAY
    UARTsendIntervall =0; //Intervall of UART sending
    HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);
	#else
    int sendStackFree = 0; //Needed for QUEUE INs to not overfill STACK
    UARTsendIntervall = 0;
    HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_SET);
    HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);
	#endif
    MessageHandlerInitDone = 1; //Tell System Handler is up and running
    LastMessageIDincoming =-1;
    NewMessageToEnqueue = 0;
    NewMessageEnqueued = 1;
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

	//##########################<<<<Enqueue Message to Process>>>>#########################//
	//#####################################################################################//

	int smallest = 999999;
	int index = 0;

	   for (int i = 0; i < MAXSTACK; i++) {
	      if (ReceiveMessageStack[i].Message_ID < smallest && ReceiveMessageStack[i].status == 99) {
	         smallest = ReceiveMessageStack[i].Message_ID;
	         index = i+1;
	      }
	   }

	   if (index != 0){
	    ReceiveMessageStack[index-1].status = 80;
		MessageFROMHandler = ReceiveMessageStack[index-1];
		xQueueSend(messageFROMHandler, &MessageFROMHandler,0);
		}

	//#####################################################################################//
	//#####################################################################################//

	//######################<<<<PACK MESSAGE TO PHYSICAL LAYER>>>>#########################//
	//#####################################################################################//
	#ifdef DISPLAY
	if(SendConfirmed){
		if(xQueueReceive(messageFORHandler, &MessageINTOHandler, 0) == pdTRUE)
		{
			PackMessage(MessageINTOHandler.MESSAGE, MessageINTOHandler.payload);
		}
	SendConfirmed = 0;
	}
	#else
	if(LastMessageConfirmed() || FirstMessage){
		if(xQueueReceive(messageFORHandler, &MessageINTOHandler, 0) == pdTRUE)
		{
			PackMessage(MessageINTOHandler.MESSAGE, MessageINTOHandler.payload);
		}
	}
	EncodeAudioStream();
	#endif

	//#####################################################################################//
	//#####################################################################################//
	#ifdef DISPLAY

	if(strcmp(MessageINTOHandler.MESSAGE, "*resetMainEngine")== 0){
		HAL_Delay(500);
		NVIC_SystemReset();
	}

	if(strcmp(MessageFROMHandler.MESSAGE, "ResetDisplay")== 0){
		if(MessageFROMHandler.payload == 1){
			SharedParamsWriteByIndex(0, 1);
			NVIC_SystemReset();
		}

	}

	#else
	//if(DisplayUpdate == 0){HAL_UART_DMAResume(&huart6);}

	//######################<<<<SENDING SPI TO PHYSICAL LAYER>>>>##########################//
	//#####################################################################################//
	HAL_SPI_DMAPause(&hspi2);//Pause/Freeze DMA so Data is no more written to it
	resetMax = 1; //Resetting the AudioStream buffer
	HAL_GPIO_WritePin(GPIOB, CS_POTI_Pin,GPIO_PIN_RESET);//CS_PIN for Display
	createHeader();
	createChecksum(); //Create the Checksum
	HAL_SPI_DMAResume(&hspi2);//Restart the DMA so Data could be written again to it
	HAL_SPI_TransmitReceive_DMA(&hspi2,(uint8_t*)UART_DMA_OUT,(uint8_t*)UART_DMA_IN, 200);//Send Data to Display
	#endif
	//#####################################################################################//
	//#####################################################################################//

	vTaskDelay(1);
  }
}




