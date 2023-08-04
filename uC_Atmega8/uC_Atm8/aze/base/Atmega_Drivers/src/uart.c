/*
 * uart.c
 *
 * Created: 05-10-2018 22:47:04
 *  Author: AZETECH
 */ 
/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include "uart.h"
#include COMIF_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/
#define F_CPU 8000000UL
/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/
BOOL IsUartCommunicationOkay = FALSE;
/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

void uart_init()
{
	
	UCSRB = (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1<<USBS) | (1 << UCSZ1); // Use 8-bit character sizes

	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

/*	UCSRA |= (1<<U2X);*/
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	IsUartCommunicationOkay = TRUE;// To indicate The uart is initialized in proper way
}

unsigned char uart_tx(unsigned char a)
{
	unsigned char retval = 0;
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = a;
	retval = 1;
	
	return retval;
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
	ComIf_RxIndication_STM32(d);
}
