/*
 * MQTT_Apllication.h
 *
 * Created: 27-12-2022 15:39:05
 *  Author: Admin
 */ 


#ifndef MQTT_APLLICATION_H_
#define MQTT_APLLICATION_H_

#include "Includes.h"

#include SIMCOM_MQTT_PUBLISH_H
#include PLATFORM_TYPES_H

#define ISPublishMsgConfigured()	(MQTTApp_State == MQTTApp_PublishMsgConfigured)

typedef enum
{
	MQTTApp_Init=0,
	MQTTApp_Idle,
	MQTTAppCheckLedStatus,
	MQTTApp_waitForResponse,
	MQTTApp_ComifRxindication,
	MQTTApp_SubMessageHandling,
	MQTTApp_ControlCommand,
	MQTTApp_Publish_IO_state,
	MQTTApp_PublishMsgConfiguringinprocess,
	MQTTApp_PublishMsgConfigured
		
}MQTTApp_States;

typedef enum{
	IdleMode = 0,
	MQTTMode,
	DTMFMode
}SimcomWorkingMode_ST;

typedef union
{
	struct
	{
		UBYTE SW1:1;
		//Byte 1
		UBYTE SW2:1;
		//Byte 2
		UBYTE SW3:1;
		//Byte 3
		UBYTE SW4:1;
		//Byte 4
		UBYTE SW5:1;
		//Byte 5
		UBYTE SW6:1;
		//Byte 6
		UBYTE SW7:1;
		//Byte 7
		UBYTE SW8:1;
		//Byte 8
		UBYTE SW9:1;
		//Byte 9
		UBYTE SW10:1;
		//Byte 10
		UBYTE SW11:1;
		//Byte 11
		UBYTE SW12:1;
		//Byte 12
		UBYTE SW13:1;
		//Byte 13
		UBYTE SW14:1;
		//Byte 14
		UBYTE SW15:1;
		//Byte 15
		UBYTE SW16:1;
		//Byte 16
	};
	UBYTE Data_Bytes[2];
}AvrCmdStatusData_ST;

extern SimcomWorkingMode_ST SimcomWorkingMode;

extern UBYTE MachineInitFlag;

extern BOOL MQTTConnectionCheckStatus;

extern MQTTApp_States MQTTApp_State;

extern char TOPIC1_SubscribeMsg[26];

extern void MQTT_AppMain();

extern UBYTE Cloud_Transmit(UWORD Length, void * Data);

#endif /* MQTT_APLLICATION_H_ */
