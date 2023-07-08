
#include "Includes.h"
#include PLATFORM_TYPES_H

extern UBYTE CurrentMotorStatus;

extern UBYTE PreviousMotorStatus;

typedef enum DtmfMessageHandler{
	IdleState = 0,
	MachineInit,
	SendNumberMessage,
	SendMotorStatus,
	NumberUpdateMessage,
	UpdateMotorStatusMsg,
	TextMessageProcessing
}DtmfMessageHandler_En;

	

extern DtmfMessageHandler_En DtmfMessageHandlerState;

extern void DtmfMessageCallFunc();

