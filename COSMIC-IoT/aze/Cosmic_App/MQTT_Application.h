/*
 * MQTT_Apllication.h
 *
 * Created: 27-12-2022 15:39:05
 *  Author: Admin
 */ 


#ifndef MQTT_APLLICATION_H_
#define MQTT_APLLICATION_H_

#include "Includes.h"

#include MQTT_PUBLISH_H
#include PLATFORM_TYPES_H


typedef enum
{
	MQTTApp_Init=0,
	MQTTApp_Idle,
	MQTTApp_ComifRxindication,
	MQTTApp_SubMessageHandling,
	MQTTApp_ReplyMsg,
	MQTTApp_UpdateBeforeIdle
}MQTTApp_States;


typedef union
{
	struct
	{
		UBYTE cmd:8;
		/*Byte - 0*/
		UBYTE IO_Ctrl:1;
		UBYTE reserved1: 7;
		/*Byte - 1*/
		ULONG reserved2:32;
		/*Bytes - 4*/
		UWORD reserved3:16;
		/*Bytes - 2*/
	};
	
	UBYTE Data_Bytes[8];
}CommandData_ST;

extern CommandData_ST CommandData;

extern MQTTApp_States MQTTApp_State;

extern BOOL IsSubscribeMsgRecieved;

extern char TOPIC1_SubscribeMsg[30];

UBYTE MQTT_StringSeperate(char *str,char endpoint);

extern void MQTT_AppMain();

extern UBYTE Cloud_Transmit(UWORD Length, void * Data);

#endif /* MQTT_APLLICATION_H_ */