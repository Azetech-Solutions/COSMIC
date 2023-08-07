/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include SIMCOM_H
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

typedef enum
{
	SMC_Idle = 0,
	WaitForUpdateCallResponse,
	SMC_DialNumber,
	SMC_AttendCalls,
	SMC_DisConnectCalls,
	SMC_WaitForCallResponses
	
}SIMCOM_Dial_Request_EN;
	

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/
extern BOOL SIMCOM_Calls_Dial(char * Number);


extern SIMCOM_Dial_Request_EN SIMCOM_Dial_Request;