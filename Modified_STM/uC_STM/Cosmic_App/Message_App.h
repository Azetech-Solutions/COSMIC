
#include "Includes.h"
#include PLATFORM_TYPES_H

extern UBYTE CurrentMotorStatus;

extern UBYTE PreviousMotorStatus;

typedef enum DtmfMessageHandler{
	IdleState = 0,
	MachineInit,
	SendNumberMessage,
	SendMotorStatus,
	AWSCmdStatusUpdate,
	NumberUpdateMessage,
	UpdateMotorStatusMsg,
	SendMultipleMessage,
	SendSingleMessage,
	EmergencyMessage,
	SendOwnerMessage,
	TextMessageProcessing
}DtmfMessageHandler_En;

#define OWNER1 "+917200033312"

#define OWNER2 "+917558023123"

extern DtmfMessageHandler_En DtmfMessageHandlerState;

extern void DtmfMessageCallFunc();

extern uint32_t EEPROMMnNoAdress[6];

extern BOOL IsSendMessageFlag;

extern void Send_TextMsgMain();

extern char strCheck[100];

extern UBYTE MsgUpdationCompleteFlag;