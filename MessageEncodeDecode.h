/*
 * MessageEncodeDecode.h
 *
 *  Created on: 8 maj 2021
 *      Author: es015742
 */

#ifndef APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_MESSAGEENCODEDECODE_H_
#define APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_MESSAGEENCODEDECODE_H_

void InitMeassageHandler();
void sendMessage(char *String, float payload);
void popFromMessageQueue();
void getMessageToReciveStack();
static void WriteMessage (uint32_t PopStackPointer_writing);
float RecreateFloats(int startadress);



uint8_t SendProcess;

char *OldestMessage;



#endif /* APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_MESSAGEENCODEDECODE_H_ */
