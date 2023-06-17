/*
 * timer.c
 *
 * Created: 28-08-2018 20:02:21
 *  Author: MDRP
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Includes.h"
#include TIMER_H
#include <stdio.h>
#include LCD_H
extern void DebugString(char *data);
#include UART_H


int a = 0;
void timer_init()// initialize timer, interrupt and variable
{
	TCCR0 |= (1 << CS02);//set up timer with prescaler = 256
	//TCCR0 |= ((1 << CS01)|(1<<CS00));// set up timer with prescaler = 64

	//TCCR0 |= (1 << CS01);//set up timer with prescaler = 8
	TCNT0 = 5;// initialize counter
	TIMSK |= (1 << TOIE0);// enable overflow interrupt
}

