/*
 * LCD.h
 *
 * Created: 29-11-2022 16:20:08
 *  Author: Admin
 */ 


#ifndef LCD_H_
#define LCD_H_


#include "Includes.h" // Will have all definitions of the Project Headers

#include <stdint.h>

#include PLATFORM_TYPES_H // For Data Types

#include <string.h>


#define RS(x) (PORTB|=(x<<PB3))
#define EN(x) (PORTB|=(x<<PB4))

#define d4_hi (PORTD|=(1<<PD4))
#define d5_hi (PORTD|=(1<<PD5))
#define d6_hi (PORTD|=(1<<PD6))
#define d7_hi (PORTD|=(1<<PD7))

#define rs_lo (PORTB&=~(1<<PB3))
#define en_lo (PORTB&=~(1<<PB4))

#define d4_lo (PORTD&=~(1<<PD4))
#define d5_lo (PORTD&=~(1<<PD5))
#define d6_lo (PORTD&=~(1<<PD6))
#define d7_lo (PORTD&=~(1<<PD7))



/* LCD Job Result State Type */
typedef enum
{
	LCD_Job_Idle = 0,    // Default State
	LCD_Job_Scheduled,   // ---> This will be updated by the Client who schedules the Job.
	LCD_Job_InProgress,  // When the Job is In-Progress
	LCD_Job_Timeout,     // Response Timed out
	LCD_Job_Incomplete,  // Half of the Response received, but timed out later
	LCD_Job_Completed,   // Job completed successfully, Response is available in the Buffer
	LCD_Job_Aborted      // Job aborted because of multiple failure attempts

}LCD_Job_Result_EN;


/* LCD Job Callback type */
typedef void (*LCD_Callback_Type)(LCD_Job_Result_EN);


typedef enum  
{
	LCD_Idle_EN,
	LCD_Init_EN,
	LCD_1stWrite_EN,
	LCD_1stEnable_Off_EN,
	LCD_2ndWrite_EN,
	LCD_2ndEnable_Off_EN
	
}LCD_State_EN;

/* LCD Job Status */
typedef enum __LCDJobStatusType
{
	C_LCD_COM_Status_Idle = 0,
	C_LCD_COM_Status_Scheduled,
	C_LCD_COM_Status_WriteInProgress,
	C_LCD_COM_Status_Completed
	
}LCDJobStatusType;


typedef union
{
	typedef struct
	{
		UBYTE CM_Data;
	}Cmd_D;	
	
	typedef struct
	{
		const char *message_D;	
	}Msg_D;
	
}LCDInputData;

/* LCD Job Scheduler Type */
typedef struct
{
	UBYTE JobCount;
	LCDInputData Input_Data;
	UBYTE InDataType;
}LCD_Jobs;



#endif /* LCD_H_ */