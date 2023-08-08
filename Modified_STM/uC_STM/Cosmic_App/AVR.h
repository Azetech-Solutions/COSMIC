/*
 * AVR.h
 *
 * Created: 07-12-2022 17:15:36
 *  Author: Admin
 */ 


#ifndef AVR_H_
#define AVR_H_

#include "Includes.h"
#include PLATFORM_TYPES_H


typedef union _AVR_IOs_
{
	struct  
	{
		UBYTE IO1:1;
		UBYTE IO2:1;
		UBYTE IO3:1;
		UBYTE IO4:1;
		UBYTE IO5:1;
		UBYTE IO6:1;
		UBYTE IO7:1;
		UBYTE IO8:1;
		//Byte 1
		UBYTE IO9:1;
		UBYTE IO10:1;
		UBYTE IO11:1;
		UBYTE IO12:1;
		UBYTE IO13:1;
		UBYTE IO14:1;
		UBYTE IO15:1;
		UBYTE IO16:1;
	};
	
	UBYTE Bytes[2];
	
}AVR_IO_Control_ST;


typedef union _ADC_Data_
{
	struct
	{
		UBYTE VoltagePhase_1_MSB : 8;
		UBYTE CurrentPhase1		 : 6;
		UBYTE VoltagePhase_1_LSB : 2;

		
		UBYTE VoltagePhase_2_MSB : 8;
		UBYTE CurrentPhase2		 : 6;
		UBYTE VoltagePhase_2_LSB : 2;
		
		UBYTE VoltagePhase_3_MSB : 8;
		UBYTE CurrentPhase3		 : 6;
		UBYTE VoltagePhase_3_LSB : 2;
	};	
	
	UBYTE Bytes[6];
	
}ADC_Data_ST;

typedef union __CALLS__
{
	struct
	{
		UBYTE Call_MN_Index :3;
		UBYTE Call : 1;
		UBYTE Res : 4;
	};
	UBYTE Byte;
}AVR_Call_ST;


typedef union _Messages_
{
	struct
	{
		UBYTE Msg_MN_Index;
		UBYTE Message[50];
	};
	
	UBYTE Bytes[51];
	
}AVR_Message_ST;


typedef union _DTMF_Command_Data_
{
	struct
	{
		UBYTE DTMF_MN_Index;
		UBYTE DTMF_Data;
	};	
	
	UBYTE Bytes[2];
	
}DTMF_Command_Data_ST;

typedef struct
{
	char MobileNumber[13];	
}MN_ST;

typedef union _To_StroreTheMobileNumbers_
{
	struct
	{
		UBYTE DTMF_MN_Index;
		MN_ST StoredMNs[6];
	};
	
	UBYTE Bytes[79];
	
}MobileNumbers_ST;




extern AVR_IO_Control_ST IO_cmdData_AVR;
extern AVR_IO_Control_ST AVR_IO_Status;
extern ADC_Data_ST ADCDatas;
extern DTMF_Command_Data_ST DTMFCommandData;
extern AVR_Message_ST AVR_Message;
extern AVR_Call_ST AVR_Calls;

#endif /* AVR_H_ */