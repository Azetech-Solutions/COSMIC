/*
 * MQTT_Header.h
 *
 * Created: 10-12-2022 16:19:10
 *  Author: Admin
 */ 


#ifndef MQTT_HEADER_H_
#define MQTT_HEADER_H_



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


#endif /* MQTT_HEADER_H_ */