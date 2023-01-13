/*
 * SIMCOM_State.c
 *
 *  Created on: 17-Apr-2021
 *      Author: Hari
 */

#include "Includes.h"
#include SIMCOM_H
#include STRINGHELPER_H
#include <avr/io.h>
#include MQTT_SSL_H
#include LCD_H
#include <stdio.h>

/*****************************************/
/* Global Variables                      */
/*****************************************/

void APN_Selection(char *checkstring);

SIMCOM_State_EN SIMCOM_State = SIMCOM_SM_Init;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SIMCOM_SM_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;

#define AIRTEL  "\"airtelgprs.com\""
#define VI      "\"internet\""
#define JIO     "\"jionet\""
#define BSNL    "\"bsnlnet\""

char APN_name[40];

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SIMCOM_StateMachine_Callback(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}

void APN_Selection(char *checkstring)
{
	checkstring = checkstring+5;
	UBYTE i;
	char *APN,temp[30];
	ULONG val;
	for(i=0;i<=3;i++)
	{
		temp[i] = *checkstring;
		checkstring++;
	}
	val = atoi(temp);
	if(val == 4044)
	{
		APN = VI;
	}
	else if(val == 4058)
	{
		APN = JIO;
	}
	else if(val == 4049)
	{
		APN = AIRTEL;
	}
	else if(val == 4048)
	{
		APN = BSNL;
	}
	sprintf(APN_name,"AT+CGDCONT=1,\"IP\",%s",APN);
}
/*****************************************/
/* Function Definitions                  */
/*****************************************/

void SIMCOM_StateMachine(void)
{
	SIMCOM_State_EN SIMCOM_State_Before_Execution = SIMCOM_State;
	
	BOOL RetryInNextCycle = FALSE;

	switch(SIMCOM_State)
	{
		case SIMCOM_SM_Init:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
						SIMCOM_State = SIMCOM_SM_SIM_Check;
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
		case SIMCOM_SM_SIM_Check:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{

				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CPIN?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
					char * RxString = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), "+CPIN: ");
							

					if(strcmp(RxString, "READY") == 0)
					{
						char * RxString = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), "+CPIN:READY");
								
						// Check if the response is OK or not.
						if(strcmp(RxString,"OK"))
						{
							SIMCOM_State = SIMCOM_DisableCall;
						}
						else
						{
							// If the returned value is ERROR or something else, then act accordingly
							// TODO: Later
							RetryInNextCycle = TRUE;
						}
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
		
		case SIMCOM_DisableCall:
		{
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CRC=0", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
					/* Accept both Roaming and Local Registration */
					if(SIMCOM_IsResponseOK())
					{
						SIMCOM_State = SIMCOM_DisableMsg; // Move to next state
					}
					else
					{
						if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
						{
							// Network Not Registered.
							// TODO: Throw Error including the Status returned by the SIM800 module
						}

						RetryInNextCycle = TRUE;
					}

				}
				else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
				{
					// If there is a problem in reception, retry sending the command
					RetryInNextCycle = TRUE;

					// TODO: Log Timeout Error.
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		
		case SIMCOM_DisableMsg:
		{
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CNMI=0,0,0,0", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
					/* Accept both Roaming and Local Registration */
					if(SIMCOM_IsResponseOK())
					{
						SIMCOM_State = SIMCOM_DisableGPRS_URC; // Move to next state
					}
					else
					{
						if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
						{
							// Network Not Registered.
							// TODO: Throw Error including the Status returned by the SIM800 module
						}

						RetryInNextCycle = TRUE;
					}

				}
				else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
				{
					// If there is a problem in reception, retry sending the command
					RetryInNextCycle = TRUE;

					// TODO: Log Timeout Error.
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		
		case SIMCOM_DisableGPRS_URC:
		{
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CSSN=0,0", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
					/* Accept both Roaming and Local Registration */
					if(SIMCOM_IsResponseOK())
					{
						SIMCOM_State = SIMCOM_SM_Check_signal_strength; // Move to next state
					}
					else
					{
						if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
						{
							// Network Not Registered.
							// TODO: Throw Error including the Status returned by the SIM800 module
						}

						RetryInNextCycle = TRUE;
					}

				}
				else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
				{
					// If there is a problem in reception, retry sending the command
					RetryInNextCycle = TRUE;

					// TODO: Log Timeout Error.
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		
		case SIMCOM_SM_Check_signal_strength:
		{
				// First Ensure the SIMCOM Module is Connected
				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CSQ", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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

						ULONG NetworkStrength = SIMCOM_GetCSV_Number_fromBuffer("+CSQ: ", 1);

						/* Accept both Roaming and Local Registration */
						if(NetworkStrength <= 31)
						{
						
							SIMCOM_State = SIMCOM_SM_NW_Registration_Check; // Move to next state
						
						}
						else
						{
							if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
							{
								// Network Not Registered.
								// TODO: Throw Error including the Status returned by the SIM800 module
							}

							RetryInNextCycle = TRUE;
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

		case SIMCOM_SM_NW_Registration_Check:
		{
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CREG?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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

					// The positive response would be -> +CREG: <n>,<stat>
					/*
					   <stat> 	0 Not registered, MT is not currently searching a new operator to register to
								1 Registered, home network
								2 Not registered, but MT is currently searching a new operator to register to
								3 Registration denied
								4 Unknown
								5 Registered, roaming
					 */

					ULONG NetworkRegistrationStatus = SIMCOM_GetCSV_Number_fromBuffer("+CREG: ", 2);

					/* Accept both Roaming and Local Registration */
					if((NetworkRegistrationStatus == 1) || (NetworkRegistrationStatus == 5))
					{
						
						SIMCOM_State = SIMCOM_SM_CheckNetwork; // Move to next state
				
					}
					else
					{
						if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
						{
							// Network Not Registered.
							// TODO: Throw Error including the Status returned by the SIM800 module
						}

						RetryInNextCycle = TRUE;
					}

				}
				else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
				{
					// If there is a problem in reception, retry sending the command
					RetryInNextCycle = TRUE;

					// TODO: Log Timeout Error.
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		
		case SIMCOM_SM_CheckNetwork:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+COPS?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
				{
					//Display_String("AT");
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
					if(IsSIMCOM_ResponseStartsWith("+COPS: "))
					{
						char *Netstr = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), "+COPS: ");
						APN_Selection(Netstr);
						SIMCOM_State = SIMCOM_SM_SelectNetwork;
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

		case SIMCOM_SM_SelectNetwork:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{

				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job(APN_name, SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
				{
					//Display_String("AT");
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
						SIMCOM_State = SIMCOM_SM_LTE_Check;
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
		
		case SIMCOM_SM_LTE_Check:
		{
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				if(SIMCOM_Schedule_Job("AT+CPSI?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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
					char * RxString = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), "+CPIN: ");
				
					if (*RxString)
					{
						if (memcmp("LTE,Online",RxString,10))
						{
											
							//do next job
							RetryInNextCycle = FALSE;
							SIMCOM_State = SIMCOM_PDP_context;
											
						}
						else
						{
							// If the returned value is ERROR or something else, then act accordingly
							//TODO: Later
							RetryInNextCycle = TRUE;
						}
					}
					else
					{
						// If the returned value is ERROR or something else, then act accordingly
						//TODO: Later
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
		
		case SIMCOM_PDP_context:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CGATT?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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

					ULONG PDP_Context = SIMCOM_GetCSV_Number_fromBuffer("+CGATT: ", 1);

					/* Accept both Roaming and Local Registration */
					if(PDP_Context == 1)
					{
						
						SIMCOM_State = SIMCOM_SM_Ready; // Move to next state		
					}
					else
					{
						if(SIMCOM_SM_Retry_Count == 0) // If this is the last attempt
						{
							// Network Not Registered.
							// TODO: Throw Error including the Status returned by the SIM800 module
						}

						RetryInNextCycle = TRUE;
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


		default:
		case SIMCOM_SM_Ready:
		case SIMCOM_SM_Error:
		{
			// Do Nothing, The state machine has been completed
		}
		break;
	}

	if(RetryInNextCycle == TRUE)
	{
		// If Retry is allowed
		if(SIMCOM_SM_Retry_Count != 0)
		{
			SIMCOM_SM_Retry_Count--; // Decrement the Retry Count

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
		
		SIMCOM_State = SIMCOM_SM_Error;

		/* Send Appropriate Error Code to Application */
		{
			SIMCOM_Error_State_EN ErrorState = SIMCOM_Error_Unknown;
			
			switch(SIMCOM_State)
			{
				case SIMCOM_SM_Init                      : ErrorState = SIMCOM_Error_GSM_Not_Connected; break;
				case SIMCOM_SM_SIM_Check                 : ErrorState = SIMCOM_Error_SIM_Card_NotInserted; break;
				case SIMCOM_SM_NW_Registration_Check     : ErrorState = SIMCOM_Error_NetworkNotRegistered; break;
				default:
					// Do Nothing, SIMCOM Module will timeout and report error
					break;
			}
			SIMCOM_ERROR_CALLBACK();
		}
	}

	/* Check if the state changed after execution */
	if(SIMCOM_State_Before_Execution != SIMCOM_State)
	{
		// If changed, the Set the New Job Result as Idle for the next state to proceed further
		SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

		SIMCOM_SM_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT; // Reset the Retry Count
	}
}
