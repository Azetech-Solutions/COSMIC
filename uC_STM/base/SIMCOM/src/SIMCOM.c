/*
 * SIMCOM.c
 *
 *  Created on: 08-Mar-2021
 *      Author: Hari
 */
#include "Includes.h" // Will have all definitions of the Project Headers
#include <stdint.h>
#include MQTT_PUBLISH_H
#include MQTT_H
#include MQTT_APPLICATION_H
#include GPIO_DRIVER_H
#include PLATFORM_TYPES_H // For Data Types
#include SIMCOM_H
#include BUFFER_H
#include "stdio.h"
#include <string.h>
#include COMIF_CONFIG_H
#include GPIO_DRIVER_H
#include UART_DRIVER_H
#include FLASH_EEPROM_H
#include MESSAGE_H


/*****************************************/
/* Global Variables                      */
/*****************************************/



extern void EepromDeleteWrite(uint32_t number,UBYTE WrtInd[]);

uint32_t EEPROMWriteAdress;

//UBYTE pageEraseCount = 0;

extern void SendNumberMessage();

extern void updateSendData(UBYTE Data[]);

extern void DTMFMesaageHanldeFunc(char DtmfCmd);

extern UBYTE FlashDataRead(uint32_t Address);

extern void EepromFlashMmeoryCopy();

extern void SIM_Send_Data(unsigned char Data);

extern void EEPROMmain(uint32_t Address, uint64_t Data);

char TOPIC1_SubscribeMsg[26];

BOOL PREVPUBLISHSTATE = FALSE;

UBYTE DTMFBuffer[15];

extern void MessageControl(void);

extern void EEPROMErasePage(uint32_t page);

SIMCOM_ComState_EN SIMCOM_ComState = SIMCOM_Idle;

SIMCOM_JobType SIMCOM_CurrentJob;

UBYTE NumberLength = 0;

extern void SendNumberMessage();

char SIMCOM_ResponseBuffer[BUFFER_MAX_SIZE];

BufferLengthType SIMCOM_ResponseLength = 0;

static ULONG SIMCOM_IncompleteCounter = P_SIMCOM_INCOMPLETE_RESPONSE_TIMEOUT;

static ULONG SIMCOM_Aliveness_Counter = P_SIMCOM_ALIVENESS_ERROR_TIME;

UBYTE SIMCOM_ReceptionIgnoreCommandCount = 0;

UBYTE PublishStatus = 0;

//UBYTE ReadyforDtmfCmd = FALSE;

Rx_Response_EN Rx_Response_State = I_MQTT_Rx_Response_Idle;

void DTMFStateMachine(char DTMFMessage);

typedef enum DtmfNumberAlterStatus{
	Idle = 0,
	ChooseTaskToAlter,
	AddNumberToStore,
	ChooseAdressToAlterNumber,
	WaitforPageErase,
	DeleteExcistingNumber
}DtmfNumberAlterStatus_En;

char DTMFStoreNumber[13];
UBYTE BufferLength = 3;

DtmfNumberAlterStatus_En DtmfState = Idle;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SIMCOM_Send_Command()
{
	while(*(SIMCOM_CurrentJob.Command))
	{
		SIMCOM_SEND_BYTE(*(SIMCOM_CurrentJob.Command));

		SIMCOM_CurrentJob.Command++;
	}

	SIMCOM_SEND_BYTE(CARRIAGE_RETURN);
	SIMCOM_SEND_BYTE(LINE_FEED);
}

static void SIMCOM_Callback(SIMCOM_Job_Result_EN JobState)
{
	
	if(SIMCOM_CurrentJob.Callback != NULL_PTR)
	{
		SIMCOM_CurrentJob.State = JobState;
		SIMCOM_CurrentJob.Callback(JobState);
	}
	else
	{ 
		if(IsSIMCOM_ResponseStartsWith("*ATREADY"))
		{
			//do nothing
		}
		else if(IsSIMCOM_ResponseStartsWith("SMS DONE"))
		{
			//wait for pb done response
		}
		else if(IsSIMCOM_ResponseStartsWith("PB DONE"))
		{
			SIMCOM_ERROR_CALLBACK();
			SIMCOM_State = SIMCOM_SM_Init;
		}
		else if(IsSIMCOM_ResponseStartsWith("+CPIN READY"))
		{
			SIMCOM_ERROR_CALLBACK();
			SIMCOM_State = SIMCOM_SM_Init;
		}
		else if(SIMCOM_IsResponseOK())
		{
			if(SIMCOM_State == SimcomWaitforCallAttendResponse)
			{
				SIMCOM_State = SIMCOM_SM_Ready;
			}
		}
		else if(SIMCOM_IsResponseOK())
		{
			if(SIMCOM_State == SimcomWaitforCallHangResponse)
			{
				SIMCOM_State = SIMCOM_SM_Ready;
			}
		}
		else if(IsSIMCOM_ResponseStartsWith("+CMQTTCONNECT: "))
		{
			ULONG ConnectResponse1 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTCONNECT:", 1);
			ULONG ConnectResponse2 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTCONNECT:", 2);
			// Check if the response is OK or not.
			
			if((ConnectResponse1==0)&&(ConnectResponse2==0))
			{
				MQTT_State = MQTT_SubscribeTopic_Config;// Move to next state
			}
			else
			{
				SIMCOM_ERROR_CALLBACK();
				SIMCOM_State = SIMCOM_SM_Reset;
			}
		}
//		else if(SendMSG_State == WaitforMessageResponse)
//		{
//			if(IsSIMCOM_ResponseStartsWith(">"))
//			{
//				SendMSG_State = MSG_SendMsg;
//			}
//		}
		else if(IsSIMCOM_ResponseStartsWith("+CMQTTPUB:"))
		{
			UBYTE PublishResponse1 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTPUB:", 1);
			UBYTE PublishResponse2 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTPUB:", 2);
			// Check if the response is OK or not.
			
			if((PublishResponse1==0)&&(PublishResponse2==0))
			{
				PublishStatus = 1;
				Publish_State = MQTT_PublishMsgPublished;
			}
			else
			{
				PublishStatus = 0;
			}
		}
		else if(IsSIMCOM_ResponseStartsWith("+CMQTTSUB: "))
		{
			ULONG SubscribeResponse1 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSUB: ", 1);
			ULONG SubscribeResponse2 = SIMCOM_GetCSV_Number_fromBuffer("+CMQTTSUB: ", 2);
			// Check if the response is OK or not.
			if((SubscribeResponse1==0) && (SubscribeResponse2==0))
			{	
				MQTT_State = MQTT_Ready;
			} 
			else
			{
				SIMCOM_ERROR_CALLBACK();
			}
		}			
		else if(IsSIMCOM_ResponseStartsWith("+CMQTTCONNLOST:"))
		{
			SIMCOM_ERROR_CALLBACK();
		}
		else if(IsSIMCOM_ResponseStartsWith("+CLCC:"))
		{
			UBYTE DtmfStatusFlag = FlashDataRead(0x08007000);
			UBYTE CallStatus;
			UBYTE BufferLength = 0,i;
			char *RXStr = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(),"+CLCC:");
			if(RXStr[5] == '4')
			{
				for(i = 12;i<=24; i++)
				{
					DTMFBuffer[BufferLength] = RXStr[i];
					BufferLength++;
				}
				UBYTE BuffDiff;
				if(DtmfStatusFlag == 1)
				{
					BuffDiff = memcmp(DTMFBuffer,"+918124922783",13);
//					BuffDiff = memcmp(DTMFBuffer,readD.MobNo,13);
				}
				else
				{
					BuffDiff = memcmp(DTMFBuffer,"+918124922783",13);
				}
				if(BuffDiff == 0)
				{
					SIMCOM_State = SIMCOMAttendCall;
				}
				else
				{
					SIMCOM_State = SIMCOMCancelCall;
				}
			}
		}
		else if(IsSIMCOM_ResponseStartsWith("+RXDTMF"))
		{
			char DTMF;
			char *RXStr = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(),"+RXDTMF: ");
			DTMF = *RXStr;
			DTMFStateMachine(DTMF);
		}
		else
		{
			// If something else is received, then give a call to the Application layer to handle
			SIMCOM_GENERIC_CALLBACK(JobState);
		}
	}
}

void DTMFStateMachine(char DTMFMessage)
{
	switch(DtmfState)
	{
		AvrCmdData_ST StatusData;		
		case Idle:
			if(DTMFMessage == '1')
			{
				StatusData.SW1 = 1;
				updateSendData(StatusData.Data_Bytes);
				SIMCOM_State = SIMCOMCancelCall;	
			}
			else if(DTMFMessage == '2')
			{
				StatusData.SW1 = 1;
				updateSendData(StatusData.Data_Bytes);
				SIMCOM_State = SIMCOMCancelCall;
			}
			else if(DTMFMessage == '3')
			{
				SIMCOM_State = SIMCOMCancelCall;
			}
			else if(DTMFMessage == '4')
			{
				SendNumberMesaageFlag = TRUE;
			}
			else if(DTMFMessage == '5')
			{
				DtmfState = ChooseAdressToAlterNumber;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;				
			}
			break;
		case ChooseAdressToAlterNumber:
			if(DTMFMessage == '1')
			{
				EEPROMWriteAdress = number1;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '2')
			{
				EEPROMWriteAdress = number2;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '3')
			{
				EEPROMWriteAdress = number3;
				DtmfState = ChooseTaskToAlter;
			}
			else if(DTMFMessage == '4')
			{
				EEPROMWriteAdress = number4;
				DtmfState = ChooseTaskToAlter;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
		case ChooseTaskToAlter:
			if(DTMFMessage == '*')
			{
				DtmfState = AddNumberToStore;
			}
			else if(DTMFMessage == '#')
			{
				DtmfState = DeleteExcistingNumber;
			}
			else
			{
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
			
		case AddNumberToStore:
		{
			Data.MobNo[0] = '+';
			Data.MobNo[1] = '9';
			Data.MobNo[2] = '1';
			Data.MobNo[BufferLength] = DTMFMessage;
			BufferLength++;
			if(BufferLength == 13)
			{
				Data.WriteIndicator = 1;
				BufferLength = 3;
				DtmfState = Idle;
				EEPROMmain(EEPROMWriteAdress,Data.byte[0]);
				EEPROMWriteAdress = EEPROMWriteAdress+8;
				EEPROMmain(EEPROMWriteAdress,Data.byte[1]);
				SIMCOM_State = SIMCOMCancelCall;
			}
		}	
			break;		
		case DeleteExcistingNumber:
			if(DTMFMessage == '*')
			{
				EEPROMErasePage(14);
				DtmfState = Idle;
				SIMCOM_State = SIMCOMCancelCall;
			}
			else
			{
				UBYTE WrtInd[4];
				uint32_t EEPROMAdress = 0x08007000;
				for(UBYTE i = 0;i < 4; i++)
				{
					WrtInd[i] = (FlashDataRead(EEPROMAdress));
					SIM_Send_Data(WrtInd[i]);
					EEPROMAdress = EEPROMAdress+16;
				}
				EepromDeleteWrite(EEPROMWriteAdress,WrtInd);
				EepromFlashMmeoryCopy();
				SIMCOM_State = SIMCOMCancelCall;
				DtmfState = Idle;
			}
			break;
//		case WaitforPageErase:
//			pageEraseCount++;
//			if(pageEraseCount == 5)
//			{
//				pageEraseCount = 0;
//				SIMCOM_State = SIMCOMCancelCall;
//				DtmfState = Idle;
//			}
//			break;
		default :
			break;
	}
}


static void SIMCOM_UpdateCurrentJobResponse()
{
	SIMCOM_ClearResponseBuffer();
	SIMCOM_ResponseLength = SIMCOM_GET_BUFFER_LENGTH();
	UBYTE i;
	for(i = 0; i < SIMCOM_ResponseLength; i++)
	{
		SIMCOM_ResponseBuffer[i] = SIMCOM_BUFFER_GET_BYTE();
	}
}

static char * SIMCOM_GetResponseWithoutHead_fromBuffer(const char * ResponseHead)
{
	// Replace the Response head with empty string and send back
	char * retval = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), (char *)ResponseHead);

	return retval;
}


static unsigned int SIMCOM_GetSeparated_String_fromBuffer(const char * ResponseHead, char Separator, UBYTE Position)
{
	char * responseData = SIMCOM_GetResponseWithoutHead_fromBuffer(ResponseHead);

	return StringHelper_SplitAndGet(responseData, Separator, Position);
}

static void SIMCOM_ResetJob(void)
{
	SIMCOM_ComState = SIMCOM_Idle; // Go back to Idle state

	/* Clear the Job */
	SIMCOM_CurrentJob.Command = "";
	SIMCOM_CurrentJob.Timeout = 0;
	SIMCOM_CurrentJob.Callback = NULL_PTR;
	SIMCOM_CurrentJob.State = SIMCOM_Job_Idle;

	SIMCOM_ReceptionIgnoreCommandCount = 0;
}

/*****************************************/
/* Function Definitions                  */
/*****************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                      SIMCOM Init Function                      */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SIMCOM_Init(void)
{
	/* Initialize the Global Variables */
	SIMCOM_ResetJob();

	/* Clear Response Buffer */
	SIMCOM_ClearResponseBuffer();

	/* Initialize the Buffer for SIMCOM */
	if(SIMCOM_BUFFER_INITIALIZE() == FALSE)
	{
		// Unable to Create Buffer
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                      SIMCOM Main Function                      */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void SIMCOM_MainFunction(void)
{
	switch(SIMCOM_ComState)
	{
		default:
		case SIMCOM_Idle:
		{
			SIMCOM_ResetJob();

			// Check for aliveness
			if(SIMCOM_Aliveness_Counter > 0)
			{
				SIMCOM_Aliveness_Counter -= P_SIMCOM_TASK_CYCLE_FACTOR;
			}
			else
			{
				SIMCOM_ERROR_CALLBACK(); // Report Error
				SIMCOM_Aliveness_Counter = P_SIMCOM_ALIVENESS_ERROR_TIME; // Reload Timer
			}
		}
		break;

		case SIMCOM_WriteInProgress:
		{
			// Do Nothing
			SIMCOM_Callback(SIMCOM_Job_InProgress);
		}
		break;

		case SIMCOM_ReadInProgress:
		{
			// If the Read is in progress for one Task Cycle, then give a callback that the SIMCOM is in Progress
			SIMCOM_Callback(SIMCOM_Job_InProgress);

			if(SIMCOM_IncompleteCounter != 0)
			{
				SIMCOM_IncompleteCounter--;
			}
			else
			{

				// Update the Response to the SIMCOM Response Buffer even if it is incomplete, the caller will take care of the received response
				SIMCOM_UpdateCurrentJobResponse();

				// If the Read is in progress for long time, then there might be a problem with the reception.
				SIMCOM_Callback(SIMCOM_Job_Incomplete);

				SIMCOM_ResetJob();
			}
		}
		break;

		case SIMCOM_WaitingForResponse:
		{
			// If waiting for response, then decrement the counter
			if(SIMCOM_CurrentJob.Timeout != 0)
			{
				SIMCOM_CurrentJob.Timeout--;

				SIMCOM_Callback(SIMCOM_Job_InProgress);
			}
			else
			{

				// If the response has timed out, then give a callback stating error
				SIMCOM_Callback(SIMCOM_Job_Timeout);

				SIMCOM_ResetJob();
			}
		}
		break;

		case SIMCOM_ReceptionCompleted:
		{
			// Update the Response to the SIMCOM Response Buffer
			SIMCOM_UpdateCurrentJobResponse();

			// Once the reception is completed, then give a callback to read the data
			SIMCOM_Callback(SIMCOM_Job_Completed);

			SIMCOM_ResetJob();
		}
	}

	if(SIMCOM_ComState != SIMCOM_Idle)
	{
		SIMCOM_Aliveness_Counter = P_SIMCOM_ALIVENESS_ERROR_TIME; // Reload Timer
	}

	/* Call the Main Functions of the SIMCOM Sub Modules */
	SIMCOM_StateMachine();
	SIMCOM_SSL_CONFIG_MainFunction();
	MQTT_StateMachine();
//	MQTT_AppMain();
//	MQTT_Publish_StateMachine();
	SendNumberMessage();
	MessageControl();
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                      SIMCOM Schedule Job                       */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL SIMCOM_Schedule_Job(const char * Command, ULONG Timeout, SIMCOM_Callback_Type Callback)
{
	BOOL retval = FALSE;

	if(SIMCOM_ComState == SIMCOM_Idle)
	{
		SIMCOM_ComState = SIMCOM_WriteInProgress; // Will act as a Mutex

		SIMCOM_CurrentJob.Command = Command;
		SIMCOM_CurrentJob.Timeout = (ULONG)(Timeout / P_SIMCOM_TASK_CYCLE_FACTOR);
		SIMCOM_CurrentJob.Callback = Callback;
		SIMCOM_CurrentJob.State = SIMCOM_Job_Idle;

		/* Clear the Timer */
		SIMCOM_IncompleteCounter = P_SIMCOM_INCOMPLETE_RESPONSE_TIMEOUT;

		SIMCOM_Aliveness_Counter = P_SIMCOM_ALIVENESS_ERROR_TIME;

		SIMCOM_ReceptionIgnoreCommandCount = 0;

		/* Clear Response Buffer */
		SIMCOM_ClearResponseBuffer();

		SIMCOM_Send_Command();

		SIMCOM_ComState = SIMCOM_WaitingForResponse;

		retval = TRUE;
	}

	return retval;
}

void SIMCOM_RxMessageCallBack()
{
	BufferType_ST *Buff = SIMCOM_Buffer_Get();
	
	if(memcmp((char*)Buff->BufferPtr,"+CMQTTRXSTART:",14)==0) 
	{
		Rx_Response_State = I_MQTT_Rx_msg;
	}
	
	if(memcmp((char*)Buff->BufferPtr,"RING",4)==0)
	{
		SIMCOM_Buffer_Clear();
	}
	
	if(Buff->BufferPtr == NULL)
	{
		return;
	}
	
	if(PREVPUBLISHSTATE == TRUE)
	{
		if(memcmp((char*)Buff->BufferPtr,">",1)==0)
		{
			Publish_State = MQTT_PubTopic_Name_Config;
			PREVPUBLISHSTATE = FALSE;
		}
	}
	
	switch(Rx_Response_State)
	{
		case I_MQTT_Rx_Response_Idle:
		{
			
		}
		break;
		case I_MQTT_Rx_msg:
		{
			if(memcmp((char*)Buff->BufferPtr,"prj01/cp0001/sub",12)==0)
			{
				Rx_Response_State = I_MQTT_Rx_Storedata;
			}
			
			memset(Buff->BufferPtr,0,Buff->Length);
			
			Buff->HeadIndex = 0;
			Buff->TailIndex = 0;
			Buff->Length = 0;
		}
		break;
		case I_MQTT_Rx_Storedata:
		{ 
			if ((Buff->BufferPtr != NULL) && (Buff != NULL) &&  (Buff->Length == 26))
			{				
				if (StringHelper_startsWith("7;",(char*)Buff->BufferPtr))
				{
					Rx_Response_State = I_MQTT_Rx_Response_Idle;
					
					memcpy(TOPIC1_SubscribeMsg,Buff->BufferPtr,Buff->Length);//resetting part
					
					MQTTApp_State = MQTTApp_ComifRxindication;
				}	
			}
			
			memset(Buff->BufferPtr,0,Buff->Length);
			Buff->HeadIndex = 0;
			Buff->TailIndex = 0;
			Buff->Length = 0;
		}
		break;
		default:
		{
			break;
		}
	}
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                   SIMCOM Data Read Function                    */
/*                                                                */
/* To be called by the USART ISR function                         */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

BOOL SIMCOM_Data_Read(UBYTE Data)
{
	static UBYTE prevData = 0;
	
	BOOL retval = FALSE;

	BOOL proceedStoring = FALSE;

	if(IsStartOrStopCommandByte(Data))
	{
		if((prevData == CARRIAGE_RETURN) && (Data == LINE_FEED))
		{
			// Either Start or Stop command is received.
			if(SIMCOM_ComState == SIMCOM_WaitingForResponse)
			{
				/* A Command has already been sent, and Software is waiting for a response */
				SIMCOM_ComState = SIMCOM_ReadInProgress;
				retval = TRUE;
			}
			else if(SIMCOM_ComState == SIMCOM_ReadInProgress)
			{
				/* A response has already started, then complete it */
				if(SIMCOM_ReceptionIgnoreCommandCount == 0)
				{
					SIMCOM_RxMessageCallBack();
					
					if (!SIMCOM_IsBufferEmpty())
					{
						SIMCOM_ComState = SIMCOM_ReceptionCompleted;
					}
				}
				else
				{
					// If it is an expected command, then store it
					SIMCOM_ReceptionIgnoreCommandCount--;

					(void)SIMCOM_BUFFER_PUT_BYTE('~'); // Use this for easier parsing
				}

				retval = TRUE;
			}
			else if(SIMCOM_ComState == SIMCOM_Idle)
			{
				/* A New data stream is received without any transfer or request from the uC */
				SIMCOM_ComState = SIMCOM_ReadInProgress;
				retval = TRUE;
			}
			else
			{
				/* Previously received data is not yet processed or in rare cases a data might be received during writing command */
				// TODO: Log Error.
			}
		}
		else
		{
			// If it is a line feed or Carriage return, do not store. Simply send true
			retval = TRUE;
		}
	}
	else
	{
		if(SIMCOM_ComState == SIMCOM_ReadInProgress)
		{
			proceedStoring = TRUE;
		}
	}

	/* If Okay to store in buffer */
	if(proceedStoring == TRUE)
	{	
		retval = SIMCOM_BUFFER_PUT_BYTE(Data);
	}

	prevData = Data; // Store for later processing
	
	return retval;
}


void SIMCOM_IgnoreCRLFs(UBYTE Count)
{
	if(SIMCOM_ComState == SIMCOM_WaitingForResponse)
	{
		SIMCOM_ReceptionIgnoreCommandCount = Count;

		// If there are multiple items to be received, then set the Incomplete timeout as the Job Timeout
		SIMCOM_IncompleteCounter = SIMCOM_CurrentJob.Timeout;
	}
	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                     SIMCOM Helper Functions                    */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ULONG SIMCOM_GetCSV_Number_fromBuffer(const char * ResponseHead, UBYTE Position)
{
	ULONG retval = 0;

	if(SIMCOM_GetSeparated_String_fromBuffer(ResponseHead, ',', Position) > 0)
	{
		char NumberString[12];

		StringHelper_GetString(NumberString);

		retval = atoi(NumberString);
	}

	return retval;
}

ULONG SIMCOM_Number_fromBuffer(const char * ResponseHead)
{
	ULONG retval = 0;

	char * result = SIMCOM_GetResponseWithoutHead_fromBuffer(ResponseHead);

	if(strlen(result) > 0)
	{
		retval = atoi(result);
	}

	return retval;
}

uint64_t StringToUlong(UBYTE str[])
{
	uint64_t Data = 0;
	UBYTE i;
	for(i = 0; i<8; i++)
	{
		Data = Data | (str[i] << 8*i);
	}
	return Data;
}



