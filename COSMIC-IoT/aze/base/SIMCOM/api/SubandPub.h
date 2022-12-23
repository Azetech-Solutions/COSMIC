/*
 * SubandPub.h
 *
 * Created: 12-12-2022 19:21:59
 *  Author: Admin
 */ 


#ifndef SUBANDPUB_H_
#define SUBANDPUB_H_



typedef enum
{
	MQTT_SubPub_Idle=0,
	MQTT_PublishTopic1_Config,
	MQTT_PublishTopic2_Config,
	MQTT_PubTopic1_Name_Config,
	MQTT_PubTopic2_Name_Config,
	MQTT_SetPayloadforTopic1,
	MQTT_SetPayloadforTopic2,
	MQTT_Update_PubDataforTopic2,
	MQTT_Update_PubDataforTopic1,
	MQTT_Publish_Msg1,
	MQTT_Publish_Msg2,
	MQTTWaitforPublishResponse1,
	MQTTWaitforPublishResponse2,
	MQTT_Sub_Msg

}MQTT_Subandpub_state_EN;

extern MQTT_Subandpub_state_EN SubandPub_State;
extern void MQTT_SubPub_StateMachine(void);

#endif /* SUBANDPUB_H_ */