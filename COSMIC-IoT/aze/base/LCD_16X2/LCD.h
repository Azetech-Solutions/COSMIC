/*
 * LCD.h
 *
 * Created: 29-11-2022 16:20:08
 *  Author: Admin
 */ 

#ifndef LCD_H_
#define LCD_H_

/**********************************************************/
/* Header Inclusions                                      */
/**********************************************************/
#include "Includes.h" // Will have all definitions of the Project Headers

#include <stdint.h>

#include PLATFORM_TYPES_H // For Data Types
#define F_CPU 4000000UL
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
/**********************************************************/
/* Macro Definitions                                      */
/**********************************************************/

#define LCD_AreJobsAvailable()           (g_LCDJobs_Length > 0)

#define LCD_GetNextJobInQueue()          (&LCDJobs[g_LCDJobs_HeadIndex])

#define rs_hi (PORTB|=(1<<PB3))
#define en_hi (PORTB|=(1<<PB4))

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

#define LCD_CFG_MAX_SCHEDULABLE_JOBS 20

/**********************************************************/
/* Type Definitions                                       */
/**********************************************************/

typedef enum
{
	C_LCD_Idle,
	C_LCD_Init,
	C_LCD_1stWrite,
	C_LCD_1stEnable_Off,
	C_LCD_2ndWrite_EN,
	C_LCD_2ndEnable_Off_EN
	
}LCD_State_EN;

/* LCD Job Status */
typedef enum __LCDJobStatusType
{
	C_LCD_Job_Status_Idle = 0,
	C_LCD_Job_Status_Scheduled,
	C_LCD_Job_Status_WriteInProgress,
	C_LCD_Job_Status_Completed
	
}LCDJobStatusType;


/* LCD Job Config Type */
typedef struct
{
	UBYTE CmdData;
	const char* Message;
	UBYTE InDataType;
	LCDJobStatusType Status;
	
}LCD_JobConfig;

/**********************************************************/
/* Global Variable Declarations                           */
/**********************************************************/
UBYTE g_LCDJobs_Length;

extern LCD_JobConfig LCDJobs[];

extern UBYTE g_LCDJobs_HeadIndex;
extern UBYTE g_LCDJobs_TailIndex;
extern UBYTE g_LCDJobs_Length;

extern ULONG LCD_JobTimeoutTimer;

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

/**********************************************************/
/* Function Declaration                                   */
/**********************************************************/

extern void LCD_Init(void);
extern void LCD_MainFunction(void);


/* Jobs related Function */
extern void LCD_ClearJobs(void);
extern BOOL LCD_CompleteJob(void);
extern BOOL LCD_ScheduleJob(const char * Command,UBYTE Cmd,BOOL InputDataType);

/*	To display the strings in LCD use Display string Function
 *	Example : Display_String("Example_string");
 */
extern void Display_String(const char * data);

/*	LCD Control Commands are given by Below Function 
 *	Example : LCD_command(0x80);
 */
extern void LCD_command(UBYTE cmd);

extern void DebugStringRow1(const char *data);
extern void DebugStringRow2(const char *data);

#endif /* LCD_H_ */