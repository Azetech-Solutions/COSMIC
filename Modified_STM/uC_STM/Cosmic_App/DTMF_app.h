

typedef enum DtmfNumberAlterStatus{
	Idle = 0,
	ChooseTaskToAlter,
	AddNumberToStore,
	ChooseAdressToAlterNumber,
	WaitforPageErase,
	DeleteExcistingNumber
}DtmfNumberAlterStatus_En;

DtmfNumberAlterStatus_En DtmfState = Idle;