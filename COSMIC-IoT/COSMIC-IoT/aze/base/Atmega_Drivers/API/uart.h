/*
 * uart.h
 *
 * Created: 05-10-2018 22:47:16
 *  Author: MDRP
 */ 


#ifndef UART_H_
#define UART_H_

#include "operations.h"

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


extern void UART_init(long USART_BAUDRATE);
extern void UART_TxChar(char ch);
#if 0

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

extern void uart_init(void);
extern void uart_tx(unsigned char a);
extern void uart_string(unsigned char *str);
extern void num_3dig(int aa);
extern void num_point(int aa);
extern unsigned char uart_rx(void);
extern void msg_clr(void);
extern unsigned char wait_for_dtmf(void);
extern void user(void);
extern void status(void);
extern void check(void);
extern void saved_setting(void);
extern void amps_msg_send(void);
extern void gsm_send(unsigned char *msg);
extern void send_dryL_msg();
extern void send_Volt_msg();
void msg_cmp(void);
extern unsigned char waiting_dtmf();
extern void SIM_Send_Data(unsigned char Data);
#endif

#endif /* UART_H_ */