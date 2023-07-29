/*
 * MQTT.c
 *
 * Created: 23-12-2022 11:43:31
 *  Author: Admin
 */ 
/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "Includes.h"
#include SIMCOM_H
#include SIMCOM_MQTT_H
#include STRINGHELPER_H
#include SIMCOM_MQTT_H
#include SIMCOM_MQTT_PUBLISH_H
#include GPIO_DRIVER_H
#include MQTT_APPLICATION_H
#include SIMCOM_MQTT_SSL_H
/*****************************************/
/* Global Variables                      */
/*****************************************/

MQTT_State_EN MQTT_State = MQTT_START;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE MQTT_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;

UBYTE statechangecount = 250;

unsigned short int MQTT_ConnectionCheckCounter = 10000;//check MQTT conection For Every 1 min

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void MQTT_CALLBACK(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}

/*****************************************/
/* Function Definitions                  */
/*****************************************/

void MQTT_StateMachine(void)
{
	MQTT_State_EN MQTT_State_Before_Execution = MQTT_State;

	BOOL RetryInNextCycle = FALSE;
	
	if(IsMQTT_SSL_Configuration_Completed())
	{
		if(MQTTConnectionCheckStatus == TRUE)
		{
			if (MQTT_ConnectionCheckCounter > 0)
			{
				MQTT_ConnectionCheckCounter--;
			}
			else
			{
				MQTT_State = MQTT_Connection_Check;
			}
		}
		
		switch(MQTT_State)
		{
			case MQTT_START:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSTART", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
						UBYTE MQTTStatus = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSTART: ", 1);
						
						if(MQTTStatus == 0)
						{
							MQTT_State = MQTT_Accquire;
						}
						else if(SIMCOM_IsResponseError())
						{
							MQTT_State = MQTT_Accquire;
						}
					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{
						// If there is a problem in reception, retry sending the command
						RetryInNextCycle = TRUE;
						// TODO: Log Error.
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;
			case MQTT_Accquire:
			{
				//Accquiring a client to connect with the MQTT broker
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTACCQ=0,\"cp0001\",1", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_SSL_Configure;// Move to next state
						}
						//if the response is error,then a client already accquired,so release the client & then connect 
						else if(SIMCOM_IsResponseError())
						{
							MQTT_State = MQTT_ClientRelease;
						}
						else
						{
							// If the returned value issomething else, then act accordingly
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

			case MQTT_ClientRelease:
			{
				//if a client already accquired by default, then release the client in this state
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTREL=0", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_Accquire;// Move to next state
						}
						//if response is error,then already a client connected with MQTT broker,so disconnect the MQTT connection
						else
						{
							MQTT_State = MQTT_Disconnect;
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

			case MQTT_SSL_Configure:
			{
				//condfigure the  SSL layer to connect the MQTT broker with the aws certifcates
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSSLCFG=0,0", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_Connect; // Move to next state
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
			case MQTT_Connect:
			{
				// Connect the client with the MQTT broker to publish & subscribe messages
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job(ENDPOINT, SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_WaitForConnectResponce; // Move to next state
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
			
			case MQTT_Disconnect:
			{
				//when a default client is connected with the MQTT broker then disconnect the client to configure with the new client
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTDISC=0,60", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
						//if the client has disconnected,then move to the next start condition to establish MQTT connection 						
						if(SIMCOM_IsResponseOK())
						{
							MQTT_State = MQTT_START;
						}
						// if response is error or something else,then stop all these process and start from the MQTT stop
						else
						{
							MQTT_State = MQTTSTOP;
						}
					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{
						// If there is a problem in reception, retry sending the command
						RetryInNextCycle = TRUE;
						// TODO: Log Error.
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;
			case MQTT_WaitForConnectResponce:
			//wait till MQTT connection establishes with the client for sometimes
			{
				if(statechangecount!=0)
				{
					statechangecount--;
				}
				else
				{
					MQTT_State = MQTT_SubscribeTopic_Config;
					statechangecount = 250;
				}
			}
			break;
			case MQTT_SubscribeTopic_Config:
			{
				//configure the AT+CMQTTSUBTOPIC to enter the topic to be subscribed
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSUBTOPIC=0,16,1", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_SubTopic_Name_Config; // Move to next state
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
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;
			
			case MQTT_SubTopic_Name_Config:
			{
				//Enter the topic to which the MQTT client has to subscribed
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job(SUBTOPIC, SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_SubTopicNameUpdate; // Move to next state
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
			case MQTT_SubTopicNameUpdate:
			{
				//Subscribe the MQTT client to the Topic
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSUB=0", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
						//	MQTT_State = MQTT_WaitForSubResponce;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						if(SIMCOM_IsResponseOK())
						{
							MQTT_State = MQTT_WaitForSubResponce;// Move to next state
							SIMCOM_ComState = SIMCOM_WaitingForResponse;
						}
						else
						{
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
			
			case MQTT_WaitForSubResponce:
			{
				//wait for sometime to make the client subscribe
				if(statechangecount!=0)
				{
					statechangecount--;
				}
				else
				{
					MQTT_State = MQTT_Ready;
					statechangecount = 250;
				}
			}
			break;
			
			case MQTTSTOP:
			{
				//if MQTT is already Started but the connection establishment is not done then stop the connection & restart it again 
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSTOP", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_START;
							// Move to next state
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
			case MQTT_Connection_Check:
			{
				if ((SimcomWorkingMode == MQTTMode) && (MQTT_ConnectionCheckCounter <= 0) && (!ISPublishMsgConfigured()))
				{
					//This part is to check whether the SIMCOM is connected to aws server
					// in a regular interval about(10 min once) 
					if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
					{
						// Send AT Command and wait for response
						if(SIMCOM_Schedule_Job("AT+CMQTTCONNECT?", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							//if SIMCOM is connected then, publish the currrent status again to the server 
							//to make the apllication update again
							if(IsSIMCOM_ResponseStartsWith("+CMQTTCONNECT: 0,\"tcp:"))
							{							
								MQTT_State = MQTT_Ready;
								if(MQTTApp_State == MQTTApp_Idle)
								{
									MQTTApp_State = MQTTApp_Init;
								}
							}
							else
							{
								//else start from ssl certificate init
								C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;
								MQTT_State = MQTT_START;
								Publish_State = MQTT_Publish_Idle;
							}
							MQTT_ConnectionCheckCounter = 10000;

						}
// 						else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
// 						{
// 							// If there is a problem in reception, retry sending the command
// 							RetryInNextCycle = TRUE;
// 							// TODO: Log Error.
// 						}
						else
						{
							// Do Nothing. Wait
						}
					}
				}
				else
				{
					MQTT_State = MQTT_Ready;
				}
			}
			break;
			case MQTT_Ready:
			{
				//do nothing
			}
			break;
			case SIMCOM_MQTT_Connection_Error:
			default:
			{
				// Do Nothing, The state machine has been completed
			}
			break;
		}

		if(RetryInNextCycle == TRUE)
		{
			// If Retry is allowed
			if(MQTT_Retry_Count != 0)
			{
				MQTT_Retry_Count--; // Decrement the Retry Count

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
		if(MQTT_State_Before_Execution != MQTT_State)
		{
			// If changed, the Set the New Job Result as Idle for the next state to proceed further
			SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

			MQTT_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT; // Reset the Retry Count
			
			if(MQTT_State == MQTT_WaitForSubResponce)
			{
				MQTT_Retry_Count = 50;
			}
		}
	}
}
