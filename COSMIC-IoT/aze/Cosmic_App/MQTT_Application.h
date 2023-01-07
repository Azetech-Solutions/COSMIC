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

#define PublishString "{\"cmd\":%d\"data\"{\"m1\":%d}}"

typedef enum
{
	MQTTApp_Init=0,
	MQTTApp_Idle,
	MQTTApp_StoreSubMessage,
	MQTTApp_SubMessageHandling,
	MQTTApp_ReplyMsg,
	MQTTApp_UpdateBeforeIdle
}MQTTApp_States;

extern MQTTApp_States MQTTApp_State;

extern BOOL IsSubscribeMsgRecieved;

extern char SubscribeDataBuff[30];

UBYTE MQTT_StringSeperate(char *str,char endpoint);

extern void MQTT_AppMain();

#endif /* MQTT_APLLICATION_H_ */