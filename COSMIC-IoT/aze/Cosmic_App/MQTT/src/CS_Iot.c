/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include MQTT_SSL_H
#include MQTT_PUBLISH_H
#include MQTT_H
#include <avr/io.h>
#include "stdio.h"

void Avr_Init()
{
	DDRA  = 0XFF;
	DDRB  = 0xFF;
	PORTA = 0X00;
	PORTB = 0xFF;
}
void COSMIC_SIMCOM_Error_Callback()
{
	SIMCOM_State = SIMCOM_SM_Init;
	C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;
	MQTT_State = MQTTSTART;
	Publish_State = MQTT_Publish_Idle;
}
