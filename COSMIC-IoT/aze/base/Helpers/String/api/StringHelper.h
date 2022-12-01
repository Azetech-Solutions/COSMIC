/*
 * StringHelper.h
 *
 *  Created on: 17-Apr-2021
 *      Author: Hari
 */

#ifndef _STRINGHELPER_H_
#define _STRINGHELPER_H_

#include "Includes.h"

#include PLATFORM_TYPES_H // For Data Types

#include <string.h>
#include <stdlib.h>

/*************************/
/* Macro Definitions     */
/*************************/

#define StringHelper_Buffer_LENGTH   255

/*************************/
/* Data Type Definitions */
/*************************/

/*************************/
/* Global Variables      */
/*************************/

extern char StringHelper_Buffer[];

/*************************/
/* Function Declarations */
/*************************/

static inline void StringHelper_ClearBuffer(void)
{
	memset(StringHelper_Buffer, '\0', StringHelper_Buffer_LENGTH);
}

static inline void StringHelper_GetString(char * Str)
{
	strcpy(Str, StringHelper_Buffer);
	StringHelper_ClearBuffer();
}

// extern char* StringHelper_ReplaceWord(const char* s,   const char* oldW, const char* newW ); // DO NOT USE THIS FUNCTION
extern char * StringHelper_GetPointerAfter(char *str, char * searchWord);
extern void StringHelper_removeChar( char * str, char t );
extern unsigned int StringHelper_SplitAndGet(const char *str, char sep, int position);
extern ULONG StringHelper_SplitAndGet_Number(const char *str, char sep,  int position);
extern char* StringHelper_NumberToString(ULONG Number);
extern BOOL StringHelper_startsWith(const char *pre, const char *str);

#endif /* _STRINGHELPER_H_ */
