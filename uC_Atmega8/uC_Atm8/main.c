/*
 * Atmega8Interface.c
 *
 * Created: 25-05-2023 19:00:17
 * Author : Admin
 */ 
/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Includes.h"
#include UART_H
#include COSMIC_APP_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/


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
}


int main(void)
{
	sei();
	Avr_Init();
	Atmega32DriverInit();
	
	while(1)
	{
		IOControls();
	}
}

// CALL CHECK
// int main(void)
// {
// 	sei();
// 	Avr_Init();
// 	Atmega32DriverInit();
// 	_delay_ms(20000);
// 	Call_Request(1,0);
// }


