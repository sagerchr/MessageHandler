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
int SendMessageStackPointer,PopStackPointer, ReceiveMessageStackPointer, UnsentMessages;
void InitMeassageHandler(){
	SendMessageStackPointer = -1;
	PopStackPointer = -1;
	ReceiveMessageStackPointer = -1;
	ID_COUNT = 0;

	for(int i=0; i<MAXSTACK; i++)
				{
					ReceiveMessageStack[i].Message_ID = 29;
				}
}
//###################################################//

//###################################################//
//######## Push a new Message to the Stack ##########//
//###################################################//
void pushToMessageQueue(char *String)
{
	if (SendMessageStackPointer == MAXSTACK-1){
		SendMessageStackPointer = -1;			//Reset WriteStackPointer if end of Queue is reached
		}
		SendMessageStackPointer = SendMessageStackPointer + 1; //Increment Write StackPointer

        strcpy(SendMessageStack[SendMessageStackPointer].MESSAGE, String); //Fill Message to the Stack
        SendMessageStack[SendMessageStackPointer].Message_ID = ID_COUNT++; //Set Message_ID
        SendMessageStack[SendMessageStackPointer].status = 5; //Set Status to 5 "not yet sent to slave"
}


//#####################################################################//
//####### Pop oldest Message from the Stack and write to UART #########//
//#####################################################################//
void popFromMessageQueue()
{
	char *String;

	//################Check if the last Message was read successfully bz the slave####################//
	ReceivedMessageID = (UARTDATA_CHECKED[185]<<8) | (UARTDATA_CHECKED[184] & 0xFF);
	if(ReceivedMessageID == MessageID && SendProcess == 1){
		  SendMessageStack[PopStackPointer].status = 40; //Check Message as successfully read by slave
		  UART_DMA_OUT[183] = 0; //Tell slave there is no Message to read
		  SendProcess = 0; //Send Process is done
	}


	//################################################################################################//


		if(SendProcess == 0){
			if (PopStackPointer == (MAXSTACK-1))
			{
				PopStackPointer = -1;
			}
			PopStackPointer = PopStackPointer+1;
		}



		if(SendMessageStack[PopStackPointer].status == 5 && SendProcess == 0){

			SendMessageStack[PopStackPointer].status = 10; //Set Status to 10 "sent to slave"

	        strcpy(String, SendMessageStack[PopStackPointer].MESSAGE);

	  		WriteMessage (String);  //Write Message to the UART_transmit

	  		MessageID = SendMessageStack[PopStackPointer].Message_ID;
	  		UART_DMA_OUT[181] = SendMessageStack[PopStackPointer].Message_ID  & 0x00FF; //low byte
	  		UART_DMA_OUT[182] = SendMessageStack[PopStackPointer].Message_ID >> 8; //high byte

	  		UART_DMA_OUT[183] = SendMessageStack[PopStackPointer].status; //Put the Status on the UART_transmit

	  		SendProcess = 1;
		}

	#ifdef DISPLAY
	#else
	PopStackPointer_helper = PopStackPointer;
	#endif


}
//#######################################################################//


//#######################################################################//

void getMessageToReciveStack()
{

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

		if(new_ID)
		{

				if (ReceiveMessageStackPointer >= MAXSTACK)
				{
					ReceiveMessageStackPointer = 0;			//Reset WriteStackPointer if end of Queue is reached
				}

				for(int i = 100; i < 180; i++)
				{
					ReceiveMessageStack[ReceiveMessageStackPointer].MESSAGE[i-100] = UARTDATA_CHECKED[i];
				}
				//Show Master that Message was read succesfully


				ReceiveMessageStack[ReceiveMessageStackPointer].Message_ID = MessageID_RECEIVE;
				ReceiveMessageStack[ReceiveMessageStackPointer].status = 99; // MARK as unconsumed
				ReceiveMessageStack[ReceiveMessageStackPointer].payload = (UARTDATA_CHECKED[187]<<8) | (UARTDATA_CHECKED[186] & 0xFF);
				ReceiveMessageStackPointer++;


		}
	}
	#ifdef DISPLAY
	#else
	MessageID_RECEIVE_helper = MessageID_RECEIVE;
	#endif

}



void WriteMessage (char *string){

		for(int i = 100; i < 180; i++){
			UART_DMA_OUT[i]=0x00;
		}

		int i = 0;

		  while ((*(string+i) != '\r' && *(string+i+1) != '\n') || i == 80){

			  UART_DMA_OUT[i+100]=SendMessageStack[PopStackPointer].MESSAGE[i];

			  i++;
		  }

}
