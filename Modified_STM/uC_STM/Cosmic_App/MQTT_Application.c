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
#include COMIF_H
#include COMIF_CONFIG_H
#include SIMCOM_H
#include STRINGHELPER_H
#include SIMCOM_MQTT_H
#include SIMCOM_MQTT_SSL_H
#include MQTT_APPLICATION_H
#include SIMCOM_MQTT_PUBLISH_H
#include <stdio.h>
#include GPIO_DRIVER_H
#include GPIO_DRIVER_H
#include UART_DRIVER_H
#include MESSAGE_APP_H
#include BUFFER_CONFIG_H
#include AVR_H
#include MQTT_APPLICATION_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

AvrCmdStatusData_ST AvrCmdData;

MQTTApp_States MQTTApp_State = MQTTApp_Init;

UBYTE Current_IOStatus = 0;

UBYTE Previous_IOStatus = 0;

BOOL MQTTConnectionCheckStatus = FALSE;

UBYTE MachineInitFlag = FALSE;

SimcomWorkingMode_ST SimcomWorkingMode = MQTTMode;

AvrCmdStatusData_ST prevAvrStatusData;

AvrCmdStatusData_ST prevCloudStatusData;

/***************************************************/
/* Function Declarations                           */
/***************************************************/

void IO_cmdData_CloudRxCbk(UBYTE Length, UBYTE *Data);

void CloudInit();

/***************************************************/
/* Function Definitions                            */
/***************************************************/

void updateSendData(UBYTE Data[])
{
	
	AvrCmdData.Data_Bytes[0] = Data[0];
	
	AvrCmdData.Data_Bytes[1] = Data[1];
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_AVR_IO_cmdData_AVR();
	
	memcpy(PubMsg,AvrCmdData.Data_Bytes,2);
	
	ComIf_TransmitFromBuffer_AVR_IO_cmdData_AVR();
	
}

void IO_cmdData_CloudRxCbk(UBYTE Length, UBYTE *Data)
{
	UBYTE i,StatusData[2];
	for(i = 0; i<Length ;i++)
	{
		StatusData[i] = *Data;
		Data++;
	}
	updateSendData(StatusData);
}



UBYTE Cloud_Transmit(UWORD Length, void * Data)
{	
	UBYTE retval = COMIF_EC_NO_ERROR;

	char *PtrData = (char *)Data;
	
	if (!ISPublishMsgConfigured())
	{
		
		memcpy(PublishPayload,PtrData,Length);	
		

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
	AvrCmdStatusData_ST *CurrentStatus = &AvrStatusData;
	
	CurrentStatus->Data_Bytes[0] = sts[0];
	
	CurrentStatus->Data_Bytes[1] = sts[1];
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Cloud_IO_Status_Cloud();
	
	memcpy(PubMsg,CurrentStatus->Data_Bytes,2);
	
	ComIf_TransmitFromBuffer_Cloud_IO_Status_Cloud();
}

void CloudInit()
{
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Cloud_Cloud_Init();
	
	UBYTE val[2] = {255,0};
	
	memcpy(PubMsg,val,2);
	
	ComIf_TransmitFromBuffer_Cloud_Cloud_Init();
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
	
				//if previous status data & new status data both are same then no need to update the current status,
				//if the status data varies,then update the current status
			}				
			break;
			
			
			case MQTTApp_CheckIOStatus:
			{
				
				MQTTConnectionCheckStatus = FALSE;
				
				if((prevCloudStatusData.Data_Bytes[0] == AvrStatusData.Data_Bytes[0]) 
					&& (prevCloudStatusData.Data_Bytes[1] == AvrStatusData.Data_Bytes[1]))
				{
					MQTTApp_State = MQTTApp_Idle;
				}
				else
				{
					MQTTApp_State = MQTTApp_Publish_IO_state;
				}
				prevCloudStatusData.Data_Bytes[0] = AvrStatusData.Data_Bytes[0];
				prevCloudStatusData.Data_Bytes[1] = AvrStatusData.Data_Bytes[1];
				break;
			}
			break;
			
			case MQTTApp_Init:
			{
				//default status command 255 is send through this function to make  the cloud update 
				//that the microcontroller is ready to recieve data 
				if(IsMQTT_Ready())
				{
					CloudInit();
					MQTTApp_State = MQTTApp_PublishMsgConfiguringinprocess;
					MachineInitFlag = TRUE;
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
				
				UBYTE *Data  = ComIf_GetShadowBuffer_Cloud_IO_Status_Cloud();
				
				Data[0] = AvrStatusData.Data_Bytes[0];
				
				Data[1] = AvrStatusData.Data_Bytes[1];
				
				ComIf_TransmitFromBuffer_Cloud_IO_Status_Cloud();
				
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

void MQTT_Init()
{
	C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;
	MQTT_State = MQTT_START;
//	MQTTApp_State = MQTTApp_Init;
	MQTTApp_State = MQTTApp_CheckIOStatus;
	Publish_State = MQTT_Publish_Idle;
}


/***************************************************/
/* Header Inclusions                               */
/***************************************************/

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/*****************************************/
/* Global Variables                      */
/*****************************************/

/***************************************************/
/* Function Declarations                           */
/***************************************************/

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

/***************************************************/
/* Function Definitions                            */
/***************************************************/
