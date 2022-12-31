/*
 * MQTT_SSL.c
 *
 * Created: 23-12-2022 11:44:47
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include MQTT_SSL_CONFIGURATION_H
#include STRINGHELPER_H
#include LCD_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

C_MQTT_SSL_Configuration_State_EN C_MQTT_SSL_Config_State = C_MQTT_SSL_Init;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SIMCOM_SSL_Config_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SIMCOM_SSL_Configration_Callback(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}


/*****************************************/
/* Function Definitions                  */
/*****************************************/

void SIMCOM_SSL_CONFIG_MainFunction(void)
{
	if(IsSIMCOM_Module_Ready())
	{
		C_MQTT_SSL_Configuration_State_EN C_MQTT_SSL_Configuration_State_Before_Execution = C_MQTT_SSL_Config_State;

		BOOL RetryInNextCycle = FALSE;

		switch(C_MQTT_SSL_Config_State)
		{
			case C_MQTT_SSL_Init:
			{
				

				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSSLCFG=\"sslversion\",0,4", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_SSL_Configration_Callback) == TRUE)
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

							C_MQTT_SSL_Config_State = C_MQTT_SSL_Authentication; // Move to next state
							
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
			
			case C_MQTT_SSL_Authentication:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSSLCFG=\"authmode\",0,2", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_SSL_Configration_Callback) == TRUE)
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
							C_MQTT_SSL_Config_State = C_MQTT_SSL_CaCert_Configuration; // Move to next state
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


			case C_MQTT_SSL_CaCert_Configuration:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_SSL_Configration_Callback) == TRUE)
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
							C_MQTT_SSL_Config_State = C_MQTT_SSL_Clientcert_Configuration; // Move to next state
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



			case C_MQTT_SSL_Clientcert_Configuration:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSSLCFG=\"clientcert\",0,\"clientcert.pem\"", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_SSL_Configration_Callback) == TRUE)
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
							C_MQTT_SSL_Config_State = C_MQTT_SSL_Clientkey_Configuration; // Move to next state
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


			case C_MQTT_SSL_Clientkey_Configuration:
			{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSSLCFG=\"clientkey\",0,\"clientkey.pem\"", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_SSL_Configration_Callback) == TRUE)
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
							C_MQTT_SSL_Config_State = C_MQTT_SSL_Configured; // Move to next state
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
			default:
			case C_MQTT_SSL_Configured:
			
			case C_MQTT_SSL_Configration_Error:
			{
				// Do Nothing, The state machine has been completed
			}
			break;
		}

		if(RetryInNextCycle == TRUE)
		{
			// If Retry is allowed
			if(SIMCOM_SSL_Config_Retry_Count != 0)
			{
				SIMCOM_SSL_Config_Retry_Count--; // Decrement the Retry Count

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
			// If in any of the state, the Job is aborted, then move to the Error State

			C_MQTT_SSL_Config_State = C_MQTT_SSL_Configration_Error;

			/* Send Appropriate Error Code to Application */
			{
				SIMCOM_Error_State_EN ErrorState = SIMCOM_Error_Unknown;

				switch(C_MQTT_SSL_Config_State)
				{
					// 						case SIMCOM_SSL_Init                         : ErrorState = SIMCOM_Error_SSL_Init; break;
					// 						case SIMCOM_SSL_Authentication               : ErrorState = SIMCOM_Error_SSL_Authentication; break;
					// 						case SIMCOM_SSL_CaCert_Configuration         : ErrorState = SIMCOM_Error_SSL_CaCert_Configuration; break;
					// 						case SIMCOM_SSL_Clientcert_Configuration     : ErrorState = SIMCOM_Error_SSL_Clientcert_Configuration; break;
					// 						case SIMCOM_SSL_Clientkey_Configuration      : ErrorState = //SIMCOM_Error__SSL_Clientkey_Configuration; break;
					default:
					// Do Nothing, SIMCOM Module will timeout and reerror
					break;
				}

				SIMCOM_ERROR_CALLBACK(ErrorState);
			}
		}

		/* Check if the state changed after execution */
		if(C_MQTT_SSL_Configuration_State_Before_Execution != C_MQTT_SSL_Config_State)
		{
			// If changed, the Set the New Job Result as Idle for the next state to proceed further
			SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

			SIMCOM_SSL_Config_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT; // Reset the Retry Count
		}
	}
	else
	{
		//SIMCOM_SSL_Config_State = SIMCOM_SSL_Configuration_Idle;
	}
}