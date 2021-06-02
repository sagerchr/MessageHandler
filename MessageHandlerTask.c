/* MessageHandlerTask.c */
#include "main.h"
#include "MessageHandlerTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

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

    p_Bufferd = 0.001; //Display Buffers of AudioStream
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

    uint32_t watchdog = 0; //Watchdog
    uint16_t countWatchdogIntervall = 0; //Watchdog
    uint16_t watchdogMessageIntervall = 200; //Watchdog
    MessageHandlerInitDone = 1; //Tell System Handler is up and running

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
		EncodeAudioStream();
		#endif
	}
	popFromMessageQueue();
	//#####################################################################################//
	//#####################################################################################//
	#ifdef DISPLAY

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




