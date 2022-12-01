/*
 * StringHelper.c
 *
 *  Created on: 17-Apr-2021
 *      Author: Hari
 */

#include <Includes.h>
#include STRINGHELPER_H
#include BUFFER_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

char StringHelper_Buffer[StringHelper_Buffer_LENGTH];

char StringHelper_NumberString_Buffer[12]; // To avoid dynamic Memory allocation

/*****************************************/
/* Function Definitions                  */
/*****************************************/

// Function to replace a string with another
// string
// DO NOT USE THIS FUNCTION
#if 0
char* StringHelper_ReplaceWord(const char* s, const char* oldW, const char* newW)
{
	char* result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;

			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	// Making new string of enough length
	result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 2);

	i = 0;
	while (*s) {
		// compare the substring with the result

		// Hari --> There is a problem here
		if (strstr(s, oldW) == s) {

			strcpy(&result[i], newW);

			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}
#endif

char * StringHelper_GetPointerAfter(char *str, char * searchWord)
{
	char * retval;
	int strLength = strlen(str);
	int searchLength = strlen(searchWord);

	if((strLength <= searchLength) || (searchLength == 0))
	{
		retval = str;
	}
	else
	{
		char * subStr = strstr(str, searchWord);

		int subStrLength = strlen(subStr);

		if(subStrLength == searchLength) // If it is the last value
		{
			retval = subStr;
		}
		else
		{
			retval = &subStr[searchLength];
		}
	}

	return retval;
}


void StringHelper_removeChar( char * str, char t )
{
    int i,j;
    i = 0;
    while(i<strlen(str))
    {
        if (str[i]==t)
        {
            for (j=i; j<strlen(str); j++)
                str[j]=str[j+1];
        } else i++;
    }
}

unsigned int StringHelper_SplitAndGet(const char *str, char sep, int position)
{
	unsigned int length = 0;
	unsigned int count = 0;
    unsigned int start = 0, stop;

    for (stop = 0; str[stop]; stop++) {
        if (str[stop] == sep) {
        	count++;
        	if(count == position)
        	{
        		length = stop - start;

        		break;
			}
			else
			{
				start = stop + 1;
			}
        }
    }

    if(length == 0)
    {
    	// If the position was not found, see if the request was the last split string
    	if(position == (count + 1))
    	{
    		// Then send the last value
        	length = stop - start;
		}
	}

	if( (length != 0) && ((length + 1) <= StringHelper_Buffer_LENGTH) ) // Including '\0'
	{
		int i = 0;
		for(i = 0; i < length; i++)
		{
			StringHelper_Buffer[i] = str[start + i];
		}
		StringHelper_Buffer[i] = '\0';

		return length + 1; // Includes the '\0'
	}
	else
	{
		return 0; // Send Length as 0
	}
}


char * StringHelper_NumberToString(ULONG Number)
{
	int i = 0;

	while(Number != 0)
	{
		// Shift the Number String to the next place
		int j;
		for(j = i; j > 0; j--)
		{
			StringHelper_NumberString_Buffer[j] = StringHelper_NumberString_Buffer[j - 1];
		}
		StringHelper_NumberString_Buffer[0] = ((Number % 10) + 0x30); // Convert to ASCII
		Number /= 10;
		i++;
	}

	StringHelper_NumberString_Buffer[i] = '\0';

	return StringHelper_NumberString_Buffer;
}


ULONG StringHelper_SplitAndGet_Number(const char *str, char sep, int position)
{
	ULONG retval = 0;

	if(StringHelper_SplitAndGet(str, sep, position) > 0)
	{
		retval = atoi(StringHelper_Buffer);

		StringHelper_ClearBuffer();
	}

	return retval;
}


BOOL StringHelper_startsWith(const char *pre, const char *str)
{
    ULONG lenpre = strlen(pre),
    	  lenstr = strlen(str);

    return lenstr < lenpre ? FALSE : (memcmp(pre, str, lenpre) == 0);
}
