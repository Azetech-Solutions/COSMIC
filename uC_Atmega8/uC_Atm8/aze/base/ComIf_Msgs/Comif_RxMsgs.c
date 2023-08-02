/*
 * Comif_RxMsgs.c
 *
 * Created: 02-08-2023 11:29:09
 *  Author: Azetech
 */ 
/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include COMIF_H
#include TR_MSGS_H
#include UART_H
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/
AVR_IO_Control_ST IO_cmdData_AVR;
AVR_IO_Control_ST AVR_IO_Status;

ADC_Data_ST ADCDatas;
DTMF_Command_Data_ST DTMFCommandData;
AVR_Message_ST AVR_Message;
AVR_Call_ST AVR_Calls;

BOOL IsUartBusBusy = FALSE;

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/



void STM32_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1)
{
	
}

UBYTE STM32_Transmit(UWORD Length, void * Data)
{
	UBYTE retval = COMIF_EC_GENERIC_ERROR;
	
	if(IsUartCommunicationOkay == TRUE)
	{
		if(IsUartBusBusy == FALSE)
		{
			for(int i=0;i<Length;i++)
			{
				if(uart_tx(*((UB*)(Data++))))
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
				ComIf_TxConfirmation(C_ComIfChannel_STM32); 
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

void IO_cmdData_AVRRxcbk(UBYTE Length, UBYTE *Data)
{
	AVR_IO_Control_ST *Buff = &IO_cmdData_AVR;
		
	if(Length == ComIf_GetLength_STM32_IO_cmdData_AVR())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Buff->Bytes[i] = *(Data++);
		}
	}
	
}

void DTMF_Command_DataRxCbk(UBYTE Length, UBYTE *Data)
{
	DTMF_Command_Data_ST *Buff = &DTMFCommandData;
		
	if(Length == ComIf_GetLength_STM32_DTMF_Command_Data())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Buff->Bytes[i] = *(Data++);
		}
	}
}

void Stored_MobNumsRxCbk(UBYTE Length, UBYTE *Data)
{
	DTMF_Command_Data_ST *Buff = &DTMFCommandData;
	
	if(Length == ComIf_GetLength_STM32_DTMF_Command_Data())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Buff->Bytes[i] = *(Data++);
		}
	}
}