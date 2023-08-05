/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "Includes.h"
#include AVR_H
#include COMIF_H
#include UART_DRIVER_H
#include SIMCOM_CALLS_H
#include EEPROMWRAPPER_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/


/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/
BOOL IsUartBusBusy = FALSE;

AVR_IO_Control_ST IO_cmdData_AVR;
AVR_IO_Control_ST AVR_IO_Status;

ADC_Data_ST ADCDatas;
DTMF_Command_Data_ST DTMFCommandData;
AVR_Message_ST AVR_Message;
AVR_Call_ST AVR_Calls;
/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

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
		SIMCOM_Calls_Dial(&AdressCpy[Buff->Call_MN_Index].MobNo[3]);
		
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
	}
}

void ADC_RxCbk(UBYTE Length, UBYTE *Data)
{
	
}
