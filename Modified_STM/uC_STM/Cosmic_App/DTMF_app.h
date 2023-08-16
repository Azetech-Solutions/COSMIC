#ifndef __DTMF_APP_H
#define __DTMF_APP_H

typedef enum DtmfNumberAlterStatus{
	Idle = 0,
	UpdateDTMFSendMessage,
	SendDTMFMessage,
	EndDTMFCall,
	ChooseTaskToAlter,
	AddNumberToStore,
	ChooseAdressToAlterNumber,
	WaitforPageErase,
	DeleteExcistingNumber,
	NumberUpdationCompleted
}DtmfNumberAlterStatus_En;

extern UBYTE DTMFBuffer[15];

extern DtmfNumberAlterStatus_En DtmfState;

extern void DTMFStateMachine();

extern volatile UBYTE MNID; 

extern char DTMF_Data;

extern UBYTE DTMFNumberindex;

extern UBYTE DTMFMessageFlag;

extern UBYTE DTMFCallOnProcess;

extern void DTMFMessageUpdation();

extern void ClearDtmfNumberString();

extern char DTMFNumberString[3];

extern UBYTE SendMbNoMsg;

extern void UpdateMobileNumbersToSend();

#endif /*_DTMF_APP_H*/