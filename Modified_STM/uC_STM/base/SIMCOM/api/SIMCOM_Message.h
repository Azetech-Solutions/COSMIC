/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include PLATFORM_TYPES_H
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

#define IsSendMsgStateIdle()  (SendMSG_State == MSG_Idle)

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/
typedef enum
{
	MSG_Idle=0,
	MSG_DeleteMsgs,
	TextMessageConfig,
	MSG_SelectMsgFormat,
	MSG_SelectMobNum,
	WaitforMessageResponse,
	MSG_SendMsg,
	MessageEnded
}SendMSG_EN;

extern SendMSG_EN SendMSG_State;
extern void Send_TextMessage(char* str,UBYTE Index);
extern void SendMessage(const char* str);
extern void MessageControl(void);
extern char StoreMSGs[100];
extern char MobNumber[10];


/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

