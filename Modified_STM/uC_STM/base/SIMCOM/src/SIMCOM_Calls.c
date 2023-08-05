#include "SIMCOM_Calls.h"
#include "Includes.h"
#include SIMCOM_H
#include "stdio.h"
#include DTMF_APP_H
#include UART_DRIVER_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

SIMCOM_Dial_Request_EN SIMCOM_Dial_Request = SMC_Idle;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SIMCOM_SM_Calls_Retry_Count = P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT;

char DialNumer[20];

ULONG WaitForCallTimoutCounter = 0;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static BOOL SIMCOM_Calls_IsValidNumber(const char * Number)
{
	BOOL retval = FALSE;

	if(strlen(Number) <= 13) // Example +917200033312 / 7200033312
	{
		retval = TRUE;

		// TODO: Check if the number is valid, no special characters involved etc.
	}

	return retval;
}


/*****************************************/
/* Function Definitions                  */
/*****************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                 SIMCOM Calls Callback Function                 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void SIMCOM_Calls_Callback(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response
	// Check for the last requested command and then check the result

	switch(SIMCOM_Dial_Request)
	{
		default:
		case SMC_Idle:
		{
			// If nothing was requested, then possibly the SIMCOM might have received something that is relevant for the Calls feature
		}
		break;

		case SMC_DialNumber:
		{
			switch(result)
			{
				case SIMCOM_Job_Completed:
				{
					
				}
				break;

				case SIMCOM_Job_InProgress:
				{
					// Wait
				}
				break;

				default:
				case SIMCOM_Job_Idle:
				case SIMCOM_Job_Timeout:
				case SIMCOM_Job_Incomplete:
				{
					// Error
				}
				break;
			}
		}
		break;
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                    SIMCOM Calls Dial to Number                 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL SIMCOM_Calls_Dial(char * Number)
{
	BOOL retval = FALSE;

	/* Sanity Checks */
	if(SIMCOM_Calls_IsValidNumber(Number)) // Example +917200033312 / 7200033312
	{

		
	}
	char Command[20];
//	SIM_Send_Data('k');
	sprintf(DialNumer,"ATD%s;",Number);
	DialNumer[15] = '\0';
//		SIMCOM_Dial_Request = WaitForUpdateCallResponse;
	SIMCOM_Dial_Request = SMC_DialNumber;

	return retval;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                    SIMCOM Calls Main Function                  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SIMCOM_Calls_MainFunction(void)
{
	SIMCOM_State_EN SIMCOM_State_Before_Execution = SIMCOM_SM_Reset;
	
	BOOL RetryInNextCycle = FALSE;
	
	switch(SIMCOM_Dial_Request)
	{
		case SMC_Idle:
		{
			// Do Nothing
		}
		break;

		case WaitForUpdateCallResponse:
		{
			if(CheckReadyForDtmf())
			{
				SIMCOM_Dial_Request = SMC_DialNumber;
			}
		}
		break;
		case SMC_DialNumber:
		{
						// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job(DialNumer, SIMCOM_DEFAULT_TIMEOUT, SIMCOM_Calls_Callback) == TRUE)
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
						SIMCOM_Dial_Request = SMC_WaitForCallResponses;
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
		case SMC_AttendCalls:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT+CRESET Command and wait for response
				if(SIMCOM_Schedule_Job("ATA", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_Calls_Callback) == TRUE)
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
					SIMCOM_Dial_Request = SMC_WaitForCallResponses;
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		case SMC_DisConnectCalls:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT+CRESET Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CHUP", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_Calls_Callback) == TRUE)
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
					SIMCOM_Dial_Request = SMC_Idle;
				}
				else
				{
					// Do Nothing. Wait
				}
			}
		}
		break;
		case SMC_WaitForCallResponses:
		{
			WaitForCallTimoutCounter++;
			
			if(WaitForCallTimoutCounter == 250)
			{
				WaitForCallTimoutCounter = 0;
				SIMCOM_Dial_Request = SMC_DisConnectCalls;
			}
		}
		break;
		default:
		{
			
		}
		break;
		
		if(RetryInNextCycle == TRUE)
		{
			// If Retry is allowed
			if(SIMCOM_SM_Calls_Retry_Count != 0)
			{
				SIMCOM_SM_Calls_Retry_Count--; // Decrement the Retry Count

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
			SIMCOM_ERROR_CALLBACK();
		}
	}
}

