#include "stm32g0xx.h"                  // Device header
#include "Includes.h" // Will have all definitions of the Project Headers
#include PLATFORM_TYPES_H // For Data Types
#include SIMCOM_H
#include COMIF_CONFIG_H
#include GPIO_DRIVER_H
#include UART_DRIVER_H
#include FLASH_EEPROM_H
#include SIMCOM_MESSAGE_H
#include MESSAGE_APP_H
#include DTMF_APP_H
#include MQTT_APPLICATION_H

char DTMFStoreNumber[13];

UBYTE BufferLength = 3;

char UpdatedNumber[13];

extern void EepromDeleteWrite(uint32_t number,UBYTE WrtInd[]);

extern void EEPROMmain(uint32_t Address, uint64_t Data);

extern void EEPROMErasePage(uint32_t page);

extern UBYTE DTMFBuffer[15];

extern void EepromFlashMmeoryCopy();

extern void updateSendData(UBYTE Data[]);

extern void DTMFMesaageHanldeFunc(char DtmfCmd);

extern UBYTE FlashDataRead(uint32_t Address);

uint32_t EEPROMWriteAdress;

UBYTE DTMFMessageFlag = FALSE;

void DTMFStateMachine(char DTMFMessage)
{
	switch(DtmfState)
	{
		AvrCmdData_ST StatusData;		
		case Idle:
			SIMCOM_State = SIMCOM_SM_Ready;
			if(DTMFMessage == '1')
			{
				StatusData.SW1 = 1;
				DTMFMessageFlag = TRUE;
				updateSendData(StatusData.Data_Bytes);	
			}
			else if(DTMFMessage == '2')
			{
				StatusData.SW1 = 0;
				DTMFMessageFlag = TRUE;
				updateSendData(StatusData.Data_Bytes);
			}
			else if(DTMFMessage == '3')
			{
				DtmfMessageHandlerState = UpdateMotorStatusMsg;
				DTMFMessageFlag = TRUE;
			}
			else if(DTMFMessage == '4')
			{
				DtmfMessageHandlerState = SendNumberMessage;
				DtmfState = Idle;
			}
			else if(DTMFMessage == '5')
			{
				DtmfState = ChooseAdressToAlterNumber;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;				
			}
			break;
		case ChooseAdressToAlterNumber:
			if(DTMFMessage == '1')
			{
				EEPROMWriteAdress = number1;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '2')
			{
				EEPROMWriteAdress = number2;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '3')
			{
				EEPROMWriteAdress = number3;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '4')
			{
				EEPROMWriteAdress = number4;
				DtmfState = ChooseTaskToAlter;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
		case ChooseTaskToAlter:
			for(UBYTE i=0;i<13;i++)
			{
				UpdatedNumber[i] = DTMFBuffer[i];
			}
			if(DTMFMessage == '*')
			{
				DtmfState = AddNumberToStore;
			}
			else if(DTMFMessage == '#')
			{
				DtmfState = DeleteExcistingNumber;
			}
			else if(DTMFMessage == '5')
			{
				EepromFlashMmeoryCopy();
				DtmfMessageHandlerState = NumberUpdateMessage;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
			
		case AddNumberToStore:
		{
			Data.MobNo[0] = '+';
			Data.MobNo[1] = '9';
			Data.MobNo[2] = '1';
			Data.MobNo[BufferLength] = DTMFMessage;
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
				SIMCOM_State = SIMCOMCancelCall;
			}
		}	
			break;		
		case DeleteExcistingNumber:
			if(DTMFMessage == '*')
			{
				EEPROMErasePage(14);
				DtmfState = Idle;
				EepromFlashMmeoryCopy();
				SIMCOM_State = SIMCOMCancelCall;
			}
			else
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
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
		default :
			break;
	}
}