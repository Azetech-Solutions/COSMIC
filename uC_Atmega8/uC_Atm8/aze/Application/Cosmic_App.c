/*
 * Cosmic_App.c
 *
 * Created: 02-08-2023 20:41:49
 *  Author: Admin
 */ 
/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h"
#include <avr/io.h>
#include PLATFORM_TYPES_H
#include TR_MSGS_H
#include COMIF_H
#include <stdio.h>
#include <string.h>
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

AVR_IO_Control_ST prevIO_Status={0};

void Call_Request(BOOL RequestType,UBYTE MN_Index)
{
	AVR_Call_ST *Buff = &AVR_Calls;
	
	UBYTE *Data = ComIf_GetShadowBuffer_STM32_AVR_Call();
	
	Buff->Res = 0;
	
	Buff->Call = RequestType;
	
	Buff->Call_MN_Index = MN_Index;
	
	*Data = Buff->Byte;
	
	ComIf_TransmitFromBuffer_STM32_AVR_Call();
}


void Send_ADC_Datas()
{
	ADC_Data_ST *Buff = &ADCDatas;
	
	UBYTE *Data = ComIf_GetShadowBuffer_STM32_ADC_Values_AVR();
	
	memcpy(Data,Buff,ComIf_GetLength_STM32_ADC_Values_AVR());
	
	ComIf_TransmitFromBuffer_STM32_ADC_Values_AVR();
}


void Send_Message(UBYTE MobileNum_Index,char *str)
{
	UBYTE Len = strlen(str);
	
	if(Len < 50)
	{
		UBYTE *Data = ComIf_GetShadowBuffer_STM32_AVR_Message();
		
		Data[0] = MobileNum_Index;

		for(UBYTE i=1;i<Len;i++)
		{
			Data[i] = (UBYTE )*(str++);
		}
		
		ComIf_TransmitFromBuffer_STM32_AVR_Message();
	}
}

void IOControls()
{
	AVR_IO_Control_ST *InputIO_Data = &IO_cmdData_AVR;
	AVR_IO_Control_ST *IO_StatusData = &AVR_IO_Status;
	
	UBYTE DataLen = ComIf_GetLength_STM32_IO_cmdData_AVR();
	
	if(InputIO_Data->IO1 == TRUE)
	{
		PORTC |= (1<<5);
	}
	else
	{
		PORTC &= ~(1<<5);
	}
	
	if(memcmp(InputIO_Data->Bytes,&prevIO_Status.Bytes,2) != 0)
	{
		for(UBYTE i=0;i<DataLen;i++)
		{
			IO_StatusData->Bytes[i] = InputIO_Data->Bytes[i];
		}
		
		UBYTE *Buff = ComIf_GetShadowBuffer_STM32_AVR_IO_Status();
		
		memcpy(Buff,IO_StatusData->Bytes,2);
		
		ComIf_TransmitFromBuffer_STM32_AVR_IO_Status();
		
		prevIO_Status = *InputIO_Data;
	}
}
