/*
 * SIMCOM_Clock.h
 *
 *  Created on: 18-Apr-2021
 *      Author: Hari
 */

#ifndef _SIMCOM_CLOCK_H_
#define _SIMCOM_CLOCK_H_

#include "Includes.h"

/*************************/
/* Macro Definitions     */
/*************************/

#define SIMCOM_IsClockRunning()        (SIMCOM_Clock_State == SIMCOM_Clock_Running)

#define SIMCOM_GetDateTime()           SIMCOM_DateTime

/*************************/
/* Data Type Definitions */
/*************************/

typedef struct
{
	UBYTE Year;
	UBYTE Month;
	UBYTE Day;
	UBYTE Hour;
	UBYTE Minute;
	UBYTE Second;
	SBYTE TimeZone;

}SIMCOM_Clock_DateTime_ST;


typedef enum
{
	SIMCOM_Clock_Init = 0,
	SIMCOM_Clock_Synchronizing,
	SIMCOM_Clock_Running

}SIMCOM_Clock_State_EN;

/*************************/
/* Global Variables      */
/*************************/

extern SIMCOM_Clock_DateTime_ST SIMCOM_DateTime;

extern SIMCOM_Clock_State_EN SIMCOM_Clock_State;

/* [[DEBUG_MEASUREMENT */
/* NAME=SIMCOM_Clock_State */
/* MODELNAME=SIMCOM_Clock_State */
/* SIZE=2 */
/* HEADER=SIMCOM_H */
/* DEBUG_MEASUREMENT]] */

/*************************/
/* Function Declarations */
/*************************/

extern void SIMCOM_Clock_MainFunction(void);

#endif /* _SIMCOM_CLOCK_H_ */
