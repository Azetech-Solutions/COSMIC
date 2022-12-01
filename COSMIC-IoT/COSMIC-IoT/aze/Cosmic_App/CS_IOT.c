/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 
#include "Includes.h"
#include SIMCOM_H
#include <avr/io.h>


void COSMIC_Generic_SIMCOM_Callback(SIMCOM_Job_Result_EN JobState)
{
	/* This function will be called for an un-scheduled job. So check for the response and clear the buffer */
	SIMCOM_ClearResponseBuffer();
}

void COSMIC_SIMCOM_Error_Callback(SIMCOM_Error_State_EN Error)
{
	
	
}


void Avr_Init()
{
	DDRA=0XFF;
	PORTA=0xFF;
}

void Avr_Main_x10()
{
	
	
}