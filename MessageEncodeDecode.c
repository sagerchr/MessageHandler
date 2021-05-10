/*
 * MessageEncodeDecode.c
 *
 *  Created on: 8 maj 2021
 *      Author: es015742
 */

#include "MessageHandlerTask.h"


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
