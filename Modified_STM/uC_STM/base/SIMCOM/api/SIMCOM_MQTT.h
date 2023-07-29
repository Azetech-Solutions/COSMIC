/*
 * MQTT.h
 *
 * Created: 23-12-2022 11:43:17
 *  Author: Admin
 */ 


#ifndef MQTT_H_
#define MQTT_H_

#define IsMQTT_Ready()	(MQTT_State == MQTT_Ready)

typedef enum
{
	MQTTIdle = 0,
	MQTT_START,
	MQTT_Accquire,
	MQTT_ClientRelease,
	MQTT_SSL_Configure,
	MQTT_Connect,
	MQTT_Disconnect,
	MQTT_Connection_Check,
	MQTT_WaitForConnectResponce,
	MQTT_SubscribeTopic_Config,
	MQTT_SubTopic_Name_Config,
	MQTT_SubTopicNameUpdate,
	MQTT_WaitForSubResponce,
	MQTTSTOP,
	

	MQTT_Ready,
	SIMCOM_MQTT_Connection_Error

}MQTT_State_EN;
extern MQTT_State_EN MQTT_State;
extern unsigned char statechangecount;
#define SUBTOPIC "prj01/cp0001/sub"
#define PUBTOPIC "prj01/cp0001/pub"
//#define ENDPOINT "AT+CMQTTCONNECT=0,\"tcp://avruhytgmbwix-ats.iot.ap-south-1.amazonaws.com:8883\",59,1"
#define ENDPOINT "AT+CMQTTCONNECT=0,\"tcp://a1nwcqrp1qjjtm-ats.iot.ap-south-1.amazonaws.com:8883\",60,1"
#endif /* MQTT_H_ */