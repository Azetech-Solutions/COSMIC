/*
 * MQTT_Connection_Establishment.c
 *
 * Created: 10-12-2022 16:20:39
 *  Author: Admin
 */ 
/*
 * SIMCOM_State.c
 *
 *  Created on: 17-Apr-2021
 *      Author: Hari
 */


#include "Includes.h"
#include SIMCOM_H
#include SIMCOM_SSL_CONFIGURATION_H
#include STRINGHELPER_H
#include MQTT_HEADER_H
#include <avr/io.h>
#include "LCD.h"

/*****************************************/
/* Global Variables                      */
/*****************************************/

MQTT_State_EN MQTT_State = MQTTSTART;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE MQTT_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;



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

	if(IsSIMCOM_SSL_Configuration_Completed())
	{
		switch(MQTT_State)
		{
			case MQTTSTART:
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
						// Positive Response would be -> +CLTS: <mode>
						// <mode> : 0 Disable   1 Enable
						
						ULONG MQTTStatus = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSTART: ", 1);
						
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
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTACCQ=0,\"COSMIC_Device01\",1", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_SSL_Configure;

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



			case MQTT_SSL_Configure:
			{
				// First Ensure the SIMCOM Module is Connected
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
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTCONNECT=0,\"tcp://a1nwcqrp1qjjtm-ats.iot.ap-south-1.amazonaws.com:8883\",60,1", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
							MQTT_State = MQTT_SubscribeTopic_Config; // Move to next state
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

			case MQTT_SubscribeTopic_Config:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSUBTOPIC=0,17,1", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("aws/things/COSSUB", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CMQTTSUB=0", SIMCOM_DEFAULT_TIMEOUT, MQTT_CALLBACK) == TRUE)
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
						MQTT_State = MQTT_WaitForSubResponce;
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
				// Job has been completed
				ULONG SubscribeResponse1 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSUB: ", 1);
				ULONG SubscribeResponse2 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSUB: ", 2);
				// Check if the response is OK or not.
				if((SubscribeResponse2==0)&&(SubscribeResponse2==0))
				{
					MQTT_State = MQTT_Ready; // Move to next state
					
				}
				else if(SIMCOM_IsResponseOK())
				{
					MQTT_State = MQTT_Ready; // Move to next state
				}
				else
				{
					//do nothing
				}
			}
			break;
			default:
			case MQTT_Ready:
			{
				DebugStringRow1("MQtt_Ready");
			}
			case SIMCOM_MQTT_Connection_Error:
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

			

			//SIMCOM_ERROR_CALLBACK(SIMCOM_Error_GPRS);
		}

		/* Check if the state changed after execution */
		if(MQTT_State_Before_Execution != MQTT_State)
		{
			// If changed, the Set the New Job Result as Idle for the next state to proceed further
			SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

			MQTT_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT; // Reset the Retry Count
		}
	}
}
