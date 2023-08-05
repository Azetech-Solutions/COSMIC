
#ifndef _FLASH_H_
#define _FLASH_H_

#include<stdint.h>

#define number1 0x08007000
#define number2 0x08007010
#define number3 0x08007020
#define number4 0x08007030

extern void EEPROMErasePage(uint32_t page);

extern void EEPROMmain(uint32_t Address, uint64_t Data);

extern void Flash_Erase(uint32_t PGN);

extern void FLASH_Unlock(void);

extern void FLASH_Lock(void);

#endif  /* _FLASH_H_ */
