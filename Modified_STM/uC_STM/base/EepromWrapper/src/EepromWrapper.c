#include "Includes.h"
#include PLATFORM_TYPES_H
#include FLASH_EEPROM_H
#include EEPROMWRAPPER_H




Mobile_Numbers_ST Data;
Mobile_Numbers_ST readD;
Mobile_Numbers_ST StoredMNs[4];
UBYTE NumberData = 0;


UBYTE FlashDataRead(uint32_t Address)
{
	readD.byte[0] = *(uint64_t *)Address;
//	SIM_Send_Data(readD.MobNo[0]);
//	SIM_Send_Data(readD.MobNo[1]);
//	SIM_Send_Data(readD.MobNo[2]);
//	SIM_Send_Data(readD.MobNo[3]);
//	SIM_Send_Data(readD.MobNo[4]);
//	SIM_Send_Data(readD.MobNo[5]);
//	SIM_Send_Data(readD.MobNo[6]);
//	SIM_Send_Data(readD.MobNo[7]);
	
	readD.byte[1] = *(uint64_t *)(Address + 8U);
//	SIM_Send_Data(readD.MobNo[8]);
//	SIM_Send_Data(readD.MobNo[9]);
//	SIM_Send_Data(readD.MobNo[10]);
//	SIM_Send_Data(readD.MobNo[11]);
//	SIM_Send_Data(readD.MobNo[12]);
//	SIM_Send_Data(readD.WriteIndicator);
	return (UBYTE)readD.WriteIndicator;
}



void EepromFlashMmeoryCopy()
{
	uint32_t NumberArray[4] = {0x08007000,0x08007010,0x08007020,0x08007030};
	UBYTE dummyRead;
	for(UBYTE i =0;i<4; i++)
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
	for(UBYTE i=0;i<4;i++)
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