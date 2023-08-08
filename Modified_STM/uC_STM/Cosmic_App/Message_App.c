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

/*****************************************/
/* Global Variables                      */
/*****************************************/
BOOL IsSendMessageFlag = FALSE;

UBYTE CurrentMotorStatus = 0;
UBYTE MsgUpdationCompleteFlag = FALSE;
uint32_t EEPROMAdressByte = 0x08007000;
UBYTE PreviousMotorStatus = 0;
char strCheck[100] = "Machine Ready";
DtmfMessageHandler_En DtmfMessageHandlerState = IdleState;
UBYTE OwnerMessageCount = 0;
void DtmfMessageCallFunc()
{
		switch(DtmfMessageHandlerState)
		{
			case IdleState:
				
				break;
//			case MachineInit:
//			{
//				UBYTE len = strlen(strCheck);
//				strCheck[len] = CARRIAGE_RETURN;
//				strCheck[len+1] = MSGLASTWORD;
//				strCheck[len+2] = '\0';
//				SendMessage(strCheck);
//				DtmfMessageHandlerState = IdleState;
//				MachineInitFlag = FALSE;
//			}
//				break;
//			case NumberUpdateMessage:
//			{
//				memset(strCheck,'\0',100);
//				if(SendMSG_State == MSG_Idle)
//				{
//					sprintf(strCheck,"Number Updated by %s",UpdatedNumber);
//					UBYTE len = strlen(strCheck);
//					strCheck[len] = CARRIAGE_RETURN;
//					strCheck[len+1] = MSGLASTWORD;
//					strCheck[len+2] = '\0';
//					DtmfMessageHandlerState = SendOwnerMessage;
//				}
//				else
//				{
//					DtmfMessageHandlerState = IdleState;
////					SIMCOM_Dial_Request = SMC_DisConnectCalls;
//				}
//			}
//			break;
//			case SendNumberMessage:
//				memset(strCheck,'\0',100);
//			if(SendMSG_State == MSG_Idle)
//			{
//				for(UBYTE i=0;i<13;i++)
//				{
//					MobNumber[i] = DTMFBuffer[i];
//				}
//				sprintf(strCheck,"N1 - %s\nN2 - %s\nN3 - %s\nN4 - %s",StoredMNs[0].MobNo,StoredMNs[1].MobNo,StoredMNs[2].MobNo,StoredMNs[3].MobNo);
//				UBYTE len = strlen(strCheck);
//				strCheck[len] = CARRIAGE_RETURN;
//				strCheck[len+1] = MSGLASTWORD;
//				strCheck[len+2] = '\0';
//				SendMessage(strCheck);
//				DtmfMessageHandlerState = IdleState;
//				MsgUpdationCompleteFlag = TRUE;
//			}
//			break;
//			case SendMotorStatus:
//			{
//				UBYTE Data;	
//				Data = FlashDataRead(EEPROMAdressByte);
//				for(UBYTE i=0;i<13;i++)
//				{
//					MobNumber[i] = readD.MobNo[i];
//				}
//				EEPROMAdressByte = EEPROMAdressByte+16;
//				if(Data == 1)
//				{
//					SendMessage(strCheck);
//					DtmfMessageHandlerState = TextMessageProcessing;					
//				}
//				if(EEPROMAdressByte == 0x08007040)
//				{
//					DtmfMessageHandlerState = IdleState;
////					SIMCOM_Dial_Request = SMC_DisConnectCalls;
//					SendMSG_State = MSG_Idle;
//					EEPROMAdressByte = 0x08007000;
//					MsgUpdationCompleteFlag = TRUE;
//				}
//			}
//			break;
//			case AWSCmdStatusUpdate:
//			{
//				memset(strCheck,'\0',100);
//				if(CurrentMotorStatus == 1)
//				{
//					sprintf(strCheck,"%s","motorstate on updated by aws");
//					strCheck[28] = CARRIAGE_RETURN;
//					strCheck[29] = MSGLASTWORD;
//					strCheck[30] = '\0';
//					DtmfMessageHandlerState = SendOwnerMessage;
//				}
//				else
//				{
//					sprintf(strCheck,"%s","motorstate off updated by aws");
//					strCheck[29] = CARRIAGE_RETURN;
//					strCheck[30] = MSGLASTWORD;
//					strCheck[31] = '\0';
//					DtmfMessageHandlerState = SendOwnerMessage;
//				}
//				
//			}
//			break;
//			case UpdateMotorStatusMsg:
//			{
//				strCheck[0] = 'M';	
//				strCheck[1] = '1';
//				strCheck[2] = ' ';
//				if(CurrentMotorStatus == 1)
//				{
//					strCheck[3] = 'o';	
//					strCheck[4] = 'n';
//					strCheck[5] = ' ';
//				}
//				else
//				{
//					strCheck[3] = 'o';	
//					strCheck[4] = 'f';
//					strCheck[5] = 'f';				
//				}
//				UBYTE j = 0;
//				strCheck[6] = 'M';	
//				strCheck[7] = 'B';
//				strCheck[8] = 'N';
//				strCheck[9] = 'O';
//				strCheck[10] = ' ';
//				strCheck[11] = '-';
//				strCheck[12] = ' ';
//				for(UBYTE i =0,j=13;j<26;i++,j++)
//				{
//					strCheck[j] = DTMFBuffer[i];
//				}
//				strCheck[26] = CARRIAGE_RETURN;
//				strCheck[27] = MSGLASTWORD;
//				strCheck[28] = '\0';
//				DtmfMessageHandlerState = SendOwnerMessage;
//			}
//			break;
//			case EmergencyMessage:
//			{
//				memset(strCheck,'\0',100);
//				strcpy(strCheck,"Emergency Message");
//				UBYTE len = strlen(strCheck);
//				strCheck[len] = CARRIAGE_RETURN;
//				strCheck[len+1] = MSGLASTWORD;
//				strCheck[len+2] = '\0';
//				SendMessage(strCheck);
//				DtmfMessageHandlerState = IdleState;
//			}
//			break;
//			case SendMultipleMessage:
//			{

//			}
//			break;
//			case SendOwnerMessage:
//			{
//				if(OwnerMessageCount == 0)
//				{
//					for(UBYTE i=0;i<13;i++)
//					{
//						MobNumber[i] = OWNER1[i];
//					}
//				}
//				else if(OwnerMessageCount == 1)
//				{
//					for(UBYTE i=0;i<13;i++)
//					{
//						MobNumber[i] = OWNER2[i];
//					}
//				}
//				OwnerMessageCount++;
//				SendMessage(strCheck);
//				DtmfMessageHandlerState = TextMessageProcessing;
//			}
//			break;
			case SendMultipleMessage:
			{
				UBYTE Data;	
				Data = FlashDataRead(EEPROMAdressByte);
				for(UBYTE i=0;i<13;i++)
				{
					MobNumber[i] = readD.MobNo[i];
				}
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