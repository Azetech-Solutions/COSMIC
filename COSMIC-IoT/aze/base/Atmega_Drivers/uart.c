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
#define F_CPU 4000000UL			/* Define frequency here its 8MHz */

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
	UBRRH = 0;//(BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = 12;//BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

	UCSRB = (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1<<USBS) | (1 << UCSZ1); // Use 8-bit character sizes
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
	UCSRA |= (1<<U2X);
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
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

void UART_Tx(unsigned char Data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = Data;

}

void UART_String(char *data)
{
	while(*data)
	{
		UART_Tx(*(data++));
	}
}

unsigned char uart_rx()
{
	while ( !(UCSRA & (1<<RXC)) );
	unsigned char a=UDR;
	return a;
}



ISR(USART_RXC_vect)
{
	
	unsigned char d = UDR;
	SIMCOM_Data_Read(d);

}