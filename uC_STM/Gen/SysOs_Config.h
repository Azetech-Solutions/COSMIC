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

#ifndef __SYSOS_CONFIG_H__
#define __SYSOS_CONFIG_H__

/*************************************************/
/* Functional Switches and Parameter Definitions */
/*************************************************/

/* Defining SysOs Method */
#define S_SYS_OS_METHOD                 S_SYS_OS_POLLING_METHOD

/* SysOs Tick */
#define P_SYS_OS_TICK                   10

/* SysOS Main Function Type */
#define S_SYS_OS_MAIN_FUNCTION_TYPE     S_SYS_OS_DEFAULT_MAIN_FUNCTION_TYPE

/* Switches and Parameters for SysOs Tasks */
#define S_SYS_OS_TASKS_ENABLED

#define S_SYS_OS_TASK_Init     ON
#define P_SYS_OS_TASK_FACTOR_Init     0

#define S_SYS_OS_TASK_x100     ON
#define P_SYS_OS_TASK_FACTOR_x100     1


#define S_SYS_OS_DUAL_CORE_ENABLED       FALSE

/* Defining MAX Parameter for the Task Factor to reset the counter */
#define P_SYS_OS_TASK_FACTOR_MAX         1

#define S_SYS_OS_TASK_OVERRUN_MONITORING_ENABLED       FALSE

#define S_SYS_OS_STACK_OVERFLOW_MONITORING_ENABLED       FALSE

/*************************************************/
/*  Type Definitions and Variable Declarations   */
/*************************************************/
typedef struct __SysOS_Ctrl_ST
{
	UBYTE ExecPending  : 1;
    UBYTE IsInit   : 1;
    UBYTE Isx100   : 1;
}SysOS_Ctrl_ST;

extern SysOS_Ctrl_ST SysOS_Ctrl;


/*************************************************/
/*  Function Declarations & Inline Definitions   */
/*************************************************/

extern void PRC_SYSOS_TASK_INIT(void);
extern void PRC_SYSOS_TASK_X100(void);

extern void FUN_SYS_Internal_Init(void);

extern void FUN_SYS_Internal_Evaluate_Tasks_x1(void);
extern void FUN_SYS_Internal_Execute_Tasks(void);

#endif /* __SYSOS_CONFIG_H__ */

