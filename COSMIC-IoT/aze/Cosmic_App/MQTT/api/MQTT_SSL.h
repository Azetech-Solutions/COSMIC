/*
 * MQTT_SSL.h
 *
 * Created: 23-12-2022 11:44:25
 *  Author: Admin
 */ 


#ifndef MQTT_SSL_H_
#define MQTT_SSL_H_


typedef enum
{
	C_MQTT_SSL_Config_Idle=0,
	C_MQTT_SSL_Init,
	C_MQTT_SSL_Authentication,
	C_MQTT_SSL_CaCert_Configuration,
	C_MQTT_SSL_Clientcert_Configuration,
	C_MQTT_SSL_Clientkey_Configuration,

	C_MQTT_SSL_Configured,
	C_MQTT_SSL_Configration_Error

}C_MQTT_SSL_Configuration_State_EN;

/*************************/
/* Global Variables      */
/*************************/

#define IsMQTT_SSL_Configuration_Completed()		 (C_MQTT_SSL_Config_State == C_MQTT_SSL_Configured)

extern C_MQTT_SSL_Configuration_State_EN C_MQTT_SSL_Config_State;


#endif /* MQTT_SSL_H_ */