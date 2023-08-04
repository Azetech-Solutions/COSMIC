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


#include <Includes.h>
#include BUFFER_H
#include BUFFER_CONFIG_H

/******************************************************/
/*           Global Variable Definitions              */
/******************************************************/

UBYTE BufferArray_AvrStatus[P_BUFFER_LENGTH_AvrStatus];
UBYTE BufferArray_SIMCOM[P_BUFFER_LENGTH_SIMCOM];

/******************************************************/
/*        Configuration Function Definitions          */
/******************************************************/

void Buffer_Init(void)
{
	AvrStatus_Buffer_Create();
	SIMCOM_Buffer_Create();
}

