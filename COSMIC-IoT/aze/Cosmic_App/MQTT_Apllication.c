/*
 * MQTT_Apllication.c
 *
 * Created: 27-12-2022 15:38:18
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include STRINGHELPER_H
#include MQTT_HEADER_H
#include MQTT_APLLICATION_H
#include <stdio.h>
#include <avr/io.h>
#include "LCD.h"

MQTTApp_States MQTTApp_State = MQTTApp_Init;

char SubscribeDataBuff[30];

UBYTE MQTT_StringSeperate(char *str,char endpoint);

BOOL IsSubscribeMsgRecieved = FALSE;

void UpdatePublishdata(UBYTE Cmd)
{
	UBYTE IO_status;
	
	if (PORTA0 & 0x1)
	{
		IO_status = 0;
	}
	else
	{
		IO_status = 1;
	}
	sprintf(PublishPayload,PublishString,Cmd,IO_status);
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
				DebugStringRow2(temp);
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
					PORTA = 0X00;
				//	DebugStringRow1("error");
				}
				UBYTE IO_Control = MQTT_StringSeperate("\"m1\":",'}');
				
				char temp2[5];
				sprintf(temp2,"%d",IO_Control);
				temp2[4] = '\0';
				LCD_command(0xC6);
				Display_String(temp2);
				
				if (IO_Control == 1)				
				{
					DebugStringRow1("on");
					PORTA = 0x00;
					PORTB = 0xFF;
				}
				else
				{
					DebugStringRow1("off");
					PORTA = 0xFF;
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

