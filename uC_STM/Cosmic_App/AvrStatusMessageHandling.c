#include "Includes.h"
#include PLATFORM_TYPES_H
#include COMIF_H
#include COMIF_CONFIG_H
#include BUFFER_CONFIG_H
#include UART_DRIVER_H
#include MQTT_APPLICATION_H


void UART2_SIM_Send_Data(unsigned char Data);

extern void AvrStatusRxFunc(UBYTE Length, UBYTE *Data);

extern UBYTE ComIf_RxIndication_Avr(UBYTE DataByte);

extern void updateSendData(UBYTE Data[]);

extern void RequestLastStatus(UBYTE sts[]);

UBYTE avrstatus[2];

typedef UBYTE (*AvrRxHanler)(UBYTE);

AvrRxHanler AvrRxHanlerType1;

void AvrStatusRxFunc(UBYTE Length, UBYTE *Data)
{
	for(UBYTE i=0;i<Length;i++)
	{
		avrstatus[i] = *Data;
		Data++;
	}
	RequestLastStatus(avrstatus);
	MQTTApp_State = MQTTApp_Publish_IO_state;
}

void UpdateStatusCmdData()
{
	
}

void 	AvrStatusHandleFunc()
{
	while(!AvrStatus_IsBufferEmpty())
	{
		UBYTE data = 0;
		
		if(AvrStatus_Buffer_DeQueue(&data))
		{
			ComIf_RxIndication_Avr(data);
		}
	}
}