/*
 * SIMCOM_HTTP.h
 *
 *  Created on: 18-Apr-2021
 *      Author: Hari
 */

#ifndef _SIMCOM_SSLCONFIG_H_
#define _SIMCOM_SSLCONFIG_H_

/*************************/
/* Macro Definitions     */
/*************************/

//#define SIMCOM_HTTP_EstablishConnection()     SIMCOM_HTTP_Connect(TRUE)
//#define SIMCOM_HTTP_Disconnect()              SIMCOM_HTTP_Connect(FALSE)

//#define SIMCOM_IsHTTP_Error()                 (SIMCOM_HTTP_State == SIMCOM_HTTP_Error)

/*************************/
/* Data Type Definitions */
/*************************/

typedef enum
{
	SIMCOM_SSL_Configuration_Idle=0,
	SIMCOM_SSL_Init,
	SIMCOM_SSL_Authentication,
	SIMCOM_SSL_CaCert_Configuration,
	SIMCOM_SSL_Clientcert_Configuration,
	SIMCOM_SSL_Clientkey_Configuration,

	SIMCOM_SSL_Configured,
	SIMCOM_SSL_Configration_Error

}SIMCOM_SSL_Configuration_State_EN;

/*************************/
/* Global Variables      */
/*************************/

extern SIMCOM_SSL_Configuration_State_EN SIMCOM_SSL_Config_State;

/* [[DEBUG_MEASUREMENT */
/* NAME=SIMCOM_HTTP_State */
/* MODELNAME=SIMCOM_HTTP_State */
/* SIZE=4 */
/* HEADER=SIMCOM_H */
/* DEBUG_MEASUREMENT]] */

/*************************/
/* Function Declarations */
/*************************/


#endif /* _SIMCOM_SSLCONFIG_H_ */
