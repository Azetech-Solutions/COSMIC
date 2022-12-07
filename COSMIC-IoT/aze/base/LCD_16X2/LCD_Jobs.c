/*
 * LCD_Jobs.c
 *
 * Created: 01-12-2022 15:07:25
 *  Author: Nelson
 */ 

/***************************************************/
/*			LCD Jobs related Source File    */
/***************************************************/
/**
 * This file will schedule the job in a circular buffer
 * so that, upon the job is completed, further new jobs
 * shall be taken into account.
 *
 * The Job scheduling will work like a FIFO:
 * First Scheduled job will be completed first
 **/

/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "LCD.h"

/***************************************************/
/* Global Variables Definitions                    */
/***************************************************/

LCD_JobConfig LCDJobs[LCD_CFG_MAX_SCHEDULABLE_JOBS];

UBYTE g_LCDJobs_HeadIndex = 0;
UBYTE g_LCDJobs_TailIndex = 0;
UBYTE g_LCDJobs_Length = 0;

/***************************************************/
/* Function Definitions                            */
/***************************************************/

static BOOL LCD_CheckIfJobIsAlreadyScheduled(const char * Command,UBYTE Cmd,BOOL InputDataType)
{
	BOOL retval = FALSE;

	for(int i = g_LCDJobs_HeadIndex; i < g_LCDJobs_Length; i++)
	{
		if((LCDJobs[i].InDataType == InputDataType) && LCDJobs[i].CmdData == Cmd && LCDJobs[i].Message == Command )
		{
			// If there is the same job already been scheduled, then check if it is in progress
			if(LCDJobs[i].Status == C_LCD_Job_Status_Scheduled)
			{
				retval = TRUE;
			}
		}
	}

	return retval;
}

/**
 * LCD_ScheduleJob
 *
 * Function to schedule a LCD Job
 **/
BOOL LCD_ScheduleJob(const char * Command,UBYTE Cmd,BOOL InputDataType)
{
	BOOL retval = FALSE;

	/* Accept only if there is a possibility to schedule a job */
	if(g_LCDJobs_Length < LCD_CFG_MAX_SCHEDULABLE_JOBS)
	{
		if(LCD_CheckIfJobIsAlreadyScheduled(Command,Cmd,InputDataType) == FALSE)
		{
			LCD_JobConfig * NewJobConfig = &LCDJobs[g_LCDJobs_TailIndex];

			if(NewJobConfig->Status == C_LCD_Job_Status_Idle)
			{
				NewJobConfig->Status = C_LCD_Job_Status_Scheduled;
				NewJobConfig->CmdData = Cmd;
				NewJobConfig->Message = Command;
				NewJobConfig->InDataType = InputDataType;

				g_LCDJobs_TailIndex++; // Increment the Tail Index

				g_LCDJobs_Length++; // Increment the Job Length
				
				/* Implement a Circular Buffer */
				if((g_LCDJobs_TailIndex == LCD_CFG_MAX_SCHEDULABLE_JOBS) && (g_LCDJobs_Length < LCD_CFG_MAX_SCHEDULABLE_JOBS))
				{
					g_LCDJobs_TailIndex = 0; // Circle back to 0
				}

				retval = TRUE;
			}
		}
		else
		{
			// If it is already scheduled, then ignore the request and FALSE
			retval = FALSE;
		}
	}

	return retval;
}

/**
 * LCD_CompleteJob
 *
 * Function to complete a Job which is scheduled already
 **/
BOOL LCD_CompleteJob(void)
{
	BOOL retval = FALSE;

	/* Get the latest completed Job */
	LCD_JobConfig * CompletedJobConfig = &LCDJobs[g_LCDJobs_HeadIndex];

	/* This API shall be called only when the Job Status is completed and from the Main Function */
	if(CompletedJobConfig->Status == C_LCD_Job_Status_Completed)
	{
		g_LCDJobs_HeadIndex++; // Increment the Head Index

		g_LCDJobs_Length--; // Decrement the Length

		/* Logic to implement a Circular Buffer */
		if((g_LCDJobs_HeadIndex == LCD_CFG_MAX_SCHEDULABLE_JOBS) && (g_LCDJobs_Length != 0))
		{
			g_LCDJobs_HeadIndex = 0; // Circle back to 0
		}

		/* If the Buffer is empty, then reset the Head and Tail Indices */
		if(g_LCDJobs_Length == 0)
		{
			g_LCDJobs_HeadIndex = 0;
			g_LCDJobs_TailIndex = 0;
		}

		/* Update the Status alone in this function. Job type is referred in the Main Function for callback */
		CompletedJobConfig->Status = C_LCD_Job_Status_Idle;

		retval = FALSE;
	}
	return retval;
}

/**
 * LCD_ClearJobs
 *
 * Function to clear any pending jobs
 **/
void LCD_ClearJobs(void)
{
	int i = 0;

	for(i = 0; i < LCD_CFG_MAX_SCHEDULABLE_JOBS; i++)
	{
		LCDJobs[i].CmdData = 0;
		LCDJobs[i].Message = 0;
		LCDJobs[i].Status = C_LCD_Job_Status_Idle;
	}
	
	g_LCDJobs_HeadIndex = 0;
	g_LCDJobs_TailIndex = 0;
	g_LCDJobs_Length = 0;
}

