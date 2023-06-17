/*
 * uart.h
 *
 * Created: 05-10-2018 22:47:16
 *  Author: MDRP
 */ 


#ifndef UART_H_
#define UART_H_
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "Includes.h"
#include PLATFORM_TYPES_H

#define USART_BAUDRATE 115200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

void uart_init();
void uart_tx(unsigned char a);
void uart_string(unsigned char *str);
unsigned char uart_rx();

#endif /* UART_H_ */