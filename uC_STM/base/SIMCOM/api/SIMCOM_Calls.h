/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"

/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

typedef enum
{
	SMC_Idle = 0,
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
extern BOOL SIMCOM_Calls_Dial(const char * Number);