/***************************************************/
/* Header Inclusions                               */
/***************************************************/

#include "Includes.h"
#include SIMCOM_MESSAGE_H
#include SIMCOM_H
#include "stdio.h"
#include PLATFORM_TYPES_H
#include "string.h"
#include UART_DRIVER_H
#include FLASH_EEPROM_H
#include MESSAGE_APP_H
#include EEPROMWRAPPER_H
#include SIMCOM_CALLS_H
#include DTMF_APP_H
#include AVR_H

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/
extern void USART1_String(const char* data);

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/

SendMSG_EN SendMSG_State = MSG_Idle;

char StoreMSGs[100];

char MobNumber[10];

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SendMSG_Retry_Count = 50;


/***************************************************/
/* Function Declarations                           */
/***************************************************/

void SendMessage(const char* str);

void updateNumbertoSendMsg(char MessageString[],UBYTE NumberIndex);

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


/***************************************************/
/* Function Definitions                            */
/***************************************************/

void AddDoubleQts(char *dest,const char *str)
{
	
}

void Send_TextMessage(char* str,UBYTE Index)
{
	memset(StoreMSGs,'\0',100);
	memset(MobNumber,'\0',10);
	
	UBYTE Len = strlen(str);
	
	str[Len] = CARRIAGE_RETURN;
	
	str[Len+1] = MSGLASTWORD;
	str[Len+2] = '\0';
	
	Mobile_Numbers_ST *MN = &StoredMNs[Index];
	
	memcpy(StoreMSGs,str,strlen(str));
	
	memcpy(MobNumber,&MN->MobNo[3],10);
	
	SendMSG_State = MSG_SelectMobNum;
	
	//USART1_String(MobNumber);
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
						SendMSG_State = MSG_Idle;
						
						memset(StoreMSGs,'\0',100);
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
				char SetMobileNumber[26];
				char StoreDoubleQtedNum[16];
				memset(SetMobileNumber,'\0',26);
				USART1_String(MobNumber);
				sprintf(SetMobileNumber,"AT+CMGS=\"%s\"",MobNumber);
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
					if(IsSIMCOM_ResponseStartsWith("> "))
					{
						SendMSG_State = MSG_SendMsg;// Move to next state
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
		
		case WaitforMessageResponse:
			// do nothing 
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
					if(IsSIMCOM_ResponseStartsWith("+CMGS:"))
					{
//						SendMSG_State = MSG_DeleteMsgs; // Move to next state
						SendMSG_State = MSG_DeleteMsgs;
					}
					else
					{
						// If the returned value is ERROR or something else, then act accordingly
						// TODO: Later
						SendMSG_State = MSG_Idle;
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
					
		}
		break;
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
