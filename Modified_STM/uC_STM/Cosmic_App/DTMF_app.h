

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

extern volatile char DTMF_Data;

extern UBYTE DTMFMessageFlag;