/*
 * MQTT_Publish.c
 *
 * Created: 23-12-2022 11:46:09
 *  Author: Admin
 */ 
/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "Includes.h"
#include "stm32g0xx.h"                  // Device header
#include SIMCOM_H
#include SIMCOM_MQTT_SSL_H
#include STRINGHELPER_H
#include SIMCOM_MQTT_H
#include GPIO_DRIVER_H
#include SIMCOM_MQTT_PUBLISH_H
#include MQTT_APPLICATION_H
#include COMIF_CONFIG_H
#include "stdio.h"
#include UART_DRIVER_H
#include MESSAGE_APP_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

/*****************************************/
/* Global Variables                      */
/*****************************************/


MQTT_Publish_state_EN Publish_State = MQTT_Publish_Idle;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE MQTT_Publish_Retry_Count = 50;

char PublishPayload[28];

/***************************************************/
/* Function Declarations                           */
/***************************************************/

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void MQTT_Publish_CallBack(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}

/***************************************************/
/* Function Definitions                            */
/***************************************************/


void MQTT_Publish_StateMachine(void)
{
	MQTT_Publish_state_EN MQTT_Publish_State_Before_Execution = Publish_State;

	BOOL RetryInNextCycle = FALSE;

	if(IsMQTT_Ready())
	{
		switch(Publish_State)
		{
			case MQTT_Publish_Idle:
			{
				
			}
			break;
			
			case MQTT_PublishTopic_Config:
			{
				if(ISPublishMsgConfigured())
				{
					// First Ensure the SIMCOM Module is Connected
					if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
					{
						//Set topic Length by using Macro The size of the Topic is Configured in PUBTOPICLength Macro
						char SetTopicLength[20];
						
						sprintf(SetTopicLength,"AT+CMQTTTOPIC=0,%d",PUBTOPICLength);
						
						// Send AT Command and wait for response
						if(SIMCOM_Schedule_Job(SetTopicLength, SIMCOM_DEFAULT_TIMEOUT, MQTT_Publish_CallBack) == TRUE)
						{
							// Set it to Scheduled only when the SIMCOM Module Accepted it
							SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
							PREVPUBLISHSTATE = TRUE;
						}
					}
					else
					{
						// Cyclic part for the response
						if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
						{
							// Job has been completed

							// Check if the response is OK or not.
							if(SIMCOM_IsResponse_Entermessage())
							{
								Publish_State = MQTT_PubTopic_Name_Config; // Move to next state
								MQTTApp_State = MQTTApp_Idle;
							}
							else
							{
								// If the returned value is ERROR or something else, then act accordingly
								// TODO: Later
								RetryInNextCycle = TRUE;
							}
						}
						else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
						{
							// If there is a problem in reception, retry sending the command
							RetryInNextCycle = TRUE;

							// TODO: Log Error. Possibly the GSM Module is not powered or connected
						}
						else
						{
							// Do Nothing. Wait
						}
					}
				}
			}
			break;

			case MQTT_PubTopic_Name_Config:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
						
					if(SIMCOM_Schedule_Job(PUBTOPIC, SIMCOM_DEFAULT_TIMEOUT, MQTT_Publish_CallBack) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						// Job has been completed

						// Check if the response is OK or not.
						if(SIMCOM_IsResponseOK())
						{
							Publish_State = MQTT_UpdatePayloadLength; // Move to next state
						}
						else
						{
							// If the returned value is ERROR or something else, then act accordingly
							// TODO: Later
							RetryInNextCycle = TRUE;
						}
					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{
						// If there is a problem in reception, retry sending the command
						RetryInNextCycle = TRUE;

						// TODO: Log Error. Possibly the GSM Module is not powered or connected
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;
			case MQTT_UpdatePayloadLength:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					char SetPayloadLength[20];
										
					sprintf(SetPayloadLength,"AT+CMQTTPAYLOAD=0,%d",strlen(PublishPayload));
					
					/*Set Payload length For The Publish Msg, Now its sets statically if we need dynamic change in Payload we have to update the Length*/ 
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job(SetPayloadLength, SIMCOM_DEFAULT_TIMEOUT, MQTT_Publish_CallBack) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						// Job has been completed
						// Check if the response is OK or not.
						if(SIMCOM_IsResponse_Entermessage())
						{
							Publish_State = MQTT_Update_PubDataforTopic; // Move to next state
						}
						else
						{
							// If the returned value is ERROR or something else, then act accordingly
							// TODO: Later
							RetryInNextCycle = TRUE;
						}
					}
				}
			}
			break;
				
			case MQTT_Update_PubDataforTopic:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					GPIOA->ODR |= (1<<0);
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job(PublishPayload, SIMCOM_DEFAULT_TIMEOUT, MQTT_Publish_CallBack) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{						
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						if (SIMCOM_IsResponseOK())
						{
							Publish_State = MQTT_Publish_Msg; // Move to next state
						}
						else
						{
								
						}
						// Job has been completed
						// Check if the response is OK or not.
					}
				}
			}
			break;
			case MQTT_Publish_Msg:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTPUB=0,1,60", SIMCOM_DEFAULT_TIMEOUT, MQTT_Publish_CallBack) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						Publish_State = MQTTWaitforPublishResponse;
					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{
						// If there is a problem in reception, retry sending the command
						RetryInNextCycle = TRUE;

						// TODO: Log Error. Possibly the GSM Module is not powered or connected
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;
			
			case MQTTWaitforPublishResponse:
			{
				if(statechangecount!=0)
				{
					statechangecount--;
				}
				else
				{
					Publish_State = MQTT_PublishMsgPublished;
					statechangecount = 250;
				}
			}
			case MQTT_PublishMsgPublished:
			{
				Publish_State = MQTT_Publish_Idle;
				if(MachineInitFlag == TRUE)
				{
						MachineInitFlag = FALSE;
//						DtmfMessageHandlerState = MachineInit;			
				}
			}
			break;
			default:
			{
				// Do Nothing, The state machine has been completed
			}
			break;
		}

		if(RetryInNextCycle == TRUE)
		{
			// If Retry is allowed
			if(MQTT_Publish_Retry_Count != 0)
			{
				MQTT_Publish_Retry_Count--; // Decrement the Retry Count

				SIMCOM_Job_Result = SIMCOM_Job_Idle; // Setting the Job to Idle will retry the Job again in next cycle.
			}
			else
			{
				// RETRY Mechanism expired, abort the Job and do not move the state
				SIMCOM_Job_Result = SIMCOM_Job_Aborted;
			}
		}
		if(SIMCOM_Job_Result == SIMCOM_Job_Aborted)
		{
			// If in any of the state, the Job is aborted, then move to the error state
			SIMCOM_ERROR_CALLBACK();
		}

		/* Check if the state changed after execution */
		
	}
	if(MQTT_Publish_State_Before_Execution != Publish_State)
	{
		// If changed, the Set the New Job Result as Idle for the next state to proceed further
		SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

		MQTT_Publish_Retry_Count = 10; // Reset the Retry Count
	}
	
}