

typedef enum DtmfNumberAlterStatus{
	Idle = 0,
	UpdateDTMFSendMessage,
	SendDTMFMessage,
	ChooseTaskToAlter,
	AddNumberToStore,
	ChooseAdressToAlterNumber,
	WaitforPageErase,
	DeleteExcistingNumber
}DtmfNumberAlterStatus_En;

extern DtmfNumberAlterStatus_En DtmfState;

extern void DTMFStateMachine();

extern volatile UBYTE MNID; 

extern volatile char DTMF_Data;