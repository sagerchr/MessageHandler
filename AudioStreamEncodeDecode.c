

#include "main.h"
#include "MessageHandlerTask.h"
#include <string.h>

float createFloats(int startadress){
	   /************Input Data to float**************/
	static int INT;
	static float result;
	   INT = UARTDATA_CHECKED[startadress+3] 			|
	   		(UARTDATA_CHECKED[startadress+2] << 8) 		|
	   		(UARTDATA_CHECKED[startadress+1] << 16) 	|
	   		(UARTDATA_CHECKED[startadress] << 24);

	   memcpy(&result, &INT, sizeof(result));

	   if (result > 20){
		   result = 20;
	   }

	   return result;
	   /*********************************************/
}

float f_inputLeft_DB_temp;

void DecodeAudioStream(){

		i_inputLeft_Indicator = maxval1;
		i_inputRight_Indicator = maxval2;
		i_VCALeft_Indicator = maxval3;
		i_VCARight_Indicator = maxval4;
		i_outputLeft_Indicator = maxval5;
		i_outputRight_Indicator = maxval6;

	  Indicator.i_VCALeft_Indicator = i_VCALeft_Indicator;
	  Indicator.i_VCARight_Indicator = i_VCARight_Indicator;
	  Indicator.i_inputLeft_Indicator = i_inputLeft_Indicator;
	  Indicator.i_inputRight_Indicator = i_inputRight_Indicator;
	  Indicator.i_outputLeft_Indicator = i_outputLeft_Indicator;
	  Indicator.i_outputRight_Indicator = i_outputRight_Indicator;

	  if (i_inputLeft_Indicator > i_inputLeft_Indicator_bufferd){i_inputLeft_Indicator_bufferd = i_inputLeft_Indicator;}
	  else {i_inputLeft_Indicator_bufferd -=0.01*(i_inputLeft_Indicator_bufferd-i_inputLeft_Indicator);}

	  if (i_inputLeft_Waveform > i_inputLeft_Indicator_MAXbufferd){i_inputLeft_Indicator_MAXbufferd = i_inputLeft_Indicator;}
	  else {i_inputLeft_Indicator_MAXbufferd -=0.001*(i_inputLeft_Indicator_MAXbufferd-i_inputLeft_Indicator);}


	  if (i_inputRight_Indicator > i_inputRight_Indicator_bufferd){i_inputRight_Indicator_bufferd = i_inputRight_Indicator;}
	  else {i_inputRight_Indicator_bufferd -=0.01*(i_inputRight_Indicator_bufferd-i_inputRight_Indicator);}

	  if (i_inputRight_Indicator > i_inputRight_Indicator_MAXbufferd){i_inputRight_Indicator_MAXbufferd = i_inputRight_Indicator;}
	  else {i_inputRight_Indicator_MAXbufferd -=0.001*(i_inputRight_Indicator_MAXbufferd-i_inputRight_Indicator);}


	  if (i_outputLeft_Indicator > i_outputLeft_Indicator_bufferd){i_outputLeft_Indicator_bufferd = i_outputLeft_Indicator;}
	  else {i_outputLeft_Indicator_bufferd -=0.01*(i_outputLeft_Indicator_bufferd-i_outputLeft_Indicator);}

	  if (i_outputLeft_Indicator> i_outputLeft_Indicator_MAXbufferd){i_outputLeft_Indicator_MAXbufferd = i_outputLeft_Indicator;}
	  else {i_outputLeft_Indicator_MAXbufferd -=0.001*(i_outputLeft_Indicator_MAXbufferd-i_outputLeft_Indicator);}


	  if (i_outputRight_Indicator > i_outputRight_Indicator_bufferd){i_outputRight_Indicator_bufferd = i_outputRight_Indicator;}
	  else {i_outputRight_Indicator_bufferd -=0.01*(i_outputRight_Indicator_bufferd-i_outputRight_Indicator);}

	  if (i_outputRight_Indicator > i_outputRight_Indicator_MAXbufferd){i_outputRight_Indicator_MAXbufferd = i_outputRight_Indicator;}
	  else {i_outputRight_Indicator_MAXbufferd -=0.001*(i_outputRight_Indicator_MAXbufferd-i_outputRight_Indicator);}


//*****************************************************************************************************************//
//***********************************INPUT SECTION FLOAT BUFFERED***********************************************************//
//*****************************************************************************************************************//


 	 	f_inputLeft_DB = createFloats(12);
		f_inputRight_DB = createFloats(16);
		f_VCALeft_DB = createFloats(20);
		f_VCARight_DB = createFloats(24);
		f_outputLeft_DB = createFloats(28);
		f_outputRight_DB = createFloats(32);
		f_inputLeft_DB_RMS = createFloats(36);
		f_inputRight_DB_RMS = createFloats(40);
		f_VCALeft_DB_RMS = createFloats(44);
		f_VCARight_DB_RMS = createFloats(48);
		f_outputLeft_DB_RMS = createFloats(52);
		f_outputRight_DB_RMS = createFloats(56);



/////////INPUT LEFT
	  float diff1=0;
	  diff1 = (f_inputLeft_DB_bufferd-f_inputLeft_DB);
	  if(diff1<0){diff1=(-1)*diff1;}
	  if (f_inputLeft_DB > f_inputLeft_DB_bufferd){f_inputLeft_DB_bufferd = f_inputLeft_DB;}
	  else {f_inputLeft_DB_bufferd -=p_Bufferd*diff1;}
	  InputLeft.db = f_inputLeft_DB_bufferd;

	  float diff2=0;
	  diff2 = (f_inputLeft_DB_MAXbufferd-f_inputLeft_DB);
	  if(diff2<0){diff2=(-1)*diff2;}
	  if (f_inputLeft_DB > f_inputLeft_DB_MAXbufferd){f_inputLeft_DB_MAXbufferd = f_inputLeft_DB;}
	  else {f_inputLeft_DB_MAXbufferd -=p_MAXBufferd*diff2;}
	  InputLeft.db_max = f_inputLeft_DB_MAXbufferd;
/////////INPUT RIGHT
	  float diff3=0;
	  diff3 = (f_inputRight_DB_bufferd-f_inputRight_DB);
	  if(diff3<0){diff3=(-1)*diff3;}
	  if (f_inputRight_DB > f_inputRight_DB_bufferd){f_inputRight_DB_bufferd = f_inputRight_DB;}
	  else {f_inputRight_DB_bufferd -=p_Bufferd*diff3;}
	  InputRight.db = f_inputRight_DB_bufferd;

	  float diff4=0;
	  diff4 = (f_inputRight_DB_MAXbufferd-f_inputRight_DB);
	  if(diff4<0){diff4=(-1)*diff4;}
	  if (f_inputRight_DB > f_inputRight_DB_MAXbufferd){f_inputRight_DB_MAXbufferd = f_inputRight_DB;}
	  else {f_inputRight_DB_MAXbufferd -=p_MAXBufferd*diff4;}
	  InputRight.db_max =f_inputRight_DB_MAXbufferd;
/////////INPUT LEFT_RMS
	  float diff5=0;
	  diff5 = (f_inputLeft_DB_RMS_bufferd-f_inputLeft_DB_RMS);
	  if(diff5<0){diff5=(-1)*diff5;}
	  if (f_inputLeft_DB_RMS > f_inputLeft_DB_RMS_bufferd){f_inputLeft_DB_RMS_bufferd = f_inputLeft_DB_RMS;}
	  else {f_inputLeft_DB_RMS_bufferd -=p_Bufferd*diff5;}
	  InputLeft.RMS = f_inputLeft_DB_RMS_bufferd;

	  float diff6=0;
	  diff6 = (f_inputLeft_DB_RMS_MAXbufferd-f_inputLeft_DB_RMS);
	  if(diff6<0){diff6=(-1)*diff6;}
	  if (f_inputLeft_DB_RMS > f_inputLeft_DB_RMS_MAXbufferd){f_inputLeft_DB_RMS_MAXbufferd = f_inputLeft_DB_RMS;}
	  else {f_inputLeft_DB_RMS_MAXbufferd -=p_MAXBufferd*diff6;}
	  InputLeft.RMS_max = f_inputLeft_DB_RMS_MAXbufferd;
/////////INPUT RIGHT_RMS
	  float diff7=0;
	  diff7 = (f_inputRight_DB_RMS_bufferd-f_inputRight_DB_RMS);
	  if(diff7<0){diff7=(-1)*diff7;}
	  if (f_inputRight_DB_RMS > f_inputRight_DB_RMS_bufferd){f_inputRight_DB_RMS_bufferd = f_inputRight_DB_RMS;}
	  else {f_inputRight_DB_RMS_bufferd -=p_Bufferd*diff7;}
	  InputRight.RMS = f_inputRight_DB_RMS_bufferd;

	  float diff8=0;
	  diff8 = (f_inputRight_DB_RMS_MAXbufferd-f_inputRight_DB_RMS);
	  if(diff8<0){diff8=(-1)*diff8;}
	  if (f_inputRight_DB_RMS > f_inputRight_DB_RMS_MAXbufferd){f_inputRight_DB_RMS_MAXbufferd = f_inputRight_DB_RMS;}
	  else {f_inputRight_DB_RMS_MAXbufferd -=p_MAXBufferd*diff8;}
	  InputRight.RMS_max = f_inputRight_DB_RMS_MAXbufferd;
//*****************************************************************************************************************//
//*****************************************************************************************************************//
//*****************************************************************************************************************//

/////////VCA LEFT
	  float diff9=0;
	  diff9 = (f_VCALeft_DB_bufferd-f_VCALeft_DB);
	  if(diff9<0){diff9=(-1)*diff9;}
	  if (f_VCALeft_DB > f_VCALeft_DB_bufferd){f_VCALeft_DB_bufferd = f_VCALeft_DB;}
	  else {f_VCALeft_DB_bufferd -=p_Bufferd*diff9;}
	  VCALeft.db = f_VCALeft_DB_bufferd;

	  float diff10=0;
	  diff10 = (f_VCALeft_DB_MAXbufferd-f_VCALeft_DB);
	  if(diff10<0){diff10=(-1)*diff10;}
	  if (f_VCALeft_DB > f_VCALeft_DB_MAXbufferd){f_VCALeft_DB_MAXbufferd = f_VCALeft_DB;}
	  else {f_VCALeft_DB_MAXbufferd -=p_MAXBufferd*diff10;}
	  VCALeft.db_max = f_VCALeft_DB_MAXbufferd;
/////////VCA RIGHT
	  float diff11=0;
	  diff11 = (f_VCARight_DB_bufferd-f_VCARight_DB);
	  if(diff11<0){diff11=(-1)*diff11;}
	  if (f_VCARight_DB > f_VCARight_DB_bufferd){f_VCARight_DB_bufferd = f_VCARight_DB;}
	  else {f_VCARight_DB_bufferd -=p_Bufferd*diff11;}
	  VCARight.db = f_VCARight_DB_bufferd;

	  float diff12=0;
	  diff12 = (f_VCARight_DB_MAXbufferd-f_VCARight_DB);
	  if(diff12<0){diff12=(-1)*diff12;}
	  if (f_VCARight_DB > f_VCARight_DB_MAXbufferd){f_VCARight_DB_MAXbufferd = f_VCARight_DB;}
	  else {f_VCARight_DB_MAXbufferd -=p_MAXBufferd*diff12;}
	  VCARight.db_max = f_VCARight_DB_MAXbufferd;
/////////VCA LEFT RMS
	  float diff13=0;
	  diff13 = (f_VCALeft_DB_RMS_bufferd-f_VCALeft_DB_RMS);
	  if(diff13<0){diff13=(-1)*diff13;}
	  if (f_VCALeft_DB_RMS > f_VCALeft_DB_RMS_bufferd){f_VCALeft_DB_RMS_bufferd = f_VCALeft_DB_RMS;}
	  else {f_VCALeft_DB_RMS_bufferd -=p_Bufferd*diff13;}
	  VCALeft.RMS = f_VCALeft_DB_RMS_bufferd;

	  float diff14=0;
	  diff14 = (f_VCALeft_DB_RMS_MAXbufferd-f_VCALeft_DB_RMS);
	  if(diff14<0){diff14=(-1)*diff14;}
	  if (f_VCALeft_DB_RMS > f_VCALeft_DB_RMS_MAXbufferd){f_VCALeft_DB_RMS_MAXbufferd = f_VCALeft_DB_RMS;}
	  else {f_VCALeft_DB_RMS_MAXbufferd -=p_MAXBufferd*diff14;}
	  VCALeft.RMS_max = f_VCALeft_DB_RMS_MAXbufferd;
/////////VCA RIGHT RMS
	  float diff15=0;
	  diff15 = (f_VCARight_DB_RMS_bufferd-f_VCARight_DB_RMS);
	  if(diff15<0){diff15=(-1)*diff15;}
	  if (f_VCARight_DB_RMS > f_VCARight_DB_RMS_bufferd){f_VCARight_DB_RMS_bufferd = f_VCARight_DB_RMS;}
	  else {f_VCARight_DB_RMS_bufferd -=p_Bufferd*diff15;}
	  VCARight.RMS = f_VCARight_DB_RMS_bufferd;

	  float diff16=0;
	  diff16 = (f_VCARight_DB_RMS_MAXbufferd-f_VCARight_DB_RMS);
	  if(diff16<0){diff16=(-1)*diff16;}
	  if (f_VCARight_DB_RMS > f_VCARight_DB_RMS_MAXbufferd){f_VCARight_DB_RMS_MAXbufferd = f_VCARight_DB_RMS;}
	  else {f_VCARight_DB_RMS_MAXbufferd -=p_MAXBufferd*diff16;}
	  VCARight.RMS_max = f_VCARight_DB_RMS_MAXbufferd;
//*****************************************************************************************************************//
//*****************************************************************************************************************//
//*****************************************************************************************************************//

	  float diff=0;
/////////OUTPUT LEFT
	  diff=0;
	  diff = (f_outputLeft_DB_bufferd-f_outputLeft_DB);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputLeft_DB > f_outputLeft_DB_bufferd){f_outputLeft_DB_bufferd = f_outputLeft_DB;}
	  else {f_outputLeft_DB_bufferd -=p_Bufferd*diff;}
	  OutputLeft.db = f_outputLeft_DB_bufferd;

	  diff=0;
	  diff = (f_outputLeft_DB_MAXbufferd-f_outputLeft_DB);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputLeft_DB > f_outputLeft_DB_MAXbufferd){f_outputLeft_DB_MAXbufferd = f_outputLeft_DB;}
	  else {f_outputLeft_DB_MAXbufferd -=p_MAXBufferd*diff;}
	  OutputLeft.db_max = f_outputLeft_DB_MAXbufferd;
/////////OUTPUT RIGHT
	  diff=0;
	  diff = (f_outputRight_DB_bufferd-f_outputRight_DB);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputRight_DB > f_outputRight_DB_bufferd){f_outputRight_DB_bufferd = f_outputRight_DB;}
	  else {f_outputRight_DB_bufferd -=p_Bufferd*diff;}
	  OutputRight.db = f_outputRight_DB_bufferd;

	  diff=0;
	  diff = (f_outputRight_DB_MAXbufferd-f_outputRight_DB);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputRight_DB > f_outputRight_DB_MAXbufferd){f_outputRight_DB_MAXbufferd = f_outputRight_DB;}
	  else {f_outputRight_DB_MAXbufferd -=p_MAXBufferd*diff;}
	  OutputRight.db_max = f_outputRight_DB_MAXbufferd;
/////////OUTPUT LEFT RMS
	  diff=0;
	  diff = (f_outputLeft_DB_RMS_bufferd-f_outputLeft_DB_RMS);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputLeft_DB_RMS > f_outputLeft_DB_RMS_bufferd){f_outputLeft_DB_RMS_bufferd = f_outputLeft_DB_RMS;}
	  else {f_outputLeft_DB_RMS_bufferd -=p_Bufferd*diff;}
	  OutputLeft.RMS = f_outputLeft_DB_RMS_bufferd;

	  diff=0;
	  diff = (f_outputLeft_DB_RMS_MAXbufferd-f_outputLeft_DB_RMS);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputLeft_DB_RMS > f_outputLeft_DB_RMS_MAXbufferd){f_outputLeft_DB_RMS_MAXbufferd = f_outputLeft_DB_RMS;}
	  else {f_outputLeft_DB_RMS_MAXbufferd -=p_MAXBufferd*diff;}
	  OutputLeft.RMS_max = f_outputLeft_DB_RMS_MAXbufferd;
/////////OUTPUT RIGHT RMS
	  diff=0;
	  diff = (f_outputRight_DB_RMS_bufferd-f_outputRight_DB_RMS);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputRight_DB_RMS > f_outputRight_DB_RMS_bufferd){f_outputRight_DB_RMS_bufferd = f_outputRight_DB_RMS;}
	  else {f_outputRight_DB_RMS_bufferd -=p_Bufferd*diff;}
	  OutputRight.RMS = f_outputRight_DB_RMS_bufferd;

	  diff=0;
	  diff = (f_outputRight_DB_RMS_MAXbufferd-f_outputRight_DB_RMS);
	  if(diff<0){diff=(-1)*diff;}
	  if (f_outputRight_DB_RMS > f_outputRight_DB_RMS_MAXbufferd){f_outputRight_DB_RMS_MAXbufferd = f_outputRight_DB_RMS;}
	  else {f_outputRight_DB_RMS_MAXbufferd -=p_MAXBufferd*diff;}
	  OutputRight.RMS_max = f_outputRight_DB_RMS_MAXbufferd;



		for(int i=0; i<50; i++){
			if (f_spectrumLeft_bufferd[i]<UARTDATA[i+50]){f_spectrumLeft_bufferd[i] = UARTDATA[i+50];}
			else {f_spectrumLeft_bufferd[i] -=p_Bufferd*(f_spectrumLeft_bufferd[i]-UARTDATA[i+50]);}
		}

		AudioStreamToModel.InputLeft = InputLeft;
		AudioStreamToModel.InputRight = InputRight;
		AudioStreamToModel.OutputLeft = OutputLeft;
		AudioStreamToModel.OutputRight = OutputRight;
		AudioStreamToModel.VCALeft = VCALeft;
		AudioStreamToModel.VCARight = VCARight;
		AudioStreamToModel.Indicator = Indicator;

}




void EncodeAudioStream(){

	 	 	  char data[sizeof(float)];
			  float f = -1.236;
			  char a = data[0];char b = data[1];char c = data[2];char d = data[3];

			  UART_DMA_OUT[0]='#';
			  UART_DMA_OUT[1]='s';
			  UART_DMA_OUT[2]='t';
			  UART_DMA_OUT[3]='a';
			  //UART_DMA_OUT[4]=upcounter;//3

			  UART_DMA_OUT[5]=0x02;//2
			  UART_DMA_OUT[6]=voltageIn1MAX*100;//3
			  UART_DMA_OUT[7]=voltageIn2MAX*100;//4
			  UART_DMA_OUT[8]=voltageIn3MAX*100;//7
			  UART_DMA_OUT[9]=voltageIn4MAX*100;//7
			  UART_DMA_OUT[10]=voltageIn5MAX*100;//7
			  UART_DMA_OUT[11]=voltageIn6MAX*100;//8

			  memcpy(data, &dbuMAX[0], sizeof &dbuMAX[0]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[12]=d;
			  UART_DMA_OUT[13]=c;
			  UART_DMA_OUT[14]=b;
			  UART_DMA_OUT[15]=a;

			  memcpy(data, &dbuMAX[1], sizeof &dbuMAX[1]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[16]=d;
			  UART_DMA_OUT[17]=c;
			  UART_DMA_OUT[18]=b;
			  UART_DMA_OUT[19]=a;

			  memcpy(data, &dbuMAX[2], sizeof &dbuMAX[2]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[20]=d;
			  UART_DMA_OUT[21]=c;
			  UART_DMA_OUT[22]=b;
			  UART_DMA_OUT[23]=a;

			  memcpy(data, &dbuMAX[3], sizeof &dbuMAX[3]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[24]=d;
			  UART_DMA_OUT[25]=c;
			  UART_DMA_OUT[26]=b;
			  UART_DMA_OUT[27]=a;

			  memcpy(data, &dbuMAX[4], sizeof &dbuMAX[4]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[28]=d;
			  UART_DMA_OUT[29]=c;
			  UART_DMA_OUT[30]=b;
			  UART_DMA_OUT[32]=a;

			  memcpy(data, &dbuMAX[5], sizeof &dbuMAX[5]);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[32]=d;
			  UART_DMA_OUT[33]=c;
			  UART_DMA_OUT[34]=b;
			  UART_DMA_OUT[35]=a;

			  memcpy(data, &RMS_CH1_long, sizeof &RMS_CH1_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[36]=d;
			  UART_DMA_OUT[37]=c;
			  UART_DMA_OUT[38]=b;
			  UART_DMA_OUT[39]=a;

			  memcpy(data, &RMS_CH2_long, sizeof &RMS_CH2_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[40]=d;
			  UART_DMA_OUT[41]=c;
			  UART_DMA_OUT[42]=b;
			  UART_DMA_OUT[43]=a;

			  memcpy(data, &RMS_CH3_long, sizeof &RMS_CH3_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[44]=d;
			  UART_DMA_OUT[45]=c;
			  UART_DMA_OUT[46]=b;
			  UART_DMA_OUT[47]=a;

			  memcpy(data, &RMS_CH4_long, sizeof &RMS_CH4_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[48]=d;
			  UART_DMA_OUT[49]=c;
			  UART_DMA_OUT[50]=b;
			  UART_DMA_OUT[51]=a;

			  memcpy(data, &RMS_CH5_long, sizeof &RMS_CH5_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[52]=d;
			  UART_DMA_OUT[53]=c;
			  UART_DMA_OUT[54]=b;
			  UART_DMA_OUT[55]=a;

			  memcpy(data, &RMS_CH6_long, sizeof &RMS_CH6_long);    // send data
			  a = data[0]; b = data[1];c = data[2];d = data[3];
			  UART_DMA_OUT[56]=d;
			  UART_DMA_OUT[57]=c;
			  UART_DMA_OUT[58]=b;
			  UART_DMA_OUT[59]=a;


			  UART_DMA_OUT[60]=reductLeft;
			  UART_DMA_OUT[61]=reductRight;
}








