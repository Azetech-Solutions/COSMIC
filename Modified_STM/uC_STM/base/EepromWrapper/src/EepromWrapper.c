/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "Includes.h"
#include PLATFORM_TYPES_H
#include FLASH_EEPROM_H
#include EEPROMWRAPPER_H
#include DTMF_APP_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/*****************************************/
/* Global Variables                      */
/*****************************************/

Mobile_Numbers_ST Data;
Mobile_Numbers_ST readD;
Mobile_Numbers_ST StoredMNs[6];
UBYTE NumberData = 0;

/***************************************************/
/* Function Declarations                           */
/***************************************************/


/***************************************************/
/* Function Definitions                            */
/***************************************************/


UBYTE FlashDataRead(uint32_t Address)
{
	readD.byte[0] = *(uint64_t *)Address;
	
	readD.byte[1] = *(uint64_t *)(Address + 8U);
	
	return (UBYTE)readD.WriteIndicator;
}



void EepromFlashMmeoryCopy()
{
	uint32_t NumberArray[6] = {0x08007000,0x08007010,0x08007020,0x08007030,0x08007040,0x08007050};
	UBYTE dummyRead;
	for(UBYTE i =0;i<6; i++)
	{
		dummyRead = FlashDataRead(NumberArray[i]);
		if(dummyRead == 1)
		{
			 StoredMNs[i] = readD;
		}
	}
}

void EepromDeleteWrite(uint32_t number,UBYTE WrtInd[])
{
	EEPROMErasePage(14);
	uint32_t EEPROMWriteAdress = 0x08007000;
	for(UBYTE i=0;i<6;i++)
	{
		if((EEPROMWriteAdress == number) || (WrtInd[i] != 1))
		{
			EEPROMWriteAdress = EEPROMWriteAdress+16;
			continue;
		}
		EEPROMmain(EEPROMWriteAdress,StoredMNs[i].byte[0]);
		EEPROMWriteAdress = EEPROMWriteAdress+8;
		EEPROMmain(EEPROMWriteAdress,StoredMNs[i].byte[1]);
		EEPROMWriteAdress = EEPROMWriteAdress+8;
	}
}			


void EEPROMErasePage(uint32_t page)
{
	FLASH_Unlock();
	Flash_Erase(14);
	FLASH_Lock();
}


