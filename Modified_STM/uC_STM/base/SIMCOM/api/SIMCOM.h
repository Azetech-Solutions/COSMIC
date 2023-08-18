/*
 * SIMCOM.h
 *
 *  Created on: 08-Mar-2021
 *      Author: Hari
 */

#ifndef _SIMCOM_H_
#define _SIMCOM_H_
#include "Includes.h"
#include SIMCOM_CONFIG_H
#include STRINGHELPER_H
#include BUFFER_CONFIG_H
/* Include Sub Modules */
#include SIMCOM_CLOCK_H

/*************************/
/* Macro Definitions     */
/*************************/
#define ENTER_MESSAGE                  ">"
#define CARRIAGE_RETURN               0x0D
#define MSGLASTWORD                   0X1A


#define LINE_FEED                     0x0A

#define IsStartOrStopCommandByte(x)   ((x == CARRIAGE_RETURN) || (x == LINE_FEED))

#define SIMCOM_GetResponseLength()    SIMCOM_ResponseLength

#define SIMCOM_GetResponseBuffer()    SIMCOM_ResponseBuffer

#define SIMCOM_IsResponseReceived()   (SIMCOM_ResponseLength != 0)

#define IsSIMCOM_Module_Ready()       (SIMCOM_State == SIMCOM_SM_Ready)

#define IsSIMCOM_Module_Error()       (SIMCOM_State == SIMCOM_SM_Error)

#define SIMCOM_ReEvaluate_State()     (SIMCOM_State = SIMCOM_SM_Init)

#define IsSIMCOM_ResponseStartsWith(x)    StringHelper_startsWith(x, SIMCOM_GetResponseBuffer())

#define IsStringStartsWith(X)			StringSearch(BackupBuffer_Buffer_Get(),X)

#define IsSIMCOM_SubModule_Error()    0

#define IsSIMCOM_ReadyForApplication() (SIMCOM_IsClockRunning() && SIMCOM_IsBT_CheckedAtleastOnce())

#define IsSIMCOM_ClockReady()  (SIMCOM_Clock_State == SIMCOM_Clock_Running)



/*************************/
/* Data Type Definitions */
/*************************/

/* SIMCOM State Machine States Type */
typedef enum
{
	SIMCOM_SM_Init = 0,
	SIMCOM_ECHO_OFF,
	SIMCOM_SM_SIM_Check,
	SIMCOM_SM_Check_signal_strength,
	SIMCOM_SM_NW_Registration_Check,
	SIMCOM_PDP_context,
	SIMCOM_DisableCall,
	SIMCOM_DisableMsg,
	SIMCOM_DisableGPRS_URC,
	SIMCOM_SM_LTE_Check,
	SIMCOM_SM_Reset,
	SIMCOMCancelCall,
	WaitforSIMCOMReset,
	SIMCOMAttendCall,
	DialMobileNumber,
	SIMCOM_SM_CheckNetwork,
	SIMCOM_SM_SelectNetwork,
	SimcomWaitforCallAttendResponse,
	SimcomWaitforCallHangResponse,
	SetFullFunctionality,
	SetLTEmode,
	DTMFWaitState,
	SaveCurrentChanges,
	
	// Below states will not request any AT commands to the module, Will be used for SIMCOM Sub Modules
	SIMCOM_SM_Ready,                 // This state will reach with all the state has been covered
	SIMCOM_SM_Error                  // This state will reach when any of the states were failed

}SIMCOM_State_EN;

/* SIMCOM Communication States Type */
typedef enum
{
	SIMCOM_Idle = 0,
	SIMCOM_WriteInProgress,
	SIMCOM_ReadInProgress,
	SIMCOM_WaitingForResponse,
	SIMCOM_ReceptionCompleted

}SIMCOM_ComState_EN;

typedef enum
{	
	I_MQTT_Rx_Response_Idle,
	I_MQTT_Rx_OK,
	I_MQTT_Rx_ERROR,
	I_MQTT_Rx_msg,
	I_MQTT_Rx_Storedata,
	I_MQTT_Rx_CMQTTSUB,
	I_MQTT_Rx_CMQTTPUB
	
}Rx_Response_EN;

/* SIMCOM Job Callback type */
typedef void (*SIMCOM_Callback_Type)(SIMCOM_Job_Result_EN);


/* SIMCOM Job Scheduler Type */
typedef struct
{
	const char * Command;
	ULONG Timeout;
	SIMCOM_Callback_Type Callback;
	SIMCOM_Job_Result_EN State;
}SIMCOM_JobType;


/*************************/
/* Global Variables      */
/*************************/

extern SIMCOM_ComState_EN SIMCOM_ComState;

/* [[DEBUG_MEASUREMENT */
/* NAME=SIMCOM_ComState */
/* MODELNAME=SIMCOM_ComState */
/* SIZE=3 */
/* HEADER=SIMCOM_H */
/* DEBUG_MEASUREMENT]] */

extern SIMCOM_State_EN SIMCOM_State;

/* [[DEBUG_MEASUREMENT */
/* NAME=SIMCOM_State */
/* MODELNAME=SIMCOM_State */
/* SIZE=4 */
/* HEADER=SIMCOM_H */
/* DEBUG_MEASUREMENT]] */

extern char SIMCOM_ResponseBuffer[];

extern BufferLengthType SIMCOM_ResponseLength;

extern UBYTE SIMCOM_ReceptionIgnoreCommandCount;

extern UBYTE PublishStatus;

extern void  ComIf_TxConfirmation(UBYTE ChannelIndex);

extern unsigned short int MQTT_ConnectionCheckCounter;

/*************************/
/* Function Declarations */
/*************************/

static inline void SIMCOM_ClearResponseBuffer()
{
	memset(SIMCOM_ResponseBuffer, 0, BUFFER_MAX_SIZE);

	SIMCOM_ResponseLength = 0;
}

static inline void SIMCOMCurrentBufferClear(BufferType_ST *Buff)
{
	memset(Buff->BufferPtr,0,Buff->Length);
				
	Buff->HeadIndex = 0;
	Buff->TailIndex = 0;
	Buff->Length = 0;
}

static inline BOOL SIMCOM_IsResponseOK()
{
	BOOL retval = FALSE;

	if(SIMCOM_GetResponseLength() == 2)
	{
		if(strcmp(SIMCOM_ResponseBuffer, "OK") == 0)
		{
			retval = TRUE;
		}
	}

	return retval;
}

static inline BOOL SIMCOM_IsResponseError()
{
	BOOL retval = FALSE;

	if(SIMCOM_GetResponseLength() == 5)
	{
		if(strcmp(SIMCOM_ResponseBuffer, "ERROR") == 0)
		{
			retval = TRUE;
		}
	}

	return retval;
}

static inline BOOL SIMCOM_IsResponse_Entermessage()
{
	BOOL retval = FALSE;

	if(SIMCOM_GetResponseLength() == 1)
	{
		if(strcmp(SIMCOM_ResponseBuffer, ENTER_MESSAGE) == 0)
		{
			retval = TRUE;
		}
	}

	return retval;
}
/*************************/
/* Function Declarations */
/*************************/

extern void SIMCOM_Init(void);

extern void SIMCOM_MainFunction(void);

extern void SIMCOM_StateMachine(void);

extern void SIMCOM_SSL_CONFIG_MainFunction(void);

extern BOOL SIMCOM_Data_Read(UBYTE Data); // MUST ONLY USED BY UART ISR

extern BOOL SIMCOM_Schedule_Job(const char * Command, ULONG Timeout, SIMCOM_Callback_Type Callback);

extern BOOL SIMCOM_SSL_Schedule_Job(const char * Command, ULONG Timeout, SIMCOM_Callback_Type Callback);
//extern void SIMCOM_StateMachine_Callback(SIMCOM_Job_Result_EN result);

extern ULONG SIMCOM_GetCSV_Number_fromBuffer(const char * ResponseHead, UBYTE Position);

extern ULONG SIMCOM_Number_fromBuffer(const char * ResponseHead);

extern void SIMCOM_IgnoreCRLFs(UBYTE Count);

extern void MQTT_StateMachine(void);

extern void MQTT_SubPub_StateMachine(void);

extern void APN_Selection(char *checkstring);

#endif /* _SIMCOM_H_ */

