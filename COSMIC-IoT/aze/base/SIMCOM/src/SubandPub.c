/*
 * SubandPub.c
 *
 * Created: 12-12-2022 19:21:23
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include SIMCOM_SSL_CONFIGURATION_H
#include STRINGHELPER_H
#include MQTT_HEADER_H
#include <avr/io.h>
#include "LCD.h"
#include SUBANDPUB_H
#include "stdio.h"
/*****************************************/
/* Global Variables                      */
/*****************************************/

MQTT_Subandpub_state_EN SubandPub_State = MQTT_SubPub_Idle;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE MQTT_SubandPub_Retry_Count = 50;

//static UBYTE Machine_status = 1;

char PublishData[50];

char Payloadcmd[50];

ULONG PublishTimoutCounter = 30;

UBYTE PrevData=0;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void MQTT_SubPub_CallBack(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}


/*****************************************/
/* Function Definitions                  */
/*****************************************/

UBYTE UpdatePublishdata()
{
	UBYTE IO_status=0;
	
/*	if(Machine_status<10)
	{
		Machine_status++;
	} 
	else
	{
		Machine_status = 1;
	}
*/	
	
	if(PORTA & 0xFF)
	{
		IO_status = 0;
	}
	else
	{
		IO_status = 1;
	}
	if(PrevData != IO_status)
	{
		
	}
	sprintf (PublishData, "{\"IO_status\":\"%d\"}",IO_status);
		
	UBYTE len  = strlen(PublishData);
	
	return len;
}

int i=0;

void MQTT_SubPub_StateMachine(void)
{
	MQTT_State_EN MQTT_SubandPub_State_Before_Execution = SubandPub_State;

	BOOL RetryInNextCycle = FALSE;

	if(IsMQTT_Ready())
	{
		PORTB = 0X00;
		switch(SubandPub_State)
		{
			
			case MQTT_SubPub_Idle:
			{
				if(PORTA & 0XFF)
				{
					
				}
				else
				{
					SubandPub_State = MQTT_PublishTopic1_Config;
				}
			}
			break;
			case MQTT_PublishTopic1_Config:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTTOPIC=0,18", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
							SubandPub_State = MQTT_PubTopic1_Name_Config; // Move to next state
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

			case MQTT_PubTopic1_Name_Config:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("aws/things/COSPUB1", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
							SubandPub_State = MQTT_SetPayloadforTopic1; // Move to next state
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
			case MQTT_SetPayloadforTopic1:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{	
					UBYTE Len = UpdatePublishdata();
					sprintf(Payloadcmd,"AT+CMQTTPAYLOAD=0,%d",Len);
						
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job(Payloadcmd, SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
							SubandPub_State = MQTT_Update_PubDataforTopic1; // Move to next state
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
			
			case MQTT_Update_PubDataforTopic1:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
						// Send AT Command and wait for response
						if(SIMCOM_Schedule_Job(PublishData, SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
							SubandPub_State = MQTT_Publish_Msg1; // Move to next state
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
			case MQTT_Publish_Msg1:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTPUB=0,1,60", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
						SubandPub_State = MQTTWaitforPublishResponse1;
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
			
			case MQTTWaitforPublishResponse1 :
			{
				if(PublishStatus == 1)
				{
					SubandPub_State = MQTT_PublishTopic2_Config;
				}
			}
			break;
			
					case MQTT_PublishTopic2_Config:
					{
						// First Ensure the SIMCOM Module is Connected
						if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
						{
							// Send AT Command and wait for response
							if(SIMCOM_Schedule_Job("AT+CMQTTTOPIC=0,18", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
									SubandPub_State = MQTT_PubTopic2_Name_Config; // Move to next state
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

					case MQTT_PubTopic2_Name_Config:
					{
						// First Ensure the SIMCOM Module is Connected
						if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
						{
							// Send AT Command and wait for response
							if(SIMCOM_Schedule_Job("aws/things/COSPUB2", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
									SubandPub_State = MQTT_SetPayloadforTopic2; // Move to next state
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
					case MQTT_SetPayloadforTopic2:
					{
						// First Ensure the SIMCOM Module is Connected
						if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
						{
							UBYTE Len = UpdatePublishdata();
							sprintf(Payloadcmd,"AT+CMQTTPAYLOAD=0,%d",Len);
								
							// Send AT Command and wait for response
							if(SIMCOM_Schedule_Job(Payloadcmd, SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
									SubandPub_State = MQTT_Update_PubDataforTopic2; // Move to next state
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

					case MQTT_Update_PubDataforTopic2:
					{
						// First Ensure the SIMCOM Module is Connected
						if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
						{
							// Send AT Command and wait for response
							if(SIMCOM_Schedule_Job(PublishData, SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
									SubandPub_State = MQTT_Publish_Msg2; // Move to next state
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
			
			case MQTT_Publish_Msg2:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTPUB=0,1,60", SIMCOM_DEFAULT_TIMEOUT, MQTT_SubPub_CallBack) == TRUE)
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
						SubandPub_State = MQTTWaitforPublishResponse2;
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
			
			case MQTTWaitforPublishResponse2:
			{
				if(PublishStatus == 1)
				{
					SubandPub_State = MQTT_SubPub_Idle;
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
			if(MQTT_SubandPub_Retry_Count != 0)
			{
				MQTT_SubandPub_Retry_Count--; // Decrement the Retry Count

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

			

			//SIMCOM_ERROR_CALLBACK(SIMCOM_Error_GPRS);
		}

		/* Check if the state changed after execution */
		if(MQTT_SubandPub_State_Before_Execution != MQTT_State)
		{
			// If changed, the Set the New Job Result as Idle for the next state to proceed further
			SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

			MQTT_SubandPub_Retry_Count = 50; // Reset the Retry Count
		}
	}
}
