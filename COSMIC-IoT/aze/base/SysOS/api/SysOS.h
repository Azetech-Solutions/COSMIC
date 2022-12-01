/*
 * SysOS.h
 *
 *  Created on: 12 Feb., 2017
 *      Author: Hari
 */

#ifndef _SYSOS_H_
#define _SYSOS_H_

/********************************************/
/*                 Includes                 */
/********************************************/
#include "Platform_Types.h" /* Include platform types */

#include "SysOS_Config_Elements.h" // Include Config Elements before Config
#include "SysOs_Config.h"          // Include Config header

/* Configuration specific inclusions */
#if (S_SYS_OS_METHOD == S_SYS_OS_RTOS_METHOD)
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#endif

/********************************************/
/*            Type Definitions              */
/********************************************/

typedef void (*PRCPTR)(void);

/********************************************/
/*      Extern Function Declarations        */
/********************************************/

#if (S_SYS_OS_DUAL_CORE_ENABLED != TRUE)
extern void FUN_SYS_Timer_x1(void);
#else
extern void FUN_SYS_Timer_x1_0(void);
extern void FUN_SYS_Timer_x1_1(void);
#endif

#if (S_SYS_OS_MAIN_FUNCTION_TYPE == S_SYS_OS_ARDUINO_MAIN_FUNCTION_TYPE)
extern void SysOs_Setup(void);
#endif

extern void SysOs_Main(void);

#if (S_SYS_OS_METHOD == S_SYS_OS_RTOS_METHOD)
extern void SysOs_ExecuteTask(TaskHandle_t xHandle);
#endif

#endif /* _SYSOS_H_ */
