/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "stm32g0xx.h"                  // Device header
#include "Includes.h" // Will have all definitions of the Project Headers
#include <stdio.h>
#include PLATFORM_TYPES_H // For Data Types
#include SIMCOM_H
#include COMIF_H
#include COMIF_CONFIG_H
#include GPIO_DRIVER_H
#include UART_DRIVER_H
#include FLASH_EEPROM_H
#include SIMCOM_MESSAGE_H
#include MESSAGE_APP_H
#include DTMF_APP_H
#include MQTT_APPLICATION_H
#include SIMCOM_CALLS_H
#include AVR_H
#include EEPROMWRAPPER_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

DtmfNumberAlterStatus_En DtmfState = Idle;

char DTMFStoreNumber[13];

UBYTE BufferLength = 3;

volatile UBYTE MNID;

char DTMF_Data;

char UpdatedNumber[13];

uint32_t EEPROMWriteAdress;

uint32_t EEPROMMnNoAdress[6] = {0x08007000,0x08007010,0x08007030,0x08007040,0x08007050};

UBYTE DTMFMessageFlag = FALSE;

DTMF_Command_Data_ST DTMF_Command_Data;

UBYTE DTMFCallOnProcess = FALSE;

char DTMFNumberString[3];

UBYTE DTMFNumberindex = 0;

UBYTE SendMbNoMsg = FALSE;
	
/***************************************************/
/* Function Declarations                           */
/***************************************************/

void updateDtmfNumber(char msg);

void UpdateMobileNumbersToSend();

void ClearDtmfNumberString();

void DTMFMessageUpdation();

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void DTMFStateMachine()
{
	if(DTMFMessageFlag == TRUE)
	{
		static char PrevDtmfData;
		DTMFMessageFlag = FALSE;
		switch(DtmfState)
		{
			case Idle:
			{
				if(DTMF_Data == '*')
				{
					DtmfState = ChooseAdressToAlterNumber;
				}
				else if(DTMF_Data == '#')
				{
					DtmfState = ChooseAdressToAlterNumber;
				}
				else
				{
					updateDtmfNumber(DTMF_Data);
				}
			}
			break;
			case ChooseAdressToAlterNumber:
			{
				if(DTMF_Data == '0')
				{
					SendMbNoMsg = TRUE;
					AVR_SendData('Y');
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
					DtmfState = Idle;
				}
				else if(DTMF_Data == '1')
				{
					EEPROMWriteAdress = number1;
				}
				else if(DTMF_Data == '2')
				{
					EEPROMWriteAdress = number2;
				}
				else if(DTMF_Data == '3')
				{
					EEPROMWriteAdress = number3;
				}
				else if(DTMF_Data == '4')
				{
					EEPROMWriteAdress = number4;
				}
				else if(DTMF_Data == '5')
				{
					EEPROMWriteAdress = number5;
				}
				else if(DTMF_Data == '6')
				{
					EEPROMWriteAdress = number6;
				}
				else
				{
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
					DtmfState = Idle;
				}
				if(PrevDtmfData == '*' && DtmfState == ChooseAdressToAlterNumber)
				{
					DtmfState = AddNumberToStore;
				}
				else if(PrevDtmfData == '#' && DtmfState == ChooseAdressToAlterNumber)
				{
					DtmfState = DeleteExcistingNumber;
					DTMFMessageFlag = TRUE;
				}
				else
				{
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
					DtmfState = Idle;
				}
			}
			break;
			case AddNumberToStore:
			{
				Data.MobNo[0] = '+';
				Data.MobNo[1] = '9';
				Data.MobNo[2] = '1';
				Data.MobNo[BufferLength] = DTMF_Data;
				BufferLength++;
				if(BufferLength == 13)
				{
					Data.WriteIndicator = 1;
					BufferLength = 3;
					DtmfState = Idle;
					EEPROMmain(EEPROMWriteAdress,Data.byte[0]);
					EEPROMWriteAdress = EEPROMWriteAdress+8;
					EEPROMmain(EEPROMWriteAdress,Data.byte[1]);
					EepromFlashMmeoryCopy();
					DTMF_Data = '*';
					DtmfState = NumberUpdationCompleted;
					DTMFMessageFlag = TRUE;
				}
			}	
			break;		
			case DeleteExcistingNumber:
			{
				UBYTE WrtInd[6];
				uint32_t EEPROMAdress = 0x08007000;
				for(UBYTE i = 0;i < 6; i++)
				{
					WrtInd[i] = (FlashDataRead(EEPROMAdress));
					EEPROMAdress = EEPROMAdress+16;
				}
				EepromDeleteWrite(EEPROMWriteAdress,WrtInd);
				EepromFlashMmeoryCopy();
				DTMF_Data = '#';
				DtmfState = NumberUpdationCompleted;
				DTMFMessageFlag = TRUE;
			}
			break;
			case NumberUpdationCompleted:
			{
				DTMFMessageUpdation();
				DtmfState = Idle;
				SIMCOM_Dial_Request = SMC_DisConnectCalls;
			}
			break;
			default:
			break;
		}
		PrevDtmfData = DTMF_Data;
	}
}


void DTMFMessageUpdation()
{
	DTMF_Command_Data.Bytes[0] = MNID+48;
	
	DTMF_Command_Data.Bytes[1] = DTMF_Data;
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_AVR_DTMFCommandData();
	
	memcpy(PubMsg,DTMF_Command_Data.Bytes,2);
	
	ComIf_TransmitFromBuffer_AVR_DTMFCommandData();	
	
	ClearDtmfNumberString();
}


UBYTE CheckReadyForDtmf()
{	
	return MachineInitFlag == FALSE;
}


void DTMFCallMessageCbk(UBYTE Length, UBYTE *Data)
{
	
}

void DTMFMessageRxCbk(UBYTE Length, UBYTE *Data)
{
	
}

void updateDtmfNumber(const char msg)
{
	DTMFNumberString[DTMFNumberindex] = msg;
	DTMFNumberindex++;
	if(DTMFNumberindex == 3)
	{
		SIMCOM_Dial_Request = SMC_DisConnectCalls;
	}
}

void ClearDtmfNumberString()
{
	DTMFNumberindex = 0;
	memset(DTMFNumberString,'\0',3);
}



void UpdateMobileNumbersToSend()
{
	char arr[100];
	sprintf(arr,"N1 - %s\nN2 - %s\nN3 - %s\nN4 - %s\nN5 - %s\nN6 - %s",
	&StoredMNs[0].MobNo[3],&StoredMNs[1].MobNo[3],
	&StoredMNs[2].MobNo[3],&StoredMNs[3].MobNo[3],
	&StoredMNs[4].MobNo[3],&StoredMNs[5].MobNo[3]);
	Send_TextMessage(arr,MNID);
}



