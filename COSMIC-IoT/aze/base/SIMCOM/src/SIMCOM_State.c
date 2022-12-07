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

/*****************************************/
/* Global Variables                      */
/*****************************************/

SIMCOM_State_EN SIMCOM_State = SIMCOM_SM_Init;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SIMCOM_SM_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SIMCOM_StateMachine_Callback(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
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
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CSMINS?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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

					// The positive response would be -> +CSMINS: <n>,<SIM inserted>

					ULONG InsertionStatus = SIMCOM_GetCSV_Number_fromBuffer("+CSMINS: ", 2);

					if(InsertionStatus == 1)
					{
						SIMCOM_State = SIMCOM_SM_NW_Registration_Check; // Proceed checking for the Network Registration Check
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

					// TODO: Log Timeout Error.
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
						SIMCOM_State = SIMCOM_SM_Clock_Configuration_Check; // Move to next state
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


		case SIMCOM_SM_Clock_Configuration_Check:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CLTS?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_StateMachine_Callback) == TRUE)
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

					ULONG Status = SIMCOM_Number_fromBuffer("+CLTS: ");

					if(Status == 1)
					{
						// Transition to Ready only when the Automatic Network Time is Enabled.
						// This is because, the time is used to sync between the app, both in BT and GPRS mode.

						SIMCOM_State = SIMCOM_SM_Ready;
					}
					else
					{
						// TODO: If the Automatic Clock configuration is not done, then do it now or log failure

						// For now, Abort the Job and do not start application
						SIMCOM_Job_Result = SIMCOM_Job_Aborted;
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
				case SIMCOM_SM_Clock_Configuration_Check : ErrorState = SIMCOM_Error_ClockConfigurationDisabled; break;
				default:
					// Do Nothing, SIMCOM Module will timeout and report error
					break;
			}

			SIMCOM_ERROR_CALLBACK(ErrorState);
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
