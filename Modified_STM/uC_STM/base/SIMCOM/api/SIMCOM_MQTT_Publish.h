/*
 * MQTT_Publish.h
 *
 * Created: 23-12-2022 11:45:39
 *  Author: Admin
 */ 


#ifndef MQTT_PUBLISH_H_
#define MQTT_PUBLISH_H_

#define IsMQTTPublishStateIdle()	(Publish_State == MQTT_Publish_Idle)

typedef enum
{
	MQTT_Publish_Idle=0,
	MQTT_PublishTopic_Config,
	MQTT_PubTopic_Name_Config,
	MQTT_UpdatePayloadLength,
	MQTT_Update_PubDataforTopic,
	MQTT_Publish_Msg,
	MQTTWaitforPublishResponse,
	MQTT_PublishMsgPublished
	
}MQTT_Publish_state_EN;


extern MQTT_Publish_state_EN Publish_State;

extern void MQTT_Publish_StateMachine(void);

extern char PublishPayload[28];

extern UBYTE MachineInitFlag;

#define PUBTOPICLength 16


#endif /* MQTT_PUBLISH_H_ */
