/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include <stdio.h>
#include <string.h>
#include PLATFORM_TYPES_H
#include MESSAGE_APP_H
#include FLASH_EEPROM_H
#include SIMCOM_MESSAGE_H
#include SIMCOM_H
#include SIMCOM_CALLS_H
#include EEPROMWRAPPER_H
#include AVR_H
#include UART_DRIVER_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

BOOL IsSendMessageFlag = FALSE;

UBYTE CurrentMotorStatus = 0;

UBYTE MsgUpdationCompleteFlag = FALSE;

uint32_t EEPROMAdressByte = 0x08007000;

UBYTE PreviousMotorStatus = 0;

char strCheck[100];

DtmfMessageHandler_En DtmfMessageHandlerState = IdleState;

UBYTE OwnerMessageCount = 0;


/***************************************************/
/* Function Declarations                           */
/***************************************************/

/***************************************************/
/* Function Definitions                            */
/***************************************************/


void DtmfMessageCallFunc()
{
		switch(DtmfMessageHandlerState)
		{
			case IdleState:
			{
	
			}
			break;
			case SendMultipleMessage:
			{
				UBYTE Data;	
				Data = FlashDataRead(EEPROMAdressByte);
//				for(UBYTE i=0;i<13;i++)
//				{
//					MobNumber[i] = readD.MobNo[i];
//				}
				EEPROMAdressByte = EEPROMAdressByte+16;
				if(Data == 1)
				{
					SendMessage(strCheck);
					DtmfMessageHandlerState = TextMessageProcessing;					
				}
				if(EEPROMAdressByte == 0x08007040)
				{
					DtmfMessageHandlerState = IdleState;
					SendMSG_State = MSG_Idle;
					EEPROMAdressByte = 0x08007000;
					MsgUpdationCompleteFlag = TRUE;
				}
			}
			break;
			case TextMessageProcessing:
				
			break;
			default:
				
			break;
		}
}



void Send_TextMsgMain()
{
	if(IsSendMessageFlag == TRUE)
	{
		AVR_Message_ST *Msg = &AVR_Message;
		
		Send_TextMessage((char*)Msg->Message,Msg->Msg_MN_Index);
		
		IsSendMessageFlag = FALSE;
	}
}

