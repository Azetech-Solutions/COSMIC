/******************************************************************
                A Z E T E C H    S O L U T I O N S                 
-------------------------------------------------------------------
        T O O L   G E N E R A T E D   S O U R C E   C O D E        
                    D O   N O T   M O D I F Y                      
-------------------------------------------------------------------
 This file is generated with the Azetech copyrighted software tool 
 No modification should be done in this file without a written     
 consent from Azetech Solutions. Incase of any such instances,     
 Azetech Solutions will not be liable to any damages it cause.     
-------------------------------------------------------------------
 Any Distribution, Alteration, Removal, Disassemble, Decompile or  
 or Reverse Engineer the given software, without written consent   
 from Azetech Solutions by the Licensee or third parties were not  
 allowed. Such instance is considered as a breach and the Licensee 
 is the sole responsible and liable to any damages it cause.       
******************************************************************/

#ifndef __BUFFER_CONFIG_H__
#define __BUFFER_CONFIG_H__

#include <Includes.h>
#include PLATFORM_TYPES_H
/*************************************************/
/* Functional Switches and Parameter Definitions */
/*************************************************/

/* Total number of Buffers */
#define BUFFER_MAX_COUNT                  2

/* Maximum Size of each configured Buffer */
#define BUFFER_MAX_SIZE                   100


/* Buffer for AvrStatus */
#define P_BUFFER_ID_AvrStatus            0
#define P_BUFFER_LENGTH_AvrStatus        100

/* Buffer for SIMCOM */
#define P_BUFFER_ID_SIMCOM            1
#define P_BUFFER_LENGTH_SIMCOM        100



/*************************************************/
/*               Macro Definitions               */
/*************************************************/

#define AvrStatus_Buffer_Create()             Buffer_Create(P_BUFFER_ID_AvrStatus, BufferArray_AvrStatus, P_BUFFER_LENGTH_AvrStatus)
#define AvrStatus_Buffer_Get()                Buffer_GetBuffer(P_BUFFER_ID_AvrStatus)
#define AvrStatus_Buffer_EnQueue(Data)        Buffer_EnQueue(P_BUFFER_ID_AvrStatus, Data)
#define AvrStatus_Buffer_DeQueue(Data)        Buffer_DeQueue(P_BUFFER_ID_AvrStatus, Data)
#define AvrStatus_Buffer_Length()             Buffer_Length(P_BUFFER_ID_AvrStatus)
#define AvrStatus_Buffer_Clear()              Buffer_Clear(P_BUFFER_ID_AvrStatus)
#define AvrStatus_IsBufferEmpty()             (AvrStatus_Buffer_Length() == 0)

#define SIMCOM_Buffer_Create()             Buffer_Create(P_BUFFER_ID_SIMCOM, BufferArray_SIMCOM, P_BUFFER_LENGTH_SIMCOM)
#define SIMCOM_Buffer_Get()                Buffer_GetBuffer(P_BUFFER_ID_SIMCOM)
#define SIMCOM_Buffer_EnQueue(Data)        Buffer_EnQueue(P_BUFFER_ID_SIMCOM, Data)
#define SIMCOM_Buffer_DeQueue(Data)        Buffer_DeQueue(P_BUFFER_ID_SIMCOM, Data)
#define SIMCOM_Buffer_Length()             Buffer_Length(P_BUFFER_ID_SIMCOM)
#define SIMCOM_Buffer_Clear()              Buffer_Clear(P_BUFFER_ID_SIMCOM)
#define SIMCOM_IsBufferEmpty()             (SIMCOM_Buffer_Length() == 0)


/*************************************************/
/*               Type Definitions                */
/*************************************************/

typedef UBYTE BufferLengthType;

/*************************************************/
/*               Global Variables                */
/*************************************************/

extern UBYTE BufferArray_AvrStatus[];
extern UBYTE BufferArray_SIMCOM[];

/*************************************************/
/*          Extern Function Declarations         */
/*************************************************/



#endif /* __BUFFER_CONFIG_H__ */

