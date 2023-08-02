
/*
 * uart.h
 *
 * Created: 05-10-2018 22:47:16
 *  Author: MDRP
 */ 
#ifndef UART_H_
#define UART_H_
/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "Includes.h"
#include PLATFORM_TYPES_H
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/
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


/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

extern void uart_init();
extern unsigned char uart_tx(unsigned char a);
extern void uart_string(unsigned char *str);
extern unsigned char uart_rx();

extern BOOL IsUartCommunicationOkay;

#endif /* UART_H_ */