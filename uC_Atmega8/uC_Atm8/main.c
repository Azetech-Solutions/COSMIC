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
#include TR_MSGS_H
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
	AVR_IO_Control_ST *InputIO_Data = &IO_cmdData_AVR;
	
	while(1)
	{
		IOControls();
		
		if(DTMFCommandData.DTMF_Data == 49)
		{
			//Send_Message(0,"Hello Azetech");
			DTMFCommandData.DTMF_Data = 0;
			InputIO_Data->IO1 = TRUE;
		}
		
		if(DTMFCommandData.DTMF_Data == 50)
		{
			//Send_Message(0,"Hello Azetech");
			DTMFCommandData.DTMF_Data = 0;
			InputIO_Data->IO1 = FALSE;
		}
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


