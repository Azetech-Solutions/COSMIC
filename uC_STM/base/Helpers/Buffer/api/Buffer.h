/*
 * Buffer.h
 * 
 * FIFO Buffer Helper
 *
 *  Created on: 15 Apr., 2021
 *      Author: Hari
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "Includes.h" /* Include platform types */
#include BUFFER_CONFIG_H

/*************************/
/* Macro Definitions     */
/*************************/


/*************************/
/* Data Type Definitions */
/*************************/

typedef struct _BufferType_ST
{
    UBYTE *BufferPtr;             // Holds the pointer to the actual buffer
    BufferLengthType Size;  // Holds the total size of the buffer
    BufferLengthType Length;      // Holds the current length of the buffer
    BufferLengthType HeadIndex;   // Holds the Head Index of the Buffer (0 based)
    BufferLengthType TailIndex;   // Holds the Tail Index of the Buffer (0 based)

}BufferType_ST;

/*************************/
/* Global Variables      */
/*************************/

extern BufferType_ST Buffers[BUFFER_MAX_COUNT];

/*************************/
/* Function Declarations */
/*************************/

extern BOOL Buffer_Create(UBYTE BufferID, UBYTE * BufferPtr, BufferLengthType Size);

extern BOOL Buffer_EnQueue(UBYTE BufferID, UBYTE Data);

extern BOOL Buffer_DeQueue(UBYTE BufferID, UBYTE * Data);

extern BufferLengthType Buffer_Length(UBYTE BufferID);

extern BOOL Buffer_Clear(UBYTE BufferID);

extern BufferType_ST * Buffer_GetBuffer(UBYTE BufferID);

#endif
