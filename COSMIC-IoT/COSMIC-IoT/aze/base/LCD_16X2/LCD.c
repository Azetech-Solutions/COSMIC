/*
 * LCD.c
 *
 * Created: 29-11-2022 16:19:49
 *  Author: Admin
 */ 
#include "LCD.h"

LCD_Jobs LCD_CurrentJob;

LCDJobStatusType LCD_JobState= C_LCD_COM_Status_Idle;

LCD_State_EN LCD_State = LCD_Init_EN;




BOOL LCD_Schedule_Job(const char * Command,BOOL InputDataType ,LCD_Callback_Type Callback)
{
	BOOL retval = FALSE;

	LCD_CurrentJob.InDataType = InputDataType;
	
	if(LCD_CurrentJob.InDataType == 1)
	{
		LCD_CurrentJob.Input_Data.Cmd_D.CM_Data = (UBYTE)Command;
		retval = TRUE;
	}
	else
	{
		strcpy(LCD_CurrentJob.Input_Data.Msg_D.message_D,Command);
		retval = TRUE;
	}

	return retval;
}


void LCD_DATA(unsigned char a)
{
	if(a&0x10) {d4_hi;} else {d4_lo;}
	if(a&0x20) {d5_hi;} else {d5_lo;}
	if(a&0x40) {d6_hi;} else {d6_lo;}
	if(a&0x80) {d7_hi;} else {d7_lo;}
}


void LCD_Mainfunction()
{
	
	
	switch(LCD_State)
	{
		case LCD_Idle_EN:
		{
			
		}
		break;
		case LCD_Init_EN:
		{
			
		}
		break;
		case LCD_1stWrite_EN:
		{
				LCD_DATA(0&0xf0);
				RS(0);
				EN(1);
				
				LCD_State = LCD_1stEnable_Off_EN;
				
		}
		break;
		case LCD_1stEnable_Off_EN:
		{
				EN(0);
				LCD_State = LCD_2ndWrite_EN;
		}
		break;
		case LCD_2ndWrite_EN:
		{
				LCD_DATA((0<<4)&0xf0);
				RS(0);
				EN(1);
				LCD_State = LCD_2ndEnable_Off_EN;
		}
		break;
		case LCD_2ndEnable_Off_EN:
		{
			EN(0);
			LCD_State = LCD_Idle_EN;
		}
		break;
		default:
		{
			//Do nothing
		}
	}
	
}