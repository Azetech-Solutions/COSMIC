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
#include <util/delay.h>
#include COMIF_H
#include TR_MSGS_H
#include UART_H
#include COSMIC_APP_H
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
AVR_IO_Control_ST PrevMQTTSatus;
AVR_IO_Control_ST MQTTSatus;

typedef enum MSGMode{
	DTMF = 0,
	MQTT
}MSGMode_EN;

MSGMode_EN MSGMode = MQTT;

MSGMode_EN PrevMsgMode = MQTT;

BOOL IsUartBusBusy = FALSE;

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

void DTMFStatusHandling();

UBYTE PrevDTMFData = 0;

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
	MSGMode = MQTT;
	
	AVR_IO_Control_ST *Buff = &IO_cmdData_AVR;
	
	if(Length == ComIf_GetLength_STM32_IO_cmdData_AVR())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Buff->Bytes[i] = *(Data++);
		}
	}
	checkworking();
}

void msgcallHandling()
{
	if(IO_cmdData_AVR.IO1 == 1)
	{
		MQTTStatus(1);
	}
	if(IO_cmdData_AVR.IO2 == 1)
	{
		Send_Message(3,"MOTOR ON ");
	}
	if(IO_cmdData_AVR.IO3 == 1)
	{
		Call_Request(1,3);
	}
	if(IO_cmdData_AVR.IO4 == 1)
	{
		Send_ADC_Datas();
	}
	PORTC ^= (1<<5);
}

void DTMF_Command_DataRxCbk(UBYTE Length, UBYTE *Data)
{
	MSGMode = DTMF;
	
	DTMF_Command_Data_ST *Buff = &DTMFCommandData;
		
	if(Length == ComIf_GetLength_STM32_DTMF_Command_Data())
	{
		for(UBYTE i = 0;i < Length;i++)
		{
			Buff->Bytes[i] = *(Data++);
		}
	}
	checkworking();
}

void MQTTStatus(UBYTE Sts)
{	
		MQTTSatus.Bytes[0] = Sts;
		
		MQTTSatus.Bytes[1] = 0;
	
		UBYTE *Buff = ComIf_GetShadowBuffer_STM32_AVR_IO_Status();
		
		memcpy(Buff,MQTTSatus.Bytes,2);
		
		ComIf_TransmitFromBuffer_STM32_AVR_IO_Status();	
}

void CMDHandling()
{
	
	if(MSGMode == MQTT)
	{
		if(PrevMQTTSatus.IO1 != IO_cmdData_AVR.IO1)
		{
			if(IO_cmdData_AVR.IO1 == 1)
			{
				PORTC |= (1<<5);
			}
			else if(IO_cmdData_AVR.IO1 == 0)
			{
				PORTC &= ~(1<<5);
			}
			MQTTStatus(IO_cmdData_AVR.Bytes);
		}
	}
	else
	{
		if(DTMFCommandData.DTMF_Data == 1 || DTMFCommandData.DTMF_Data == '1')
		{
			IO_cmdData_AVR.IO1 = 1;
		}
		else if(DTMFCommandData.DTMF_Data == 2 || DTMFCommandData.DTMF_Data == '2')
		{
			IO_cmdData_AVR.IO1 = 0;
		}
		else
		{
			
		}
		if(PrevMQTTSatus.IO1 != IO_cmdData_AVR.IO1)
		{
			if(IO_cmdData_AVR.IO1 == 1)
			{
				PORTC |= (1<<5);
				UBYTE MN = DTMFCommandData.DTMF_MN_Index;
				
				Send_Message(MN-48,"MOTOR ON ");
			}
			else
			{
				PORTC &= ~(1<<5);
				UBYTE MN = DTMFCommandData.DTMF_MN_Index;
				Send_Message(MN-48,"MOTOR OFF");
			}
		}
	}
	PrevMQTTSatus = IO_cmdData_AVR;
	

}

void checkworking()
{
// 		UBYTE avrcmd[Length];
// 		
// 		for(UBYTE i=0;i<Length;i++)
// 		{
// 			avrcmd[i] = *Data;
// 			Data++;
// 		}
	AVR_IO_Control_ST *Buff = &IO_cmdData_AVR;	
	
	if(Buff->IO1 == 1)
	{
		PORTC |= (1<<0);
	}
	else
	{
		PORTC &= ~(1<<0);
	}
	if(Buff->IO2 == 1)
	{
		PORTC |= (1<<1);
	}
	else
	{
		PORTC &= ~(1<<1);
	}
	if(Buff->IO3 == 1)
	{
		PORTC |= (1<<2);
	}
	else
	{
		PORTC &= ~(1<<2);
	}
	if(Buff->IO4 == 1)
	{
		PORTC |= (1<<3);
	}
	else
	{
		PORTC &= ~(1<<3);
	}
	MQTTStatus(Buff->Bytes[0]);
// 	_delay_ms(20000);
// 	Send_Message(3,"MOTOR ON ");
// 	_delay_ms(20000);
}


// void DTMFStatusHandling()
// {
// 		UBYTE MN = DTMFCommandData.DTMF_MN_Index;
// 		if( PrevDTMFData != DTMFCommandData.DTMF_Data)
// 		{
// 			if(DTMFCommandData.DTMF_Data == 1)
// 			{
// 				IO_cmdData_AVR.IO1 = 1;
// 				PORTC |= (1<<5);
// 				Send_Message(MN-48,"MOTOR ON");
// 			}
// 			else if(DTMFCommandData.DTMF_Data == 2)
// 			{
// 				IO_cmdData_AVR.IO1 = 0;
// 				PORTC &= ~(1<<5);
// 				Send_Message(MN-48,"MOTOR OFF");
// 			}
// 			else
// 			{
// 				
// 			}
// 		}
// 		PrevDTMFData = DTMFCommandData.DTMF_Data;
// }

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
