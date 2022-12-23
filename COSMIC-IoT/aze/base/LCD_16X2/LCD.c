/*
 * LCD.c
 *
 * Created: 29-11-2022 16:19:49
 *  Author: Admin
 */ 
#include "Includes.h" // Will have all definitions of the Project Headers
#include "LCD.h"


LCD_State_EN LCD_State = C_LCD_Idle;

/**
 * LCD_Job_Callback
 *
 * Function to get a callback once the scheduled job is done
 **/
void LCD_Job_Callback(void)
{
	if(LCD_AreJobsAvailable())
	{
		LCD_JobConfig * CurrentJob = LCD_GetNextJobInQueue();

		CurrentJob->Status = C_LCD_Job_Status_Completed;
	}
	else
	{
		/* ERROR Condition: Either the Jobs were cleared or a data inconsistency issue */	
	}
}

void lcd_data(unsigned char a)
{
	if(a&0x10) {d4_hi;} else {d4_lo;}
	if(a&0x20) {d5_hi;} else {d5_lo;}
	if(a&0x40) {d6_hi;} else {d6_lo;}
	if(a&0x80) {d7_hi;} else {d7_lo;}
}

void lcd_cmd(unsigned char a)
{
	lcd_data(a&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(5);
	en_lo;
	
	lcd_data((a<<4)&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(5);
	en_lo;
}


static void lcd_init()
{
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x0c);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
	lcd_cmd(0x80);
}

void Display_String(const char * data)
{
	LCD_ScheduleJob(data,0,1);
}

void LCD_command(UBYTE cmd)
{
	LCD_ScheduleJob(NULL,cmd,0);
}

void DebugStringRow1(const char *data)
{
	LCD_command(0x80);
	Display_String(data);
}

void DebugStringRow2(const char *data2)
{
	LCD_command(0xC0);
	Display_String(data2);
}

void LCD_Init()
{
	LCD_ClearJobs();
	DDRD = 0xFF;
	DDRB = 0xFF;
	lcd_init();
	Display_String("Init");
}

void LCD_Mainfunction()
{
	
	if (LCD_AreJobsAvailable())
	{
		LCD_JobConfig * CurrentJob = LCD_GetNextJobInQueue();
		
		if(CurrentJob->Status == C_LCD_Job_Status_Completed)
		{
			LCD_CompleteJob();
		}
	}
	
	if (LCD_AreJobsAvailable())
	{
		LCD_JobConfig * CurrentJob = LCD_GetNextJobInQueue();
		
		if(CurrentJob->Status == C_LCD_Job_Status_Scheduled)
		{
			if(CurrentJob->InDataType == FALSE)
			{
				if(LCD_State == C_LCD_Idle)
				{
					if(CurrentJob->CmdData)
					{
						LCD_State = C_LCD_1stWrite;	
					}
					else
					{
						//Nothing to write
					}
				}
			}
			else
			{
				if(LCD_State == C_LCD_Idle)
				{
					if(CurrentJob->Message != NULL)
					{
						LCD_State = C_LCD_1stWrite;
					}
					else
					{
						//Nothing to write
					}
				}
			}
		}

		switch(LCD_State)
		{
			case C_LCD_Idle:
			{
				if (CurrentJob->Status == C_LCD_Job_Status_WriteInProgress)
				{
					//CurrentJob->Status = C_LCD_Job_Status_Completed;
					LCD_Job_Callback();
				}
			}
			break;
			case C_LCD_Init:
			{
							
			}
			break;
			case C_LCD_1stWrite:
			{
				CurrentJob->Status = C_LCD_Job_Status_WriteInProgress;
							
				if(CurrentJob->InDataType == FALSE)
				{
					lcd_data(CurrentJob->CmdData&0xf0);
					rs_lo;
					en_hi;
				}
				else
				{
					lcd_data((*(CurrentJob->Message))&0xf0);
					rs_hi;
					en_hi;
				}
							
				LCD_State = C_LCD_1stEnable_Off;
			}
			break;
			case C_LCD_1stEnable_Off:
			{
				en_lo;
				LCD_State = C_LCD_2ndWrite_EN;
			}
			break;
			case C_LCD_2ndWrite_EN:
			{
				if(CurrentJob->InDataType == FALSE)
				{
					lcd_data((CurrentJob->CmdData <<4)&0xf0);
					rs_lo;
					en_hi;
				}
				else
				{
					lcd_data(((*(CurrentJob->Message))<<4)&0xf0);
					rs_hi;
					en_hi;
				}
							
				LCD_State = C_LCD_2ndEnable_Off_EN;
			}
			break;
			case C_LCD_2ndEnable_Off_EN:
			{
				en_lo;
				
				if(CurrentJob->InDataType)
				{
					CurrentJob->Message++;
					if((*(CurrentJob->Message)) != '\0')
					{
						LCD_State = C_LCD_1stWrite;
					}
					else
					{
						LCD_State = C_LCD_Idle;
					}
				}
				else
				{
					LCD_State = C_LCD_Idle;
				}

			}
			break;
			default:
			{
				//Do nothing
			}
		}
	
	}
	
}