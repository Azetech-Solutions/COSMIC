/*
 * MessageHandling.c
 *
 * Created: 25-05-2023 15:44:36
 *  Author: Admin
 */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include "Includes.h"
#include LCD_H
#include PLATFORM_TYPES_H
#include COMIF_H


typedef union
{
	struct
	{
		UBYTE SD1:1;
		//Byte 1
		UBYTE SD2:1;
		//Byte 2
		UBYTE SD3:1;
		//Byte 3
		UBYTE SD4:1;
		//Byte 4
		UBYTE SD5:1;
		//Byte 5
		UBYTE SD6:1;
		//Byte 6
		UBYTE SD7:1;
		//Byte 7
		UBYTE SD8:1;
		//Byte 8
		UBYTE SD9:1;
		//Byte 9
		UBYTE SD10:1;
		//Byte 10
		UBYTE SD11:1;
		//Byte 11
		UBYTE SD12:1;
		//Byte 12
		UBYTE SD13:1;
		//Byte 13
		UBYTE SD14:1;
		//Byte 14
		UBYTE SD15:1;
		//Byte 15
		UBYTE SD16:1;
		//Byte 16
	};
	UBYTE Data_Bytes[2];
}AvrStatusData_ST;

typedef union
{
	struct
	{
		UWORD ADCVoltage1 : 10;
		//Byte 1
		UWORD ADCCurrent1 : 6;
		//Byte 2
		UWORD ADCVoltage2 : 10;
		//Byte 3
		UWORD ADCCurrent2 : 6;
		//Byte 4
		UWORD ADCVoltage3 : 10;
		//Byte 5
		UWORD ADCCurrent3 : 6;
		//Byte 6
		UWORD ADCVoltage4 : 10;
		//Byte 7
		UWORD ADCCurrent4 : 6;
		//Byte 8
		UWORD ADCVoltage5 : 10;
		//Byte 9
		UWORD ADCCurrent5 : 6;
		//Byte 10
		UWORD ADCVoltage6 : 10;
		//Byte 11
		UWORD ADCCurrent6 : 6;
		//Byte 12
		UWORD ADCVoltage7 : 10;
		//Byte 13
		UWORD ADCCurrent7 : 6;
		//Byte 14
		UWORD ADCVoltage8 : 10;
		//Byte 15
		UWORD ADCCurrent8 : 6;
		//Byte 16
		UWORD ADCVoltage9 : 10;
		
		UWORD ADCCurrent9 : 6;
		
		UWORD ADCVoltage10 : 10;
		
		UWORD ADCCurrent10 : 6;
		
		UWORD ADCVoltage11 : 10;
		
		UWORD ADCCurrent11 : 6;
		
		UWORD ADCVoltage12 : 10;
		
		UWORD ADCCurrent12 : 6;
		
		UWORD ADCVoltage13 : 10;
		
		UWORD ADCCurrent13 : 6;
		
		UWORD ADCVoltage14 : 10;
				
		UWORD ADCCurrent14 : 6;
		
		UWORD ADCVoltage15 : 10;
				
		UWORD ADCCurrent15 : 6;		
		
		UWORD ADCVoltage16 : 10;
				
		UWORD ADCCurrent16 : 6;
	};
	UBYTE Data_Bytes[32];
}AvrVoltageStatusData_ST;

typedef enum AvrMessageState{
	AvrIdle = 0,
	AvrSwitchStatus,
	AvrAdcVoltageStatus
}AvrMessageState_ST;

extern UBYTE Cloud_Transmit(UWORD Length, void * Data);
extern void DebugString(char *data);
extern void Cloud_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1);
extern UBYTE ComIf_RxIndication_Cloud(UBYTE DataByte);
extern void CloudRxCommandDataRxCbk(unsigned char Length, unsigned char *Data);
extern void uart_tx(UBYTE a);
extern void uart_string(UBYTE *data);
extern void lcd_string(unsigned char *a);

void UpdateAdcStatusData();

/*void updateSendData(UBYTE Data[]);*/

void updateSendData(UBYTE Data);

char rxCmdBuffer[14];

AvrStatusData_ST AvrStatusData;

AvrVoltageStatusData_ST AvrVoltageStatusData;

extern void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data);

UBYTE PrevAdcData[32];

AvrMessageState_ST AvrMessageState = AvrIdle;

void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data)
{
	UBYTE avrcmd[Length];
	for(UBYTE i=0;i<Length;i++)
	{
		avrcmd[i] = *Data;
		Data++;
	}
	if((avrcmd[0]&1) == 1)
	{
		PORTC |= (1<<5);
		avrcmd[0] = 1;
		updateSendData(1); 
	}
	else
	{
		PORTC &= ~(1<<5);
		avrcmd[0] = 0;
		updateSendData(0);
	}
}

void Cloud_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1)
{
	
}



UBYTE Cloud_Transmit(UWORD Length, void * Data)
{
	UBYTE retval = COMIF_EC_NO_ERROR;
	
	for(UBYTE i = 0;i < Length; i++)
	{
		uart_tx(((*(UBYTE*)Data++)));
	}
	
	memset(Data,'0',Length);
	
	ComIf_TxConfirmation(C_ComIfChannel_Cloud);
	
	return retval;
}

void updateSendData(UBYTE Data)
{
	AvrStatusData_ST *AvrStatus = &AvrStatusData;
	
	AvrStatus->Data_Bytes[0] = Data;
	
	UBYTE *PubMsg = ComIf_GetShadowBuffer_Cloud_StatusData();
	
	memcpy(PubMsg,AvrStatus->Data_Bytes,2);
	
	ComIf_TransmitFromBuffer_Cloud_StatusData();
}

void UpdateAdcStatusData()
{
	AvrVoltageStatusData_ST *AvrVoltageStatusData1 = &AvrVoltageStatusData;
	
	for(UBYTE i = 0; i < 32; i++)
	{
		AvrVoltageStatusData1->Data_Bytes[i] = i;
	}
	
	UBYTE *AvrVoltageStatus = ComIf_GetShadowBuffer_Cloud_AdcVoltageCarryFunc();
	
	memcpy(AvrVoltageStatus,AvrVoltageStatusData1->Data_Bytes,32);
	
	ComIf_TransmitFromBuffer_Cloud_AdcVoltageCarryFunc();
}

void Adccheck()
{
	
	UBYTE AdcData[32];
	
	if(PrevAdcData[0] != AdcData[0])
	{
		UpdateAdcStatusData();
	}
	
	PrevAdcData[0] = AdcData[0];
}


void AvrStatusTransmitFunc(UBYTE avrstat[])
{
	switch(AvrMessageState)
	{
		case AvrIdle :
		{
			
			break;
		}
		case AvrSwitchStatus :
		{
			updateSendData(avrstat);
			AvrMessageState = AvrAdcVoltageStatus;
			break;
		}
		case AvrAdcVoltageStatus :
		{
			UpdateAdcStatusData();
			AvrMessageState = AvrIdle;
			break;
		}
		default :
		{
			break;
		}
	}
}


	