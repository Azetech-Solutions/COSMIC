/*
 * uart.h
 *
 * Created: 05-10-2018 22:47:16
 *  Author: MDRP
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

extern void uart_init(void);

extern unsigned char uart_rx(void);

extern void SIM_Send_Data(unsigned char Data);

extern void UART_String(char *data);

extern void UART_Tx(unsigned char Data);

#endif /* UART_H_ */