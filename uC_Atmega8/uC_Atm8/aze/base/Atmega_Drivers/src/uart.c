/*
 * uart.c
 *
 * Created: 05-10-2018 22:47:04
 *  Author: MDRP
 */ 
#include "Includes.h"
#include "uart.h"
#define F_CPU 8000000UL
#include BUFFER_CONFIG_H

extern UBYTE ComIf_RxIndication_Cloud(UBYTE DataByte);

void uart_init()
{
	
	UCSRB = (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1<<USBS) | (1 << UCSZ1); // Use 8-bit character sizes

	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

/*	UCSRA |= (1<<U2X);*/
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void uart_tx(unsigned char a)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = a;
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
	ComIf_RxIndication_Cloud(d);
}
