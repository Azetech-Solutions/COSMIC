/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include <avr/io.h>
#include LCD_H
#include "stdio.h"

void Avr_Init()
{
		DDRA  = 0XFF;
		DDRB  = 0xFF;
		PORTA = 0XFF;
		PORTB = 0xFF;
}
