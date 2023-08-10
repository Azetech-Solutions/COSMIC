#ifndef _EEPROMWRAPPER_H_
#define _EEPROMWRAPPER_H_

//#define number1 0x08007000
//#define number2 0x08007010
//#define number3 0x08007020
//#define number4 0x08007030

typedef union
{
	struct
	{
		char MobNo[13];
		uint8_t WriteIndicator;
	};
	uint64_t byte[2];
}Mobile_Numbers_ST;

extern Mobile_Numbers_ST Data;

extern Mobile_Numbers_ST readD;

extern Mobile_Numbers_ST StoredMNs[6];

extern void EEPROMErasePage(uint32_t page);

extern void EepromDeleteWrite(uint32_t number,UBYTE WrtInd[]);

extern void EepromFlashMmeoryCopy();

extern UBYTE FlashDataRead(uint32_t Address);

#endif
