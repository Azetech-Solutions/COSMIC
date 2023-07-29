
#ifndef _FLASH_H_
#define _FLASH_H_

#include<stdint.h>


typedef union
{
	struct
	{
		char MobNo[13];
		uint8_t WriteIndicator;
	};
	uint64_t byte[2];
}St;

#define number1 0x08007000
#define number2 0x08007010
#define number3 0x08007020
#define number4 0x08007030

extern St Data;
extern St readD;
extern St AdressCpy[4];

#endif  /* _FLASH_H_ */
