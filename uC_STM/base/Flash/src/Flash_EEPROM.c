#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include UART_DRIVER_H
#include PLATFORM_TYPES_H
#include FLASH_EEPROM_H
#define FLASH_KEY1                      0x45670123U   /*!< Flash key1 */
#define FLASH_KEY2                      0xCDEF89ABU   /*!< Flash key2: used with FLASH_KEY1*/


St readD;
extern void SIM_Send_Data(unsigned char Data);
void FLASH_Lock(void);

void FLASH_Unlock(void)
{
  if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00U)
  {
    /* Authorize the FLASH Registers access */
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);

    /* verify Flash is unlock */
    if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00U)
    {
			//TODO lock error
    }
  }
}

void Flash_Erase(uint32_t PGN)
{	
		while(FLASH->SR & FLASH_SR_BSY1);//check busy	
	
		FLASH->SR = 0x00000000;
	
		FLASH->CR &= ~FLASH_CR_MER1;
		
		FLASH->CR &= ~FLASH_CR_PNB;

		FLASH->CR |= (PGN << FLASH_CR_PNB_Pos); // Page select
	
		FLASH->CR |= FLASH_CR_PER;  //Page erase bit
	
		FLASH->CR |= FLASH_CR_STRT ;//STRT: Start erase
	
		while(FLASH->SR & FLASH_SR_BSY1);//check busy	
		
		FLASH->CR &= ~FLASH_CR_PER;  //Page erase bit
}

static void FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data)
{
  /* Set PG bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);
	
  /* Program first word */
  *(uint32_t *)Address = (uint32_t)Data;

  /* Barrier to ensure programming is performed in 2 steps, in right order
    (independently of compiler optimization behavior) */
  __ISB();

  /* Program second word */
  *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);
	
	    /* If the program operation is completed, disable the PG or FSTPG Bit */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
}



void FLASH_Lock(void)
{

  /* Set the LOCK Bit to lock the FLASH Registers access */
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);

  /* verify Flash is locked */
  if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00u)
  {
    //TODO: 
  }

}

void EEPROMmain(uint32_t Address, uint64_t Data)
{
	FLASH_Unlock();
	FLASH_Program_DoubleWord(Address,Data);
	FLASH_Lock();
	
}


void EEPROMErasePage(uint32_t page)
{
	FLASH_Unlock();
	Flash_Erase(14);
	FLASH_Lock();
}

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

