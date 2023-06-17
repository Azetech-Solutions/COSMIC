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
#include GPIO_DRIVER_H
#include LCD_DRIVER_H
#include GPIO_DRIVER_H
#include UART_DRIVER_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

//StatusData_ST StatusData;
AvrCmdData_ST AvrCmdData;

MQTTApp_States MQTTApp_State = MQTTApp_Init;

extern void USART1_String(const char* data);
extern void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data);

UBYTE Current_IOStatus = 0;
UBYTE Previous_IOStatus = 0;
BOOL MQTTConnectionCheckStatus = FALSE;
StatusData_ST StatusData;
/***************************************************/
/* Function Definitions                            */
/***************************************************/


extern UBYTE avrstatus[2];

extern UBYTE ComIf_RxIndication_Avr(UBYTE DataByte);

extern UBYTE AvrTransmitFunc(UWORD Length, void * Data);

extern void SIM_Send_Data(unsigned char Data);

UBYTE AvrTransmitFunc(UWORD Length, void * Data)
{
	UBYTE retval = COMIF_EC_NO_ERROR;
	
	for(UBYTE i = 0;i < Length; i++)
	{
		SIM_Send_Data(((*(UBYTE*)Data++)));
	}
	
	memset(Data,'0',Length);
	
	ComIf_TxConfirmation(C_ComIfChannel_Avr);
	
	return retval;
}

void updateSendData(UBYTE Data[])
{
	AvrCmdData_ST *AvrCmd = &AvrCmdData;
	
	AvrCmd->Data_Bytes[0] = Data[0];
	
	AvrCmd->Data_Bytes[1] = Data[1];
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Avr_AvrTxFunc();
	
	memcpy(PubMsg,AvrCmd->Data_Bytes,2);
	
	ComIf_TransmitFromBuffer_Avr_AvrTxFunc();
	
}

void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data)
{
	UBYTE i,StatusData[2];
	for(i = 0; i<Length ;i++)
	{
		StatusData[i] = *Data;
	}
	updateSendData(StatusData);
}



UBYTE Cloud_Transmit(UWORD Length, void * Data)
{	
	UBYTE retval = COMIF_EC_NO_ERROR;
	
	if (!ISPublishMsgConfigured())
	{
		
		memcpy(PublishPayload,Data,Length);
		ComIf_TxConfirmation(C_ComIfChannel_Cloud);
	}
	else
	{
		retval = COMIF_EC_CHANNEL_BUSY;
	}
	
	return retval;
}

//function to send status data(like led on or led off)from microcontroller to cloud to update application  
void RequestLastStatus(UBYTE sts[])
{
	StatusData_ST *CurrentStatus = &StatusData;
	
	CurrentStatus->Data_Bytes[0] = sts[0];
	
	CurrentStatus->Data_Bytes[1] = sts[1];
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Cloud_StatusData();
	
	memcpy(PubMsg,CurrentStatus->Data_Bytes,2);
	
	ComIf_TransmitFromBuffer_Cloud_StatusData();
}


void MQTT_AppMain()
{	
	if(IsMQTT_Ready())
	{
		switch(MQTTApp_State)
		{
			case MQTTApp_Idle:
			{
				//this part is to update the current status of the led changed based on the inputs from the application  
				MQTTConnectionCheckStatus = TRUE;	
				
				GPIOA->ODR & (1<<0) ? (Current_IOStatus &= ~(1<<0)): (Current_IOStatus |= (1<<0));
				
				GPIOA->ODR & (1<<1) ? (Current_IOStatus &= ~(1<<1)) : (Current_IOStatus |= (1<<1));
	
				//if previous status data & new status data both are same then no need to update the current status,
				//if the status data varies,then update the current status
				if (Current_IOStatus != Previous_IOStatus)
				{
						Previous_IOStatus = Current_IOStatus;
					MQTTApp_State = MQTTApp_Publish_IO_state;
				}

				
			}
			break;
			
			case MQTTApp_Init:
			{
				//default status command 255 is send through this function to make  the cloud update 
				//that the microcontroller is ready to recieve data 
				if(IsMQTT_Ready())
				{
					UBYTE LastStatus[2] = {255,0};
					RequestLastStatus(LastStatus);
					MQTTApp_State = MQTTApp_PublishMsgConfiguringinprocess;
				}
			}
			break;
			
			case MQTTApp_ComifRxindication:
			{
				ComIf_RxIndication_Cloud(TOPIC1_SubscribeMsg,strlen(TOPIC1_SubscribeMsg));
			
				memset(TOPIC1_SubscribeMsg,0,strlen(TOPIC1_SubscribeMsg));
			}
			break;
			case MQTTApp_Publish_IO_state:
			{	
				//this part is to publish the command status of the 
				
				UBYTE *Data  = ComIf_GetShadowBuffer_Cloud_TxCommandData();
				
				Data[0] = avrstatus[0];
				
				Data[1] = avrstatus[1];
				
				for(UBYTE i=2;i<8;i++)
				{
					Data[i] = 0;
				}

				ComIf_TransmitFromBuffer_Cloud_TxCommandData();
				MQTTApp_State = MQTTApp_PublishMsgConfiguringinprocess;
			}
			break;
			
			case MQTTApp_PublishMsgConfiguringinprocess:
			{
				Publish_State = MQTT_PublishTopic_Config;
				MQTTApp_State = MQTTApp_PublishMsgConfigured;
			}
			
			case MQTTApp_PublishMsgConfigured:
			{
				//do nothing
			}
			
			default:
			{
				//do Nothing
				break;
			}
		}
	}
}

void Cloud_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1)
{
	
}


void COSMIC_SIMCOM_Error_Callback()
{
	SIMCOM_State = SIMCOM_SM_Reset;
	C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;
	MQTT_State = MQTT_START;
	MQTTApp_State = MQTTApp_Init;
	Publish_State = MQTT_Publish_Idle;
}


void COSMIC_Generic_SIMCOM_Callback(SIMCOM_Job_Result_EN JobState)
{

	/* This function will be called for an un-scheduled job. So check for the response and clear the buffer */
	SIMCOM_ClearResponseBuffer();
}