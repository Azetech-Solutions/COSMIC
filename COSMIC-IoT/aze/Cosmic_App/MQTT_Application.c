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
#include MQTT_APPLICATION_H
#include <stdio.h>
#include <avr/io.h>
#include LCD_H
/***************************************************/
/* Global Variables Definitions                    */
/***************************************************/


CommandData_ST CommandData;


void COSMIC_Generic_SIMCOM_Callback(SIMCOM_Job_Result_EN JobState)
{

	/* This function will be called for an un-scheduled job. So check for the response and clear the buffer */
	SIMCOM_ClearResponseBuffer();
}

void COSMIC_SIMCOM_Error_Callback(SIMCOM_Error_State_EN Error)
{
	if(Error == SIMCOM_Error_GSM_Not_Connected)
	{
		DebugStringRow2("GSM_Not_Connected");
	}
}


MQTTApp_States MQTTApp_State = MQTTApp_Init;

char SubscribeDataBuff[30];

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

void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data)
{
	
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

UBYTE MQTT_StringSeperate(char *str,char endpoint)
{
	static char arr[5];
	int i,j,findlen,count = 0,temp;
	findlen = strlen(str);
	for(i = 0;i<SIMCOM_ResponseLength-findlen;i++)
	{
		j = 0;
		if(SubscribeDataBuff[i] == str[0] && SubscribeDataBuff[i+1] == str[1])
		{
			temp = i;
			while(j<findlen)
			{
				if(str[j] == SubscribeDataBuff[temp])
				{
					count++;
					if(count == findlen)
					{
						temp = temp+1;
						count = 0;
						for(i = temp;SubscribeDataBuff[i]!=endpoint;i++)
						{
							arr[count] = SubscribeDataBuff[i];
							count++;
						}
						return atoi(arr);
						break;
					}
					j++;
					temp++;
				}
			}
		}
		count=0;
	}
	
	return 0;
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
		case MQTTApp_SubMessageHandling:
		{
			if(IsSubscribeMsgRecieved == TRUE)
			{
				IsSubscribeMsgRecieved = FALSE;
				UBYTE cmd = MQTT_StringSeperate("\"cmd\":",'\"');
				char temp[5];
				sprintf(temp,"%d",cmd);
				temp[4] = '\0';
				//DebugStringRow2(temp);
				if (cmd == 255)
				{
					MQTTApp_State = MQTTApp_UpdateBeforeIdle;
				}
				else if (cmd == 1)
				{
					MQTTApp_State = MQTTApp_ReplyMsg;
				}
				else
				{
					//Invalid data received Error
				//	PORTA = 0X00;
				//	DebugStringRow1("error");
				}
				UBYTE IO_Control = MQTT_StringSeperate("\"m1\":",'}');
				
				char temp2[5];
				sprintf(temp2,"%d",IO_Control);
				temp2[4] = '\0';

				if (IO_Control == 1)				
				{
				//	DebugStringRow1("on");
				//	PORTA = 0x00;
				//	PORTB = 0xFF;
				}
				else
				{
					//DebugStringRow1("off");
				//	PORTA = 0xFF;
				}
				
			}
			else
			{
				//Rx_timout Have to implement
			}
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

