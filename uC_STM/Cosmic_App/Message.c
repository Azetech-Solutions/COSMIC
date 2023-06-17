/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include "Message.h"
#include SIMCOM_H
#include "stdio.h"
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/


/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/
SendMSG_EN SendMSG_State = MSG_Idle;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SendMSG_Retry_Count = 50;

char StoreMSGs[100];
char MobNumber[13]="+917558023123";
/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/


/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SendMSG_CallBack(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}


/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

void AddDoubleQts(char *dest,const char *str)
{
		dest[0] = '\"';
		UBYTE len = strlen(str);
		memcpy(&dest[1],str,len);
		dest[len++] = '\"';
		dest[len] = '\0';
}


void SendMessage(const char* str)
{
	memcpy(StoreMSGs,str,strlen(str));
	SendMSG_State = MSG_SelectMobNum;
}


void MessageControl(void)
{
	SendMSG_EN SendMSG_State_Before_Execution = SendMSG_State;

	BOOL RetryInNextCycle = FALSE;

		
	switch(SendMSG_State)
	{
		case MSG_Idle:
		{
			
		}
		break;
		case MSG_DeleteMsgs:
		{
				// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
				if(SIMCOM_Schedule_Job("AT+CMGD=1,4", SIMCOM_DEFAULT_TIMEOUT, SendMSG_CallBack) == TRUE)
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
						SendMSG_State = MSG_SelectMsgFormat; // Move to next state
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
		case MSG_SelectMobNum:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				char SetMobileNumber[25];
				char StoreDoubleQtedNum[16];
				AddDoubleQts(StoreDoubleQtedNum,MobNumber);
				sprintf(SetMobileNumber,"AT+CMGS=%s",StoreDoubleQtedNum);
				
				// Send AT Command and wait for response	
				if(SIMCOM_Schedule_Job(SetMobileNumber, SIMCOM_DEFAULT_TIMEOUT, SendMSG_CallBack) == TRUE)
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
						SendMSG_State = MSG_SendMsg; // Move to next state
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
		case MSG_SendMsg:
		{
			// First Ensure the SIMCOM Module is Connected
			if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
			{
				// Send AT Command and wait for response
					
				if(SIMCOM_Schedule_Job(StoreMSGs, SIMCOM_DEFAULT_TIMEOUT, SendMSG_CallBack) == TRUE)
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
						SendMSG_State = MSG_Idle; // Move to next state
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
		{
			// Do Nothing, The state machine has been completed
					break;
		}

	}

	if(RetryInNextCycle == TRUE)
	{
		// If Retry is allowed
		if(SendMSG_Retry_Count != 0)
		{
			SendMSG_Retry_Count--; // Decrement the Retry Count

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

	if(SendMSG_State_Before_Execution != SendMSG_State)
	{
		// If changed, the Set the New Job Result as Idle for the next state to proceed further
		SIMCOM_Job_Result = SIMCOM_Job_Idle; // Reset the Job state so that next command will be sent

/*		MQTT_Publish_Retry_Count = 10; // Reset the Retry Count*/
	}
	
}