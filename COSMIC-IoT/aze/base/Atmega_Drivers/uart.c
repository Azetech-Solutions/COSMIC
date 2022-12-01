/*
 * File:   uart.c
 * Author: Azetech
 *
 * Created on 4 November, 2022, 1:28 PM
 */

/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include <avr/io.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Includes.h"
#include SYSOS_H
#include SIMCOM_H
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/
#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
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

void UART_init()
{
	UCSRB |= (1 << RXEN) | (1 << RXCIE) | (1<<TXEN);/* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */

	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits */
}


void uart_tx(unsigned char a)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = a;
}

/*
 *This Function Is used to Send the Commands to GSM module.
 *This Function is Called in SIMCOM Schedule JOB.
 */

void SIM_Send_Data(unsigned char Data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = Data;

}

unsigned char uart_rx()
{
	while ( !(UCSRA & (1<<RXC)) );
	unsigned char a=UDR;
	return a;
}


void uart_string(unsigned char *str)
{
	while(*str!='\0')
	{
		uart_tx(*str++);
	}
}

ISR(USART_RXC_vect)
{
	
	unsigned char d = UDR;
	SIMCOM_Data_Read(d);

}