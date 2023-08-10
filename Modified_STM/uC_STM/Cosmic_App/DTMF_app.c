/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "stm32g0xx.h"                  // Device header
#include "Includes.h" // Will have all definitions of the Project Headers
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

volatile char DTMF_Data;

char UpdatedNumber[13];

uint32_t EEPROMWriteAdress;

uint32_t EEPROMMnNoAdress[6] = {0x08007000,0x08007010,0x08007030,0x08007040,0x08007050};

UBYTE DTMFMessageFlag = FALSE;

DTMF_Command_Data_ST DTMF_Command_Data;

/***************************************************/
/* Function Declarations                           */
/***************************************************/


void DTMFMessageUpdation();

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void DTMFStateMachine()
{
	if(DTMFMessageFlag == TRUE)
	{
		DTMFMessageFlag = FALSE;
		switch(DtmfState)
		{		
			case Idle:
			{
				if(DTMF_Data == '1')
				{
					DTMFMessageUpdation();
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
				}
				else if(DTMF_Data == '2')
				{
					DTMFMessageUpdation();
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
				}
				else if(DTMF_Data == '3')
				{
					DTMFMessageUpdation();
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
				}
				else if(DTMF_Data == '4')
				{
					DTMFMessageUpdation();
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
				}
				else if(DTMF_Data == '5')
				{
					DtmfState = ChooseAdressToAlterNumber;
				}
				else
				{
					SIMCOM_Dial_Request = SMC_DisConnectCalls;				
				}
				break;
			}
			case ChooseAdressToAlterNumber:
			{
				if(DTMF_Data == '1')
				{
					EEPROMWriteAdress = number1;
					DtmfState = ChooseTaskToAlter;
				}
				else if(DTMF_Data == '2')
				{
					EEPROMWriteAdress = number2;
					DtmfState = ChooseTaskToAlter;
				}
				else if(DTMF_Data == '3')
				{
					EEPROMWriteAdress = number3;
					DtmfState = ChooseTaskToAlter;
				}
				else if(DTMF_Data == '4')
				{
					EEPROMWriteAdress = number4;
					DtmfState = ChooseTaskToAlter;
				}
				else
				{
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
					DtmfState = Idle;
				}
				break;
			}
			case ChooseTaskToAlter:
			{
				for(UBYTE i=0;i<13;i++)
				{
					UpdatedNumber[i] = DTMFBuffer[i];
				}
				if(DTMF_Data == '*')
				{
					DtmfState = AddNumberToStore;
					DTMFMessageFlag = TRUE;
				}
				else if(DTMF_Data == '#')
				{
					DtmfState = DeleteExcistingNumber;
					DTMFMessageFlag = TRUE;
				}
				else
				{
					SIMCOM_Dial_Request = SMC_DisConnectCalls;
					DtmfState = Idle;
				}
				break;
			}	
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
				}
				DtmfState = NumberUpdationCompleted;
				DTMFMessageFlag = TRUE;
			}	
			break;		
			case DeleteExcistingNumber:
			{
				UBYTE WrtInd[4];
				uint32_t EEPROMAdress = 0x08007000;
				for(UBYTE i = 0;i < 4; i++)
				{
					WrtInd[i] = (FlashDataRead(EEPROMAdress));
					EEPROMAdress = EEPROMAdress+16;
				}
				EepromDeleteWrite(EEPROMWriteAdress,WrtInd);
				EepromFlashMmeoryCopy();
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
	}
}


void DTMFMessageUpdation()
{
	DTMF_Command_Data.Bytes[0] = MNID+48;
	
	DTMF_Command_Data.Bytes[1] = DTMF_Data;
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_AVR_DTMFCommandData();
	
	memcpy(PubMsg,DTMF_Command_Data.Bytes,2);
	
	ComIf_TransmitFromBuffer_AVR_DTMFCommandData();	
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
