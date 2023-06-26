#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include <stdio.h>
#include <string.h>
#include PLATFORM_TYPES_H
#include MESSAGEHANDLINGAPPLICATION_H
#include FLASH_EEPROM_H
#include MESSAGE_H
#include SIMCOM_H



char strCheck[100] = "Machine Ready";
DtmfMessageHandler_En DtmfMessageHandlerState = IdleState;

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
			case SendNumberMessage:
			if(SendMSG_State == MSG_Idle)
			{
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
				
				break;
			default:
				
				break;
		}
}
