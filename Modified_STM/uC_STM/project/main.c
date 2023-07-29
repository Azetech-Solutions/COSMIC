#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include SYSOS_H
#include UART_DRIVER_H
#include GPIO_DRIVER_H
#include FLASH_EEPROM_H

void Sysos_check();
void GPIO_init();
extern void SystickTimerInit();
extern void FlashDoubleWordWrite(UBYTE data[],uint32_t *Adress);

int main()
{
	SysOs_Main();
	while(1);
	return 0;
}
void GPIO_init()
{
	GPIO_InitTypeDef PA = {0};

  /* GPIO Ports Clock Enable */
	PORTA_CLOCK_ENABLE();

  /*Configure GPIO pin Output Level */
  GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  PA.Pin = GPIO_PIN_1|GPIO_PIN_0;
  PA.Mode = GPIO_MODE_OUTPUT_PP;
  PA.Pull = GPIO_NOPULL;
  PA.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Config(GPIOA, &PA);
}
void Sysos_check()
{
	GPIO_TogglePin(GPIOA,GPIO_PIN_1);
}