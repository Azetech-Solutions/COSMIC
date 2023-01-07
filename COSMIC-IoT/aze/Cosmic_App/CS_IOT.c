/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include <avr/io.h>
#define F_CPU 4000000
#include <avr/delay.h>
#include MQTT_SSL_H


/*****************************************/
/* Global Variables                      */
/*****************************************/


static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

void Avr_Init()
{
		DDRA  = 0XFF;
		DDRB  = 0xFF;
		PORTA = 0XFF;
		PORTB = 0xFF;
		
		while(IsSSLCertConfigured == FALSE)
		{
					
			SIMCOM_MainFunction();
			_delay_ms(100);
		}

}

void Avr_Main_x10()
{
	
	
}