/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "Includes.h"
#include PLATFORM_TYPES_H
#include COMIF_H
#include COMIF_CONFIG_H
#include BUFFER_CONFIG_H
#include SIMCOM_H
#include UART_DRIVER_H
#include MQTT_APPLICATION_H
#include MESSAGE_APP_H
/*****************************************/
/* Global Variables                      */
/*****************************************/
void UART2_SIM_Send_Data(unsigned char Data);

extern UBYTE DTMFBuffer[15];

extern UBYTE ComIf_RxIndication_Avr(UBYTE DataByte);

extern void updateSendData(UBYTE Data[]);

extern void RequestLastStatus(UBYTE sts[]);

AvrCmdData_ST AvrStat;

UBYTE avrstatus[2];

typedef UBYTE (*AvrRxHanler)(UBYTE);

AvrRxHanler AvrRxHanlerType1;

/***************************************************/
/* Function Definitions                            */
/***************************************************/



void AVR_IO_StatusRxCbk(UBYTE Length, UBYTE *Data)
{
	for(UBYTE i=0;i<Length;i++)
	{
		AvrStat.Data_Bytes[i] = *Data;
		Data++;
	}
	if(AvrStat.SW16 == 1)
	{
		SIMCOM_State = DialMobileNumber;
	}
	else
	{
		CurrentMotorStatus = AvrStat.SW1;
		
		if(PreviousMotorStatus != CurrentMotorStatus)
		{
			if(SimcomWorkingMode == MQTTMode)
			{
				RequestLastStatus(AvrStat.Data_Bytes);
				MQTTApp_State = MQTTApp_Publish_IO_state;			
			}
			else
			{
				DtmfMessageHandlerState = UpdateMotorStatusMsg;
			}
		}
		else
		{
			SIMCOM_State = SIMCOMCancelCall;
		}
		PreviousMotorStatus = CurrentMotorStatus;		
	}
}

void ADC_RxCbk(UBYTE Length, UBYTE *Data)
{
	
}

void 	AvrStatusHandleFunc()
{
	while(!AvrStatus_IsBufferEmpty())
	{
		UBYTE data = 0;
		
		if(AvrStatus_Buffer_DeQueue(&data))
		{
			ComIf_RxIndication_AVR(data);
		}
	}
}
	
