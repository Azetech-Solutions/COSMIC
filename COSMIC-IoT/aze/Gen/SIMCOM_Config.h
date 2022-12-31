/******************************************************************
                A Z E T E C H    S O L U T I O N S                 
-------------------------------------------------------------------
        T O O L   G E N E R A T E D   S O U R C E   C O D E        
                    D O   N O T   M O D I F Y                      
-------------------------------------------------------------------
 This file is generated with the Azetech copyrighted software tool 
 No modification should be done in this file without a written     
 consent from Azetech Solutions. Incase of any such instances,     
 Azetech Solutions will not be liable to any damages it cause.     
-------------------------------------------------------------------
 Any Distribution, Alteration, Removal, Disassemble, Decompile or  
 or Reverse Engineer the given software, without written consent   
 from Azetech Solutions by the Licensee or third parties were not  
 allowed. Such instance is considered as a breach and the Licensee 
 is the sole responsible and liable to any damages it cause.       
******************************************************************/

#ifndef __SIMCOM_CONFIG_H__
#define __SIMCOM_CONFIG_H__

#include <Includes.h>
#include PLATFORM_TYPES_H
#include BUFFER_H

/*************************************************/
/* Functional Switches and Parameter Definitions */
/*************************************************/

#define P_SIMCOM_INCOMPLETE_RESPONSE_TIMEOUT         10

#define P_SIMCOM_DEFAULT_FAILURE_RETRY_COUNT         10

#define P_SIMCOM_TASK_CYCLE_FACTOR                   100

#define P_SIMCOM_ALIVENESS_ERROR_TIME                60000

#define SIMCOM_DEFAULT_TIMEOUT                       2000


/*---------------------------------------*/
/*    SIMCOM Network Control Related     */
/*---------------------------------------*/

#define SIMCOM_CLOCK_SYNC_COUNT                      10

/*---------------------------------------*/
/*   SIMCOM Calls and Dialing Related    */
/*---------------------------------------*/

/* SIMCOM Calls and Dialing Feature is Disabled */
#define S_SIMCOM_ENABLE_DIALING_FEATURE              OFF

/*---------------------------------------*/
/*           SIMCOM HTTP Related         */
/*---------------------------------------*/

/* SIMCOM HTTP Feature is Disabled */


/*************************************************/
/*               Macro Definitions               */
/*************************************************/

#define SIMCOM_SEND_BYTE                             SIM_Send_Data

#define SIMCOM_GENERIC_CALLBACK                      COSMIC_Generic_SIMCOM_Callback

#define SIMCOM_ERROR_CALLBACK                        COSMIC_SIMCOM_Error_Callback

//#define SIMCOM_RX_INDICATION                         

//#define SIMCOM_TX_CONFIRMATION                       


/*---------------------------------------*/
/*        SIMCOM Buffer Related          */
/*---------------------------------------*/

#define SIMCOM_BUFFER_INITIALIZE()                   SIMCOM_Buffer_Create()

#define SIMCOM_GET_BUFFER_LENGTH()                   SIMCOM_Buffer_Length()

#define SIMCOM_BUFFER_PUT_BYTE(x)                    SIMCOM_Buffer_EnQueue(x)

#define SIMCOM_BUFFER_GET_BYTE()                     SIMCOM_GetResponseByte()

/*************************************************/
/*               Type Definitions                */
/*************************************************/

/* SIMCOM Job Result State Type */
typedef enum
{
	SIMCOM_Job_Idle = 0,    // Default State
	SIMCOM_Job_Scheduled,   // ---> This will be updated by the Client who schedules the Job.
	SIMCOM_Job_InProgress,  // When the Job is In-Progress
	SIMCOM_Job_Timeout,     // Response Timed out
	SIMCOM_Job_Incomplete,  // Half of the Response received, but timed out later
	SIMCOM_Job_Completed,   // Job completed successfully, Response is available in the Buffer
	SIMCOM_Job_Aborted      // Job aborted because of multiple failure attempts

}SIMCOM_Job_Result_EN;


/* SIMCOM Error State Type */
typedef enum
{
	SIMCOM_Error_Inactivity = 0,
	SIMCOM_Error_Unknown,
	SIMCOM_Error_GSM_Not_Connected,
	SIMCOM_Error_SIM_Card_NotInserted,
	SIMCOM_Error_NetworkNotRegistered,
	SIMCOM_Error_ClockConfigurationDisabled,
	SIMCOM_Error_Clock,
	SIMCOM_Error_GPRS,
	SIMCOM_Error_HTTP,
	SIMCOM_Error_POST,
	SIMCOM_Error_Buffer_Empty,

}SIMCOM_Error_State_EN;

/*************************************************/
/*               Global Variables                */
/*************************************************/


/*************************************************/
/*          Extern Function Declarations         */
/*************************************************/

extern void SIM_Send_Data(UBYTE Data);

extern void COSMIC_Generic_SIMCOM_Callback(SIMCOM_Job_Result_EN JobState);

extern void COSMIC_SIMCOM_Error_Callback(SIMCOM_Error_State_EN Error);

//extern UBYTE (char * Message);

//extern void (void);


/*************************************************/
/*           Inline Function Definitions         */
/*************************************************/

static inline UBYTE SIMCOM_GetResponseByte()
{
	UBYTE Data = 0;

	if(SIMCOM_Buffer_DeQueue(&Data) == FALSE)
	{
		// Buffer is Empty. When the Buffer is empty, the SIMCOM Module should've not called.
		// This is a development Error. So report Error to the application
		SIMCOM_ERROR_CALLBACK(SIMCOM_Error_Buffer_Empty);
	}
	return Data;
}


#endif /* __SIMCOM_CONFIG_H__ */

