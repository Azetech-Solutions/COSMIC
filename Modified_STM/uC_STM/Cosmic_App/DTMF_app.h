

typedef enum DtmfNumberAlterStatus{
	Idle = 0,
	ChooseTaskToAlter,
	AddNumberToStore,
	ChooseAdressToAlterNumber,
	WaitforPageErase,
	DeleteExcistingNumber
}DtmfNumberAlterStatus_En;

extern DtmfNumberAlterStatus_En DtmfState;

extern UBYTE CheckReadyForDtmf();