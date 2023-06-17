/*
 * Atmega8Interface.c
 *
 * Created: 25-05-2023 19:00:17
 * Author : Admin
 */ 

#include <avr/io.h>
#include "Includes.h"
#include LCD_H
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
extern void Avr_Init(void);
extern void TIM1_Init(void);
extern void uart_init(void);
extern void lcd_init(void);


// extern void lcd_cmd(unsigned char a);
// extern void lcd_string(unsigned char *a);
/*extern void DebugString(char *data);*/

void Avr_Init()
{
	DDRB |= (1<<0);
	DDRD |= (1<<7);
	DDRB |= (1<<5);
	DDRD |= (1<<4);
	DDRC |= (1<<5);
}
void Atmega32DriverInit()
{
	uart_init();
	Avr_Init();
	lcd_init();
	TIM1_Init();
}


int main(void)
{

	sei();
	Atmega32DriverInit();
	while(1)
	{
		/*SysOs_Main();*/
	}
}




