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
#else

xQueueHandle messageFORHandler; //Queue for Incoming Messages
xQueueHandle messageFROMHandler; //Queue for Incoming Messages
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



void MessageHandlerTask(void *argument)
{

	HAL_UART_Receive_DMA(&huart6, UART_DMA_IN, RX_IN_SIZE);
	HAL_UART_Transmit_DMA(&huart6, UART_DMA_OUT, TX_OUT_SIZE);
	//Display Buffers of AudioStream
    p_Bufferd = 0.03;
    p_MAXBufferd = 0.01;
    //init the MessageHandler
    InitMeassageHandler();
    //Intervall of UART sending
	#ifdef DISPLAY
    UARTsendIntervall = 1;
	#else
    UARTsendIntervall = 1;
	#endif
    //Watchdog
    uint32_t watchdog = 0;
    uint16_t countWatchdogIntervall = 0;
    uint16_t watchdogMessageIntervall = 200;

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
	if(xQueueReceive(messageFORHandler, &MessageINTOHandler, 0) == pdTRUE)
	{
		sendMessage(MessageINTOHandler.MESSAGE, MessageINTOHandler.payload);
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
		sendMessage("WatchdogMessageHandler:", watchdog);
		sendMessage("DemoValue:", 0.1 * watchdog);
	}
	//#####################################################################################//
	//#####################################################################################//

	//######################<<<<SENDING UART TO PHYSICAL LAYER>>>>#########################//
	//#####################################################################################//
	if(count>UARTsendIntervall){
		popFromMessageQueue();
		EncodeAudioStream();
		UARTSEND();	//Send UART to physical OUT
		count = 0;
		#ifdef DISPLAY
		#else
		resetMax = 1; //Resetting the AudioStream buffer
		#endif
	}
	//#####################################################################################//
	//#####################################################################################//

	#ifdef DISPLAY
	maxval1=0;maxval2=0;maxval3=0;maxval4=0;maxval5=0;maxval6=0;
	#else
	//if(DisplayUpdate == 0){HAL_UART_DMAResume(&huart6);}
	#endif
	vTaskDelay(5);
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart6){
	UARTRECIVE(); //Recive Data from UART --> UARTDATA
	getMessageToReciveStack();

	#ifdef DISPLAY
	//Collect the MAX value in every crawling period.//
	if(UARTDATA_CHECKED[6] > maxval1){maxval1 = UARTDATA_CHECKED[6];}
	if(UARTDATA_CHECKED[7] > maxval2){maxval2 = UARTDATA_CHECKED[7];}
	if(UARTDATA_CHECKED[8] > maxval3){maxval3 = UARTDATA_CHECKED[8];}
	if(UARTDATA_CHECKED[9] > maxval4){maxval4 = UARTDATA_CHECKED[9];}
	if(UARTDATA_CHECKED[10] > maxval5){maxval5 = UARTDATA_CHECKED[10];}
	if(UARTDATA_CHECKED[11] > maxval6){maxval6 = UARTDATA_CHECKED[11];}
	#endif
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
	HAL_UART_Receive_DMA(&huart6, &UART_DMA_IN, RX_IN_SIZE);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
}



