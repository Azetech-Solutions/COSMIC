/*
 * SysOs_Config_Elements.h
 *
 *  Created on: 12 Feb., 2017
 *      Author: Hari
 */

#ifndef CORE_SYSOS_API_SYSOS_CONFIG_ELEMENTS_H_
#define CORE_SYSOS_API_SYSOS_CONFIG_ELEMENTS_H_

/* Switch settings elements */

/********************************/
/* Elements for S_SYS_OS_METHOD */
/********************************/
/*********************************************************************************
 * Polling method of execution uses the SYS_Timer Interrupt function
 * to set a flag, which will be polled in the while loop to execute the
 * tasks. This is suitable for the micro-controllers without nested
 * interrupt concept, or projects enabled with WDG timers, or projects
 * which executes a lot of instructions in the tasks like LCD / EEPROM emulation.
 *********************************************************************************/
#define S_SYS_OS_POLLING_METHOD       1

/*********************************************************************************
 * Interrupt method of execution uses the interrupt function of the SYS_Timer to
 * execute the tasks. This is the fastest method of execution and can be used for
 * the controllers with nested interrupt concept, or the projects which does not have
 * any other interrupts configured, or the projects with time critical applications.
 *********************************************************************************/
#define S_SYS_OS_INTERRUPT_METHOD     2

/*********************************************************************************
 * RTOS method of execution uses the RTOS Tick callback function for the SYS_Timer to
 * execute the tasks. This is used for the RTOSes that doesn't support time slicing.
 * This method or the SysOs itself shall be integrated alongside RTOS only when
 * the deterministic time slicing scheduling algorithm is needed and not supported
 * in the underlying RTOS framework.
 *********************************************************************************/
#define S_SYS_OS_RTOS_METHOD          3



#define S_SYS_OS_DEFAULT_MAIN_FUNCTION_TYPE      1
#define S_SYS_OS_ARDUINO_MAIN_FUNCTION_TYPE      2
#define S_SYS_OS_RTOS_MAIN_FUNCTION_TYPE         3

#endif /* CORE_SYSOS_API_SYSOS_CONFIG_ELEMENTS_H_ */
