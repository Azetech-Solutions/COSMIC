/* FIFO Buffer Helper Source File */

#include <Includes.h>

#include BUFFER_H
#include BUFFER_CONFIG_H

/*****************************************/
/* Global Variables                      */
/*****************************************/

BufferType_ST Buffers[BUFFER_MAX_COUNT];

/*****************************************/
/* Static Function Definitions           */
/*****************************************/

/*****************************************/
/* Function Definitions                  */
/*****************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Buffer Create Function                        */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL Buffer_Create(UBYTE BufferID, UBYTE * BufferPtr, BufferLengthType Size)
{
	BOOL retval = FALSE;

	BufferType_ST * Buffer = Buffer_GetBuffer(BufferID);

	/* Check if the Buffer is valid */
	if(Buffer != NULL_PTR)
	{
		if(Buffer->BufferPtr == NULL_PTR)
		{
			Buffer->BufferPtr = BufferPtr;
			Buffer->Size = Size;
			Buffer->HeadIndex = 0;
			Buffer->Length = 0;
			Buffer->TailIndex = 0;

			retval = TRUE;
		}
	}

	return retval;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Buffer EnQueue Function                       */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL Buffer_EnQueue(UBYTE BufferID, UBYTE Data)
{
	BOOL retval = FALSE;

	BufferType_ST * Buffer = Buffer_GetBuffer(BufferID);

	/* Check if the Buffer is valid */
	if(Buffer != NULL_PTR)
	{
		if(Buffer->BufferPtr != NULL_PTR)
		{
			/* If the Buffer is Free */
			if(Buffer->Length < Buffer->Size)
			{
				Buffer->BufferPtr[Buffer->TailIndex] = Data;

				Buffer->TailIndex++; // Increment the Tail position for next Insertion

				Buffer->Length++; // Increment the Length of the Buffer

				/* Logic to implement a Circular Buffer */
				if((Buffer->TailIndex == Buffer->Size) && (Buffer->Length < Buffer->Size))
				{
					Buffer->TailIndex = 0; // Circle back to 0
				}

				retval = TRUE;
			}
		}
	}

	return retval;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Buffer DeQueue Function                       */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL Buffer_DeQueue(UBYTE BufferID, UBYTE * Data)
{
	BOOL retval = FALSE;

	BufferType_ST * Buffer = Buffer_GetBuffer(BufferID);

	/* Check if the Buffer is valid */
	if(Buffer != NULL_PTR)
	{
		if(Buffer->BufferPtr != NULL_PTR)
		{
			/* If the Buffer is not Empty */
			if(Buffer->Length != 0)
			{
				*Data = Buffer->BufferPtr[Buffer->HeadIndex];

				Buffer->BufferPtr[Buffer->HeadIndex] = 0; // While Dequeuing clear the value

				Buffer->HeadIndex++; // Increment the Head position for next Dequeuing

				Buffer->Length--; // Decrement the Length of the Buffer

				/* Logic to implement a Circular Buffer */
				if((Buffer->HeadIndex == Buffer->Size) && (Buffer->Length != 0))
				{
					Buffer->HeadIndex = 0; // Circle back to 0
				}

				/* If the Buffer is empty, then reset the Head and Tail Indices */
				if(Buffer->Length == 0)
				{
					Buffer->HeadIndex = 0;
					Buffer->TailIndex = 0;
				}

				retval = TRUE;
			}
		}
	}

	return retval;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                  Buffer Length Function                        */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BufferLengthType Buffer_Length(UBYTE BufferID)
{
	BufferLengthType Length = 0;

	BufferType_ST * Buffer = Buffer_GetBuffer(BufferID);

	/* Check if the Buffer is valid */
	if(Buffer != NULL_PTR)
	{
		if(Buffer->BufferPtr != NULL_PTR)
		{
			Length = Buffer->Length;
		}
	}

	return Length;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                   Buffer Clear Function                        */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BOOL Buffer_Clear(UBYTE BufferID)
{
	BOOL retval = FALSE;

	BufferType_ST * Buffer = Buffer_GetBuffer(BufferID);

	/* Check if the Buffer is valid */
	if(Buffer != NULL_PTR)
	{
		if(Buffer->BufferPtr != NULL_PTR)
		{
			int i = 0;

			for(i = 0; i < Buffer->Size; i++)
			{
				Buffer->BufferPtr[i] = 0;
			}

			Buffer->Length = 0;
			Buffer->HeadIndex = 0;
			Buffer->TailIndex = 0;

			retval = TRUE;
		}
	}

	return retval;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                     Buffer Get Function                        */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
BufferType_ST * Buffer_GetBuffer(UBYTE BufferID)
{
	/* Check if the BufferID is valid */
	if(BufferID < BUFFER_MAX_COUNT)
	{
		return &(Buffers[BufferID]);
	}
	else
	{
		/* Maximum Buffer Count Reached / Invalid Buffer ID */
		return NULL_PTR;
	}
}
