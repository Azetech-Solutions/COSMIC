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
#include "Includes.h"
#include TR_MSGS_H
#include COMIF_H

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

void IOControls()
{
	AVR_IO_Control_ST *InputIO_Data = &IO_cmdData_AVR;
	AVR_IO_Control_ST *IO_StatusData = &AVR_IO_Status;
	
	UBYTE DataLen = ComIf_GetLength_STM32_IO_cmdData_AVR();
	
	if(InputIO_Data->IO1 == TRUE)
	{
		PORTC |= (1<<5);
	}
	else
	{
		PORTC &= ~(1<<5);
	}
	
	for(UBYTE i=0;i<DataLen;i++)
	{
		InputIO_Data->Bytes[i] = IO_StatusData->Bytes[i];
	}
	 
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




