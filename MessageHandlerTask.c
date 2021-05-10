/* MessageHandlerTask.c */
#include "main.h"
#include "MessageHandlerTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"


extern void MessageHandlerTask(void *argument);

#ifdef DISPLAY
extern xQueueHandle messageIN; //Queue for Incoming Messages
extern xQueueHandle messageOUT; //Queue for Outgoing Messages
extern xQueueHandle messageAudioStream; //Queue for Send AudioStream to Display

extern UART_HandleTypeDef huart6; //UART Handle for Transport
#else
extern UART_HandleTypeDef huart6; //UART Handle for Transport
#endif

void MessageHandlerTask(void *argument)
{
	HAL_UART_Receive_DMA(&huart6, &UART_DMA_IN, RX_IN_SIZE);
	HAL_UART_Transmit_DMA(&huart6, &UART_DMA_OUT, TX_OUT_SIZE);
	uint8_t watchdog = 0;
    p_Bufferd = 0.1;
    p_MAXBufferd = 0.001;


    InitMeassageHandler();

	#ifdef DISPLAY
    UARTsendIntervall = 10;
	#else
    UARTsendIntervall = 2;
	#endif
  for(;;)
  {

	watchdog++;
	#ifdef DISPLAY
	DecodeAudioStream(); //DecodeThe AudioStream and send into Queue.
	xQueueSend(messageAudioStream, &AudioStreamToModel, 0);//Fill the Queue of AudioStream to the Model in TouchGFX
	UART_DMA_OUT[0] = '#';
	UART_DMA_OUT[1] = 's';
	UART_DMA_OUT[2] = 't';
	UART_DMA_OUT[3] = 'a';
	UART_DMA_OUT[10] = watchdog;
	#else
	if(DisplayUpdate == 0){HAL_UART_DMAPause(&huart6);}
	EncodeAudioStream();
	#endif

	readout = ReceiveMessageStack[0].Message_ID;

	//##############SENDING UART################################//
	count++;
	if(count>UARTsendIntervall){
		UARTSEND();
		count = 0;
		#ifdef DISPLAY
		#else
		resetMax = 1;
		#endif
	}
	//#########################################################//

	#ifdef DISPLAY
	maxval1=0;maxval2=0;maxval3=0;maxval4=0;maxval5=0;maxval6=0;
	#else
	if(DisplayUpdate == 0){HAL_UART_DMAResume(&huart6);}
	#endif
	vTaskDelay(10);
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart6){
	UARTRECIVE(); //Recive Data from UART --> UARTDATA
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



