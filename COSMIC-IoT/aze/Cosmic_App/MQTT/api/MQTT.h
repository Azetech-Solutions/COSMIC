/*
 * MQTT.h
 *
 * Created: 23-12-2022 11:43:17
 *  Author: Admin
 */ 


#ifndef MQTT_H_
#define MQTT_H_

#define IsMQTT_Ready()	(MQTT_State==MQTT_Ready)

typedef enum
{
	MQTTSTART=0,
	MQTT_Accquire,
	MQTT_SSL_Configure,
	MQTT_Connect,
	MQTT_WaitForConnectResponce,
	MQTT_SubscribeTopic_Config,
	MQTT_SubTopic_Name_Config,
	MQTT_SubTopicNameUpdate,
	MQTT_WaitForSubResponce,
	

	MQTT_Ready,
	SIMCOM_MQTT_Connection_Error

}MQTT_State_EN;

extern MQTT_State_EN MQTT_State;

#define SUBTOPIC "prj01/cp0001/sub"
#define PUBTOPIC "prj01/cp0001/pub"

#endif /* MQTT_H_ */