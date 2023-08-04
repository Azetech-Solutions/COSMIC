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
/*****************************************/
/* Global Variables                      */
/*****************************************/

UBYTE CurrentMotorStatus = 0;
extern char MobNumber[13];
extern char UpdatedNumber[13];
extern UBYTE DTMFBuffer[15];
UBYTE MsgUpdationCompleteFlag = FALSE;
uint32_t EEPROMAdressByte = 0x08007000;
UBYTE PreviousMotorStatus = 0;
extern UBYTE FlashDataRead(uint32_t Address);
char strCheck[100] = "Machine Ready";
DtmfMessageHandler_En DtmfMessageHandlerState = IdleState;

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void DtmfMessageCallFunc()
{
		switch(DtmfMessageHandlerState)
		{
			case IdleState:
				
				break;
			case MachineInit:
			{
				UBYTE len = strlen(strCheck);
				strCheck[len] = CARRIAGE_RETURN;
				strCheck[len+1] = MSGLASTWORD;
				strCheck[len+2] = '\0';
				SendMessage(strCheck);
				len = strlen(strCheck);
				memset(strCheck,'\0',20);
				DtmfMessageHandlerState = IdleState;
			}
				break;
			case NumberUpdateMessage:
			{
				if(SendMSG_State == MSG_Idle)
				{
					sprintf(strCheck,"N1 - %s\nN2 - %s\nN3 - %s\nN4 - %s\nNumber Updated by %s",AdressCpy[0].MobNo,AdressCpy[1].MobNo,AdressCpy[2].MobNo,AdressCpy[3].MobNo,UpdatedNumber);
					UBYTE len = strlen(strCheck);
					strCheck[len] = CARRIAGE_RETURN;
					strCheck[len+1] = MSGLASTWORD;
					strCheck[len+2] = '\0';
					SendMessage(strCheck);
					memset(strCheck,'\0',100);
				}				
			}
			break;
			case SendNumberMessage:
			if(SendMSG_State == MSG_Idle)
			{
				for(UBYTE i=0;i<13;i++)
				{
					MobNumber[i] = DTMFBuffer[i];
				}
				
				sprintf(strCheck,"N1 - %s\nN2 - %s\nN3 - %s\nN4 - %s",AdressCpy[0].MobNo,AdressCpy[1].MobNo,AdressCpy[2].MobNo,AdressCpy[3].MobNo);
				UBYTE len = strlen(strCheck);
				strCheck[len] = CARRIAGE_RETURN;
				strCheck[len+1] = MSGLASTWORD;
				strCheck[len+2] = '\0';
				SendMessage(strCheck);
				memset(strCheck,'\0',100);
			}
				break;
			case SendMotorStatus:
			{
				UBYTE Data;	
				
				Data = FlashDataRead(EEPROMAdressByte);
				
				for(UBYTE i=0;i<13;i++)
				{
					MobNumber[i] = readD.MobNo[i];
				}
				
				EEPROMAdressByte = EEPROMAdressByte+16;
				
				if(EEPROMAdressByte == 0x08007040)
				{
					EEPROMAdressByte = 0x08007000;
					MsgUpdationCompleteFlag = TRUE;
				}
				
				if(Data == 1)
				{
					SendMessage(strCheck);
					UBYTE len = strlen(strCheck);
					DtmfMessageHandlerState = IdleState;					
				}
			}
			break;
			case UpdateMotorStatusMsg:
			{
				strCheck[0] = 'M';	
				strCheck[1] = '1';
				strCheck[2] = ' ';
				
				if(CurrentMotorStatus == 1)
				{
					strCheck[3] = 'o';	
					strCheck[4] = 'n';
					strCheck[5] = ' ';
				}
				else
				{
					strCheck[3] = 'o';	
					strCheck[4] = 'f';
					strCheck[5] = 'f';				
				}
				
				UBYTE j = 0;
				strCheck[6] = 'M';	
				strCheck[7] = 'B';
				strCheck[8] = 'N';
				strCheck[9] = 'O';
				strCheck[10] = ' ';
				strCheck[11] = '-';
				strCheck[12] = ' ';
				
				for(UBYTE i =0,j=13;j<26;i++,j++)
				{
					strCheck[j] = DTMFBuffer[i];
				}
				
				UBYTE len = strlen(strCheck);
				strCheck[len] = CARRIAGE_RETURN;
				strCheck[len+1] = MSGLASTWORD;
				strCheck[len+2] = '\0';
				DtmfMessageHandlerState = SendMotorStatus;
			}
			break;
			case TextMessageProcessing:
				
				break;
			default:
				
				break;
		}
}
