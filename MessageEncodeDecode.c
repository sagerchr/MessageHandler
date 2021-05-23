/*
 * MessageEncodeDecode.c
 *
 *  Created on: 8 maj 2021
 *      Author: es015742
 */
#ifdef DISPLAY
#else
#include "main.h"
#endif
#include "MessageHandlerTask.h"
#include <string.h>
int MessageID_RECEIVE;
uint32_t ID_COUNT;
uint16_t MessageID, ReceivedMessageID;
static int SendMessageStackPointer,PopStackPointer, ReceiveMessageStackPointer, UnsentMessages;
int timeout;

void InitMeassageHandler(){
	SendMessageStackPointer = -1;
	PopStackPointer = -1;
	ReceiveMessageStackPointer = -1;
	ID_COUNT = 0;
	timeout = 100;
	for(int i=0; i<MAXSTACK; i++)
				{
					ReceiveMessageStack[i].Message_ID = 29;
				}

		#ifdef DISPLAY
		#else

		#endif
}
//###################################################//

//###################################################//
//######## Push a new Message to the Stack ##########//
//###################################################//
void sendMessage(char *String, float payload)
{
	if (SendMessageStackPointer == MAXSTACK-1){
		SendMessageStackPointer = -1;			//Reset WriteStackPointer if end of Queue is reached
		}
		SendMessageStackPointer = SendMessageStackPointer + 1; //Increment Write StackPointer
		#ifdef DISPLAY
		#else
		SendMessageStackPointer_helper = SendMessageStackPointer;
		#endif
        strcpy(SendMessageStack[SendMessageStackPointer].MESSAGE, String); //Fill Message to the Stack
        SendMessageStack[SendMessageStackPointer].Message_ID = ID_COUNT++; //Set Message_ID
        SendMessageStack[SendMessageStackPointer].status = 5; //Set Status to 5 "not yet sent to slave"
        SendMessageStack[SendMessageStackPointer].payload = payload; //Set Status to 5 "not yet sent to slave"

}


//#####################################################################//
//####### Pop oldest Message from the Stack and write to UART #########//
//#####################################################################//
void popFromMessageQueue()
{



	char *String;
	char data[sizeof(float)];
	char a = data[0];char b = data[1];char c = data[2];char d = data[3];

	static float payload;

	//################Check if the last Message was read successfully bz the slave####################//


	ReceivedMessageID = (UARTDATA_CHECKED[185]<<8) | (UARTDATA_CHECKED[184] & 0xFF);
	if((ReceivedMessageID == MessageID && SendProcess == 1)||timeout <= 0){
		  SendMessageStack[PopStackPointer].status = 40; //Check Message as successfully read by slave
		  UART_DMA_OUT[183] = 0; //Tell slave there is no Message to read
		  SendProcess = 0; //Send Process is done
		  timeout = 20;
	}

	if(SendProcess){timeout--;} //countdown TimeOut for Received by slave

	//################################################################################################//


		if(SendProcess == 0){
			if (PopStackPointer == (MAXSTACK-1))
			{
				PopStackPointer = -1;
			}
			PopStackPointer = PopStackPointer+1;
		}



		#ifdef DISPLAY
		#else
			PopStackPointer_helper = PopStackPointer;
		#endif
		if(SendMessageStack[PopStackPointer].status == 5 && SendProcess == 0){

			SendMessageStack[PopStackPointer].status = 10; //Set Status to 10 "sent to slave"

	        //strcpy(String, SendMessageStack[PopStackPointer].MESSAGE);

	        WriteMessage (PopStackPointer);  //Write Message to the UART_transmit

	  		MessageID = SendMessageStack[PopStackPointer].Message_ID;
			UART_DMA_OUT[181] = SendMessageStack[PopStackPointer].Message_ID  & 0x00FF; //low byte
			UART_DMA_OUT[182] = SendMessageStack[PopStackPointer].Message_ID >> 8; //high byte

	  		UART_DMA_OUT[183] = SendMessageStack[PopStackPointer].status; //Put the Status on the UART_transmit

	  		payload = SendMessageStack[PopStackPointer].payload;
	  		memcpy(data, &payload, sizeof (payload));
	  		a = data[0]; b = data[1];c = data[2];d = data[3];
	  		UART_DMA_OUT[171]=d;
	  		UART_DMA_OUT[172]=c;
	  		UART_DMA_OUT[173]=b;
	  		UART_DMA_OUT[174]=a;

	  		SendProcess = 1;
		}




}
//#######################################################################//


//#######################################################################//

void getMessageToReciveStack()
{
	#ifdef DISPLAY
	#else
	ReceiveMessageStackPointer_helper = ReceiveMessageStackPointer;
	#endif

	MessageID_RECEIVE = (UARTDATA_CHECKED[182]<<8) | (UARTDATA_CHECKED[181] & 0xFF);
	UART_DMA_OUT[184] = MessageID_RECEIVE & 0x00FF;
	UART_DMA_OUT[185] = MessageID_RECEIVE >> 8;
	if (UARTDATA_CHECKED[183] == 10)
	{

		int new_ID = 0;
		for(int i=0; i<MAXSTACK; i++)
		{
			if(MessageID_RECEIVE == ReceiveMessageStack[i].Message_ID){
			new_ID = 0;
			break;
			}
			else
			{
			new_ID = 1;
			}
		}
		/*
		//Find youngest Message
		int index_youngest = 0;
		int id_youngest = 9999999;

		for(int i=0; i<MAXSTACK; i++)
		{
			if (ReceiveMessageStack[i].Message_ID < id_youngest){
				id_youngest = ReceiveMessageStack[i].Message_ID;
			}

			index_youngest = i;
		}
		*/
		if(new_ID)
		{

				if (ReceiveMessageStackPointer >= MAXSTACK)
				{
					ReceiveMessageStackPointer = 0;			//Reset WriteStackPointer if end of Queue is reached
				}


				for(int i = 100; i < 170; i++)
				{
					ReceiveMessageStack[ReceiveMessageStackPointer].MESSAGE[i-100] = UARTDATA_CHECKED[i];
				}


				ReceiveMessageStack[ReceiveMessageStackPointer].Message_ID = MessageID_RECEIVE;
				ReceiveMessageStack[ReceiveMessageStackPointer].status = 99; // MARK as unconsumed
				ReceiveMessageStack[ReceiveMessageStackPointer].payload = RecreateFloats(171);

				ReceiveMessageStackPointer++;
		}
	}
	#ifdef DISPLAY
	#else
	MessageID_RECEIVE_helper = MessageID_RECEIVE;
	#endif

}



void WriteMessage (uint32_t PopStackPointer_writing){

		int i = 0;

		while (i <= 70){
			  UART_DMA_OUT[i+100]=0x00;
			  UART_DMA_OUT[i+100]=SendMessageStack[PopStackPointer_writing].MESSAGE[i];
			  i++;
		  }

}

float RecreateFloats(int startadress){
	   /************Input Data to float**************/
	static int INT;
	static float result;
	   INT = UARTDATA_CHECKED[startadress+3] 			|
	   		(UARTDATA_CHECKED[startadress+2] << 8) 		|
	   		(UARTDATA_CHECKED[startadress+1] << 16) 	|
	   		(UARTDATA_CHECKED[startadress] << 24);

	   memcpy(&result, &INT, sizeof(result));

	   if (result > 20){
		  // result = 20;
	   }

	   return result;
	   /*********************************************/
}
