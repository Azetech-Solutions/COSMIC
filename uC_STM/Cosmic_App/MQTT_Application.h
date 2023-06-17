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

#define ISPublishMsgConfigured()	(MQTTApp_State == MQTTApp_PublishMsgConfigured)

typedef enum
{
	MQTTApp_Init=0,
	MQTTApp_Idle,
	MQTTApp_waitForResponse,
	MQTTApp_ComifRxindication,
	MQTTApp_SubMessageHandling,
	MQTTApp_ControlCommand,
	MQTTApp_Publish_IO_state,
	MQTTApp_PublishMsgConfiguringinprocess,
	MQTTApp_PublishMsgConfigured
		
}MQTTApp_States;

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
}StatusData_ST;

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
}AvrCmdData_ST;

typedef union
{
	struct
	{
		short int VS1:10;
		//Byte 1
		short int CS1:6;
		
		short int VS2:10;
		//Byte 3
		short int CS2:6;
		
		short int VS3:10;
		//Byte 5
		short int CS3:6;
		
		short int VS4:10;
		//Byte 7
		short int CS4:6;
		
		short int VS5:10;
		//Byte 9
		short int CS5:6;
		
		short int VS6:10;
		//Byte 11
		short int CS6:6;
		
		short int VS7:10;
		//Byte 13
		short int CS7:6;
		
		short int VS8:10;
		//Byte 15
		short int CS8:6;
		
		short int VS9:10;
		//Byte 17
		short int CS9:6;
		
		short int VS10:10;
		//Byte 19
		short int CS10:6;
		
		short int VS11:10;
		//Byte 21
		short int CS11:6;
		
		short int VS12:10;
		//Byte 23
		short int CS12:6;
				
		short int VS13:10;
		//Byte 25
		short int CS13:6;
		
		short int VS14:10;
		//Byte 27
		short int CS14:6;
			
		short int VS15:10;
		//Byte 29
		short int CS15:6;

		short int VS16:10;
		//Byte 31
		short int CS16:6;

	};
	UBYTE Data_Bytes[32];
}adcStatusData_ST;

typedef union
{
	struct
	{
		UBYTE StatusCmd;
		/*Byte - 0*/
		UBYTE reserved1: 8;
		/*Byte - 1*/
	};
	UBYTE Data_Bytes[2];
}AvrCmdSendData_ST;


extern BOOL MQTTConnectionCheckStatus;

extern MQTTApp_States MQTTApp_State;

extern char TOPIC1_SubscribeMsg[26];

extern void MQTT_AppMain();

extern UBYTE Cloud_Transmit(UWORD Length, void * Data);

#endif /* MQTT_APLLICATION_H_ */
