/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "Includes.h"
#include AVR_H
#include COMIF_H
#include UART_DRIVER_H
#include SIMCOM_CALLS_H
#include EEPROMWRAPPER_H
#include MESSAGE_APP_H
#include MQTT_APPLICATION_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/


/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

typedef UBYTE (*AvrRxHanler)(UBYTE);

/*****************************************/
/* Global Variables                      */
/*****************************************/

BOOL IsUartBusBusy = FALSE;

AVR_IO_Control_ST IO_cmdData_AVR;

AVR_IO_Control_ST AVR_IO_Status;

ADC_Data_ST ADCDatas;

DTMF_Command_Data_ST DTMFCommandData;

AVR_Message_ST AVR_Message;

AVR_Call_ST AVR_Calls;

AvrCmdStatusData_ST AvrStatusData;

UBYTE avrstatus[2];

AvrRxHanler AvrRxHanlerType1;

MobileNumbers_ST MobileNumberData;

/***************************************************/
/* Function Declarations                           */
/***************************************************/

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

/***************************************************/
/* Function Definitions                            */
/***************************************************/

UBYTE AVR_Transmit(UWORD Length, void * Data)
{
	UBYTE retval = COMIF_EC_GENERIC_ERROR;
	
	if(IsUartCommunicationOkay == TRUE)
	{
		if(IsUartBusBusy == FALSE)
		{
			for(int i=0;i<Length;i++)
			{
				if(UART2_TransmitByte(*((UB*)(Data++))))
				{
					retval = COMIF_EC_NO_ERROR;
				}
				else
				{
					retval = COMIF_EC_TRANSMISSION_ABORTED;
					break;
				}
				
				IsUartBusBusy = TRUE;
			}
			
			if(retval == COMIF_EC_NO_ERROR)
			{
				ComIf_TxConfirmation(C_ComIfChannel_AVR); 
			}
			
			IsUartBusBusy = FALSE;
		}
		else
		{
			retval = COMIF_EC_CHANNEL_BUSY;
		}
	}
	
	return retval;
}


void AVR_CallRequestRxCbk(UBYTE Length, UBYTE *Data)
{
	AVR_Call_ST *Buff = &AVR_Calls;
	
	if(Length == ComIf_GetLength_AVR_AVR_Call())
	{
			Buff->Byte = *Data;
	}
	
	if(Buff->Call == TRUE)
	{
		SIMCOM_Calls_Dial(&StoredMNs[Buff->Call_MN_Index].MobNo[3]);
		
		Buff->Call = FALSE;
	}
}

void TextMessageRxCbk(UBYTE Length, UBYTE *Data)
{
	AVR_Message_ST *Msg = &AVR_Message;
	
	if(Length == ComIf_GetLength_AVR_AVR_TextMessage())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Msg->Bytes[i] = *(Data++);
		}
		IsSendMessageFlag = TRUE;
	}
}

void ADC_RxCbk(UBYTE Length, UBYTE *Data)
{
	
}

void AVR_IO_StatusRxCbk(UBYTE Length, UBYTE *Data)
{
	for(UBYTE i=0;i<Length;i++)
	{
		AvrStatusData.Data_Bytes[i] = *Data;
		Data++;
	}
	MQTT_Init();
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
	
void UpdateEEPROMNumbers()
{
	MobileNumberData.DTMF_MN_Index = 0;
	
	for(UBYTE i = 0;i< 6;i++ )
	{
		for(UBYTE j =0;j<13;j++)
		{
			MobileNumberData.StoredMNs[i].MobileNumber[j] = StoredMNs[i].MobNo[j];
		}
	}
}

