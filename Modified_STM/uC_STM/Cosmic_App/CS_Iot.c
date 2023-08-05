/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 

#include "Includes.h"
#include <string.h>
#include COMIF_H
#include CS_IOT_H
#include PLATFORM_TYPES_H
#include COMIF_CONFIG_H
#include UART_DRIVER_H




//UBYTE DTMF_Transmit(UWORD Length, void * Data)
//{
//	UBYTE retval = COMIF_EC_NO_ERROR;
//	
//	UBYTE *DtmfCmd;
//	
//	for(UBYTE i=0;i < Length; i++)
//	{
//		SIM_Send_Data(((*(UBYTE*)Data++)));
//		
//	}
//	
//	memset(Data,'0',Length);
//	
//	ComIf_TxConfirmation(C_ComIfChannel_DTMF);
//	
//	return retval;
//}


void DTMFCallMessageCbk(UBYTE Length, UBYTE *Data)
{
	
}

void DTMFMessageRxCbk(UBYTE Length, UBYTE *Data)
{
	
}

//void UpdateDtmfData(UBYTE MNid,UBYTE DTMFData)
//{
//	UBYTE *DTMF = ComIf_GetShadowBuffer_DTMF_MN_Data_Message();
//	
//	DTMFCommandData.DTMF_MN_Index = MNid;
//	
//	DTMFCommandData.DTMF_Data = DTMFData;
//	
//	memcpy(DTMF,DTMFCommandData.Bytes,2);
//	
//	ComIf_TransmitFromBuffer_DTMF_MN_Data_Message();
//}



