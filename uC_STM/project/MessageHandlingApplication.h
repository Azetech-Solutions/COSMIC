


typedef enum DtmfMessageHandler{
	IdleState = 0,
	MachineInit,
	SendNumberMessage,
	SendMotorStatus
}DtmfMessageHandler_En;

extern DtmfMessageHandler_En DtmfMessageHandlerState;

extern void DtmfMessageCallFunc();