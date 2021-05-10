/*
 * MessageHandlerTask.h
 *
 *  Created on: 5 maj 2021
 *      Author: es015742
 */

#ifndef APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_H_
#define APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_H_

#include "types.h"
#include "SerialProcessing.h"
#include "AudioStreamEncodeDecode.h"
#include "MessageEncodeDecode.h"
#include "valueTable.h"

struct AudioStream AudioStreamToModel;
struct AudioStream AudioStreamFromProcessor;
uint8_t count;
uint8_t UARTsendIntervall;
uint8_t readout;

#ifdef DISPLAY
typedef struct Message {

   char MESSAGE[80];
   int Message_ID;
   int status;
   int payload;
}  Message;
Message ReceiveMessageStack[MAXSTACK];
Message SendMessageStack[MAXSTACK];
#endif

#endif /* APPLICATION_USER_TASKS_MESSAGEHANDLERTASK_H_ */
