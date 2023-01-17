/*
 * MQTT_Apllication.c
 *
 * Created: 27-12-2022 15:38:18
 *  Author: Admin
 */ 
/***************************************************/
/* COSMIC Application Main Source File                          */
/***************************************************/

/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "Includes.h"
#include COMIF_CONFIG_H
#include SIMCOM_H
#include STRINGHELPER_H
#include MQTT_H
#include MQTT_SSL_H
#include MQTT_APPLICATION_H
#include MQTT_PUBLISH_H
#include <stdio.h>
#include <avr/io.h>
#include LCD_H
/***************************************************/
/* Global Variables Definitions                    */
/***************************************************/


CommandData_ST CommandData;

MQTTApp_States MQTTApp_State = MQTTApp_Init;

char TOPIC1_SubscribeMsg[30];

UBYTE MQTT_StringSeperate(char *str,char endpoint);

BOOL IsSubscribeMsgRecieved = FALSE;

extern void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data);

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void COSMIC_Generic_SIMCOM_Callback(SIMCOM_Job_Result_EN JobState)
{

	/* This function will be called for an un-scheduled job. So check for the response and clear the buffer */
	SIMCOM_ClearResponseBuffer();
}

void COSMIC_SIMCOM_Error_Callback()
{
	SIMCOM_State = SIMCOM_SM_Init;
	C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;
	MQTT_State = MQTTSTART;
	Publish_State = MQTT_Publish_Idle;
}

void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data)
{
			
		if (Data[0] == 255)
		{
			DebugStringRow2("255");
		}
		
		if (Data[1] == 1)
		{
			lcd_cmd(0xc8);
			lcd_string("Trun On");
		}
		else
		{
			lcd_cmd(0xc8);
			lcd_string("Trun Off");
		}
}

void Cloud_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1)
{
	
}

UBYTE Cloud_Transmit(UWORD Length, void * Data)
{
	memcpy(PublishPayload,Data,Length);
	
	return COMIF_EC_NO_ERROR;
}

void UpdatePublishdata(UBYTE Cmd)
{
	CommandData_ST *CD = &CommandData;
	
	CD->cmd = Cmd;
	
	if (PORTA0 & 0x1)
	{
		CD->IO_Ctrl = FALSE;
	}
	else
	{
		CD->IO_Ctrl = TRUE;
	}
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Cloud_CommandDataStatus();
	
	memcpy(PubMsg,CD->Data_Bytes,8);
	
	ComIf_TransmitFromBuffer_Cloud_CommandDataStatus();
	
}

void MQTT_AppMain()
{
	switch(MQTTApp_State)
	{
		case MQTTApp_Idle:
		{
			
		}
		break;
		case MQTTApp_Init:
		{
			if(IsMQTT_Ready())
			{
				UpdatePublishdata(255);
				Publish_State = MQTT_PublishTopic_Config;
				MQTTApp_State = MQTTApp_SubMessageHandling;
			}
		}
		break;
		case MQTTApp_ComifRxindication:
		{
			DebugStringRow1(TOPIC1_SubscribeMsg);
			
			ComIf_RxIndication_Cloud(TOPIC1_SubscribeMsg,strlen(TOPIC1_SubscribeMsg));
			
			memset(TOPIC1_SubscribeMsg,0,strlen(TOPIC1_SubscribeMsg));
			
			MQTTApp_State = MQTTApp_SubMessageHandling;
		}
		break;
		case MQTTApp_SubMessageHandling:
		{
			
		}
		break;
		case MQTTApp_UpdateBeforeIdle:
		{
			UpdatePublishdata(0);
			Publish_State = MQTT_PublishTopic_Config;
			MQTTApp_State = MQTTApp_Idle;
		}
		break;
		case MQTTApp_ReplyMsg:
		{
			UpdatePublishdata(1);
			Publish_State = MQTT_PublishTopic_Config;
			MQTTApp_State = MQTTApp_Idle;
		}
		break;
		default:
		{
			//do Nothing
			break;
		}
	}
	
}

