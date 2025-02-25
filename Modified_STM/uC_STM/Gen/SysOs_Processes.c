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

#include "Includes.h"
#include SYSOS_H

/**********************************
 * Global Variables Declarations
 **********************************/
SysOS_Ctrl_ST SysOS_Ctrl;

#if(P_SYS_OS_TASK_FACTOR_MAX > 2)
UWORD g_SysOS_Counter;
#endif


/* Extern Declaration of all the Processes */
extern void SIMCOM_Init(void);
extern void SIMCOM_MainFunction(void);
extern void Buffer_Init(void);
extern void GPIO_init(void);
extern void USART2_Init(void);
extern void USART_Init(void);
extern void SystickTimerInit(void);
extern void PLLInit(void);
extern void AvrStatusHandleFunc(void);
extern void Sysos_check(void);
extern void EepromFlashMmeoryCopy(void);
extern void UpdateEEPROMNumbers(void);

/* Define the Process calls for the Tasks */

/* Definition for the task Init */
void PRC_SYSOS_TASK_INIT(void)
{
		PLLInit();
		Buffer_Init();
		GPIO_init();
		USART2_Init();
		USART_Init();
		SystickTimerInit();
		EepromFlashMmeoryCopy();
		UpdateEEPROMNumbers();
		SIMCOM_Init();
}

/* Definition for the task x100 */
void PRC_SYSOS_TASK_X100(void)
{
		SIMCOM_MainFunction();
		AvrStatusHandleFunc();
}


void FUN_SYS_Internal_Init(void)
{
	/* Initialize All the Tasks with Factor 1 as TRUE by default */

	PRC_SYSOS_TASK_INIT();

	SysOS_Ctrl.Isx100 = TRUE;
}

void FUN_SYS_Internal_Evaluate_Tasks_x1(void)
{
	/**********************************************/
	/* Tasks with Factor 0 will not be considered */
	/* Tasks with Factor 1 will be Ignored        */
	/* Toggle the Tasks which has the factor of 2 */
	/**********************************************/

	/* Task Init Ignored as the factor was 0 */
	/* Task x100 Ignored as the factor was 1 */
#if (P_SYS_OS_TASK_FACTOR_MAX > 2)

	/* Increment the counter for next run */
	g_SysOS_Counter++;
	if(g_SysOS_Counter > P_SYS_OS_TASK_FACTOR_MAX)
	{
		/* Resetting the counter to zero will cause all tha tasks to be executed once again. */
		/* So Set the value of g_SysOS_Counter to 1	*/
		g_SysOS_Counter = 1; 
	}
#endif
}

void FUN_SYS_Internal_Execute_Tasks(void)
{

	PRC_SYSOS_TASK_X100();

}


