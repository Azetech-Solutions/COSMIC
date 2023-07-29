/*
 * SysOS.c
 *
 *  Created on: 12 Feb., 2017
 *      Author: Hari
 */

#include "Includes.h"
#include SYSOS_H

/*************************************
 * Configuration Checks
 *************************************/
#ifndef S_SYS_OS_TASKS_ENABLED
#error "No OS Tasks were Enabled"
#endif

#ifndef P_SYS_OS_TASK_FACTOR_MAX_0
#if (P_SYS_OS_TASK_FACTOR_MAX == 0)
#error "There is no Cyclic Task configured!"
#endif
#else
#if ((P_SYS_OS_TASK_FACTOR_MAX_0 == 0) || (P_SYS_OS_TASK_FACTOR_MAX_1 == 0))
#error "There is no Cyclic Task configured!"
#endif
#endif

#if (S_SYS_OS_METHOD == S_SYS_OS_RTOS_METHOD)
/**
 * The SysOs component will be used alongside the RTOS only when the RTOS doesn't enable time-slicing feature.
 * So, the SysOs component will listen for the RTOS Tick and act as a time sliced scheduler
 */

#if ((1000 / CONFIG_FREERTOS_HZ) != P_SYS_OS_TICK)
#error "Actual RTOS Tick configuration differs from the SysOs configuration"
#endif

#if S_SYS_OS_DUAL_CORE_ENABLED == FALSE
#error "Currently Free RTOS tick method is tested only for ESP32 based dual core uCs"
#endif

#else
#if S_SYS_OS_DUAL_CORE_ENABLED == TRUE
#error "Dual processing needs an RTOS"
#endif
#endif

#if S_SYS_OS_DUAL_CORE_ENABLED == TRUE
#ifdef CONFIG_FREERTOS_UNICORE
#error "Incompatible Core configuration! FreeRTOS is enabled for Single Core processing, but SysOs is configured for Dual Core Processing!"
#endif
#endif

/**
 * Stack overflow monitoring takes a longer time to calculate the minimum free stack size.
 * This call can take a while and might break the system. Possibly the OS to hang.
 * This might result in the Task Overrun error obviously. So both should not be activated at the same time.
 *
 * General guideline is that the Stack Overflow monitoring shall be enabled during the development phase to calculate the free space.
 * Based on the available free space, the stack allocation shall be altered before production.
 * The Stack Overflow monitoring shall not be enabled in production.
 *
 * However, the Task Overrun monitoring shall be enabled during production.
 * There are possibilities that some ISRs might be take a while, that might block the task's execution.
 * It should be enabled both during development and during production.
 */
#if S_SYS_OS_TASK_OVERRUN_MONITORING_ENABLED && S_SYS_OS_STACK_OVERFLOW_MONITORING_ENABLED
#error "Both Task Overrun and Stack OverFlow monitoring shall not be enabled"
#endif

/**********************************
 * Global Variables Declarations
 **********************************/


/********************************************************
 * Global Flag which is used to quit the SysOs Execution
 * on the event of any Fatal Errors
 ********************************************************/
#if (S_SYS_OS_MAIN_FUNCTION_TYPE != S_SYS_OS_RTOS_MAIN_FUNCTION_TYPE)
BOOL g_Fatal_Error_B = FALSE;
#endif

/***********************************
 * Function Definitions
 ***********************************/

/* Initialization function */
void PRC_SYS_INIT(void)
{	
	FUN_SYS_Internal_Init();
}

/***********************************
 * Function to evaluate the tasks
 ***********************************/
#if (S_SYS_OS_DUAL_CORE_ENABLED != TRUE)
void FUN_SYS_Evaluate_Tasks_x1(void)
{
#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
	/* Update the tasks scheduling ONLY when the previous tasks were done. */
	if(SysOS_Ctrl.ExecPending == FALSE)
	{
#endif

		FUN_SYS_Internal_Evaluate_Tasks_x1();

#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
		/* Set the Execution Pending flag */
		SysOS_Ctrl.ExecPending = TRUE;
	}
#endif
}
#else
/* Task Evaluation Function for Core-0 */
void FUN_SYS_Evaluate_Tasks_x1_0(void)
{
	FUN_SYS_Internal_Evaluate_Tasks_x1_0();
}

/* Task Evaluation Function for Core-1 */
void FUN_SYS_Evaluate_Tasks_x1_1(void)
{
	FUN_SYS_Internal_Evaluate_Tasks_x1_1();
}
#endif
/***********************************
 * Function to execute the tasks
 ***********************************/
#if (S_SYS_OS_DUAL_CORE_ENABLED != TRUE)
void FUN_SYS_Execute_Tasks(void)
{
#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
	if(SysOS_Ctrl.ExecPending == TRUE)
	{
#endif

		FUN_SYS_Internal_Execute_Tasks();

#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
		SysOS_Ctrl.ExecPending = FALSE; /* To avoid re-entrancy */
	}
#endif
}
#else
/* Execute task for Core-0 */
void FUN_SYS_Execute_Tasks_0(void)
{
	FUN_SYS_Internal_Execute_Tasks_0();
}

/* Execute task for Core-1 */
void FUN_SYS_Execute_Tasks_1(void)
{
	FUN_SYS_Internal_Execute_Tasks_1();
}
#endif
/***********************************
 * OS Timer ISR Function.
 * This function should be called from the
 * interrupt enabled ISR function of the
 * micro-controller timer.
 ***********************************/
#if (S_SYS_OS_DUAL_CORE_ENABLED != TRUE)
void FUN_SYS_Timer_x1(void)
{
	
	/* Evaluate the tasks */
	FUN_SYS_Evaluate_Tasks_x1();

#if (S_SYS_OS_METHOD == S_SYS_OS_INTERRUPT_METHOD)
	/* If it is Interrupt method, execute the task here itself */
	FUN_SYS_Execute_Tasks();
#endif
}
#else
/**
 * For Dual Core enabled projects, the Timer Tick should be given individually for cores
 */

/* SysOs Timer Tick call for Core-0 */
void FUN_SYS_Timer_x1_0(void)
{
	/* Evaluate the tasks */
	FUN_SYS_Evaluate_Tasks_x1_0();

	/* Execute the tasks */
	FUN_SYS_Execute_Tasks_0();
}

/* SysOs Timer Tick call for Core-1 */
void FUN_SYS_Timer_x1_1(void)
{
	/* Evaluate the tasks */
	FUN_SYS_Evaluate_Tasks_x1_1();

	/* Execute the tasks */
	FUN_SYS_Execute_Tasks_1();
}

#endif

#if (S_SYS_OS_MAIN_FUNCTION_TYPE == S_SYS_OS_DEFAULT_MAIN_FUNCTION_TYPE)
void SysOs_Main(void)
{

	/* Initialization function should be called first */
	PRC_SYS_INIT();
	while(!g_Fatal_Error_B)
	{
#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
		/* If it is a polling method, Execute the task cyclically.
		 * And wait for the Interrupt function to set the flag.
		 */
		FUN_SYS_Execute_Tasks();
#endif
	}
		
}
#endif

#if (S_SYS_OS_MAIN_FUNCTION_TYPE == S_SYS_OS_ARDUINO_MAIN_FUNCTION_TYPE)

void SysOs_Setup(void)
{
	
	/* Initialization function should be called first */
	PRC_SYS_INIT();

}

void SysOs_Main(void)
{
	while(!g_Fatal_Error_B)
	{
#if (S_SYS_OS_METHOD == S_SYS_OS_POLLING_METHOD)
		/* If it is a polling method, Execute the task cyclically.
		 * And wait for the Interrupt function to set the flag.
		 */
		FUN_SYS_Execute_Tasks();
#endif
	}
}

#endif

#if (S_SYS_OS_MAIN_FUNCTION_TYPE == S_SYS_OS_RTOS_MAIN_FUNCTION_TYPE)

void SysOs_Main(void)
{
	/* Initialization function should be called first */
	PRC_SYS_INIT();

	/* Initialize the Tick interrupt calls */
	esp_register_freertos_tick_hook_for_cpu(FUN_SYS_Timer_x1_0, P_CPU_0);
	esp_register_freertos_tick_hook_for_cpu(FUN_SYS_Timer_x1_1, P_CPU_1);

	/* Wait for the maximum time before exiting the SysOs Main to ensure all the tasks were scheduled properly */
	vTaskDelay((P_SYS_OS_TASK_FACTOR_MAX * P_SYS_OS_TICK * 2) / portTICK_PERIOD_MS);
}
#endif
