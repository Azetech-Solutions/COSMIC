/*
 * MQTT_Header.h
 *
 * Created: 10-12-2022 16:19:10
 *  Author: Admin
 */ 


#ifndef MQTT_HEADER_H_
#define MQTT_HEADER_H_


#define ENTER_MESSAGE                     ">"
#define IsMQTT_Ready()	(MQTT_State==MQTT_Ready)

typedef enum
{
	MQTTSTART=0,
	MQTT_Accquire,
	MQTT_SSL_Configure,
	MQTT_SubscribeTopic_Config,
	MQTT_SubTopic_Name_Config,
	MQTT_SubTopicNameUpdate,
	MQTT_WaitForSubResponce,
	MQTT_Connect,

	MQTT_Ready,
	SIMCOM_MQTT_Connection_Error

}MQTT_State_EN;


static inline BOOL SIMCOM_IsResponse_Entermessage()
{
	BOOL retval = FALSE;

	if(SIMCOM_GetResponseLength() == 1)
	{
		if(strcmp(SIMCOM_ResponseBuffer,ENTER_MESSAGE) == 0)
		{
			retval = TRUE;
		}
	}

	return retval;
}


extern MQTT_State_EN MQTT_State;


#endif /* MQTT_HEADER_H_ */