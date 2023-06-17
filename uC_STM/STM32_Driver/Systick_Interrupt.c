#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include GPIO_DRIVER_H
#include SYSOS_H

void SystickTimerInit(void);

void SysTick_Handler(void)
{
	FUN_SYS_Timer_x1();
}

void SystickTimerInit(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/9);// This Tick Timer is Configured for 10ms Interrupt
  NVIC_EnableIRQ (SysTick_IRQn);
}
//FlashDataRead(0X08007000);