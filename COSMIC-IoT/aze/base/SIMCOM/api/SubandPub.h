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
	MQTT_PublishTopic_Config,
	MQTT_PubTopic_Name_Config,
	MQTT_SetPayload,
	MQTT_Update_PubData,
	MQTT_WaitForResponse,
	MQTT_Publish_Msg,
	MQTT_Sub_Msg

}MQTT_Subandpub_state_EN;

extern void MQTT_SubPub_StateMachine(void);

#endif /* SUBANDPUB_H_ */