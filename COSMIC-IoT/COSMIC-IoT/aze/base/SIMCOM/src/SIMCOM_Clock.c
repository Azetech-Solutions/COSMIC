/*
 * SIMCOM_Clock.c
 *
 *  Created on: 18-Apr-2021
 *      Author: Hari
 */


#include "Includes.h"
#include SIMCOM_H
#include <avr/io.h>
/*****************************************/
/* Global Variables                      */
/*****************************************/

SIMCOM_Clock_DateTime_ST SIMCOM_DateTime;

SIMCOM_Clock_State_EN SIMCOM_Clock_State = SIMCOM_Clock_Init;

static SIMCOM_Job_Result_EN SIMCOM_Job_Result = SIMCOM_Job_Idle;

static UBYTE SIMCOM_ClockSyncCount = SIMCOM_CLOCK_SYNC_COUNT;

static UWORD SIMCOM_ClockSecondCounter = 0;

static BOOL RunClock = FALSE;

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

static void SIMCOM_Clock_ParseDateTime_InResponse()
{
	/* Postive Response would be -> +CCLK: <time>
	 *
	 * <time> 	String type(string should be included in quotation marks) value;
	 * 			format is "yy/MM/dd,hh:mm:ss±zz", where characters indicate
	 * 			year (two last digits),month, day, hour, minutes, seconds
	 * 			and time zone (indicates the difference, expressed in quarters of an hour, between the local time and GMT; range -47...+48).
	 *
	 * E.g. 6th of May 2010, 00:01:52 GMT+2 hours equals to "10/05/06,00:01:52+08".
	 *
	 * Note: For IST (GMT+5:30 hours), the Time Zone would be +22
	 */

	char * resultString = StringHelper_GetPointerAfter(SIMCOM_GetResponseBuffer(), "+CCLK: "); // Remove the +CCLK:  string

	StringHelper_removeChar(resultString, '"'); // Remove Quotes

	if(StringHelper_SplitAndGet(resultString, ',', 1) == 9) // Get Date String
	{
		char DateString[9];
		StringHelper_GetString(DateString);

		SIMCOM_DateTime.Year = StringHelper_SplitAndGet_Number(DateString, '/', 1);
		SIMCOM_DateTime.Month = StringHelper_SplitAndGet_Number(DateString, '/', 2);
		SIMCOM_DateTime.Day = StringHelper_SplitAndGet_Number(DateString, '/', 3);
	}

	if(StringHelper_SplitAndGet(resultString, ',', 2) == 12)
	{
		char TimeZString[12];
		StringHelper_GetString(TimeZString);

		SIMCOM_DateTime.Hour = StringHelper_SplitAndGet_Number(TimeZString, ':', 1);
		SIMCOM_DateTime.Minute = StringHelper_SplitAndGet_Number(TimeZString, ':', 2);

		if(StringHelper_SplitAndGet(TimeZString, ':', 3) == 6)
		{
			char SecondString[6];
			StringHelper_GetString(SecondString);

			SIMCOM_DateTime.Second = StringHelper_SplitAndGet_Number(SecondString, '+', 1); //Exclude the TimeZone
		}
	}
}


static void SIMCOM_Clock_Job_Callback(SIMCOM_Job_Result_EN result)
{
	// This function will be called by the SIMCOM handler upon successful reception of the response

	// This will be read in the Main function, so simply set the value
	SIMCOM_Job_Result = result;
}

/*****************************************/
/* Function Definitions                  */
/*****************************************/

void SIMCOM_Clock_MainFunction(void)
{
	if(RunClock == TRUE)
	{
		// By this time, we would have the updated Time value with 100ms resolution
		// So, from now on, increment the time every 100ms

		SIMCOM_ClockSecondCounter += P_SIMCOM_TASK_CYCLE_FACTOR;

		if(SIMCOM_ClockSecondCounter == 1000)
		{
			SIMCOM_DateTime.Second++;
			SIMCOM_ClockSecondCounter = 0;

			if(SIMCOM_DateTime.Second == 60)
			{
				SIMCOM_DateTime.Second = 0;
				SIMCOM_DateTime.Minute++;

				if(SIMCOM_DateTime.Minute == 60)
				{
					SIMCOM_DateTime.Minute = 0;
					SIMCOM_DateTime.Hour++;

					if(SIMCOM_DateTime.Hour == 24)
					{
						SIMCOM_DateTime.Hour = 0;

						/* In this time, Synchronize the Date from the server to avoid calculation of days in Month, Leap year etc. */
						SIMCOM_Clock_State = SIMCOM_Clock_Synchronizing;

						SIMCOM_Job_Result = SIMCOM_Job_Idle; // Set Job to Idle to Sync again

						SIMCOM_ClockSyncCount = SIMCOM_CLOCK_SYNC_COUNT; // Reset the Sync Count
					}
				}
			}
		}
	}

	// Execute only when SIMCOM Module is Ready
	if(IsSIMCOM_Module_Ready())
	{
		switch(SIMCOM_Clock_State)
		{
			case SIMCOM_Clock_Init:
			{
				// Get the Clock from GSM Module

				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CCLK?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_Clock_Job_Callback) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						// Parse the Time from the response string

						SIMCOM_Clock_ParseDateTime_InResponse();

						RunClock = FALSE; // Do not run the clock while Synchronizing

						SIMCOM_Clock_State = SIMCOM_Clock_Synchronizing; // Move to Next State
					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{
						SIMCOM_ERROR_CALLBACK(SIMCOM_Error_Clock);
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;

			case SIMCOM_Clock_Synchronizing:
			{
				// Get the Clock from GSM Module

				if(SIMCOM_Job_Result == SIMCOM_Job_Idle)
				{
					// Send AT Command and wait for response
					if(SIMCOM_Schedule_Job("AT+CCLK?", SIMCOM_DEFAULT_TIMEOUT, SIMCOM_Clock_Job_Callback) == TRUE)
					{
						// Set it to Scheduled only when the SIMCOM Module Accepted it
						SIMCOM_Job_Result = SIMCOM_Job_Scheduled;
					}
				}
				else
				{
					// Cyclic part for the response
					if(SIMCOM_Job_Result == SIMCOM_Job_Completed)
					{
						// Parse the Time from the response string
PORTA = 0X00;
						UBYTE SwSecond = SIMCOM_DateTime.Second;

						SIMCOM_Clock_ParseDateTime_InResponse();

						if(SwSecond != SIMCOM_DateTime.Second)
						{
							// Once Synced, Move the state to Running
							SIMCOM_Clock_State = SIMCOM_Clock_Running; // Move to Next State

							RunClock = TRUE; // Set the Flag to run clock independent of the SIMCOM issues after it started

							SIMCOM_ClockSecondCounter = P_SIMCOM_TASK_CYCLE_FACTOR;
						}
						else
						{
							if(SIMCOM_ClockSyncCount != 0)
							{
								SIMCOM_ClockSyncCount--;

								SIMCOM_Job_Result = SIMCOM_Job_Idle; // Set Job to Idle to Sync again
							}
							else
							{
								// Unable to Sync the Time for 100ms.
								SIMCOM_ERROR_CALLBACK(SIMCOM_Error_Clock);

								// But Still move to the next state as the time functionality is needed
								SIMCOM_Clock_State = SIMCOM_Clock_Running; // Move to Next State

								RunClock = TRUE; // Set the Flag to run clock independent of the SIMCOM issues after it started

								SIMCOM_ClockSecondCounter = P_SIMCOM_TASK_CYCLE_FACTOR;
							}
						}

					}
					else if( (SIMCOM_Job_Result == SIMCOM_Job_Timeout) || (SIMCOM_Job_Result == SIMCOM_Job_Incomplete) )
					{

						SIMCOM_ERROR_CALLBACK(SIMCOM_Error_Clock);
					}
					else
					{
						// Do Nothing. Wait
					}
				}
			}
			break;

			case SIMCOM_Clock_Running:
			{
				// Do Nothing
			}
		}
	}
	else
	{
		// If the parent module is not ready, either initializing or re-initializing, then the keep the state in Init state to re-evaluate
		SIMCOM_Clock_State = SIMCOM_Clock_Init;
	}
}
