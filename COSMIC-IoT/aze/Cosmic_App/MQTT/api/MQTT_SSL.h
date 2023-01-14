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
	C_MQTT_SSL_Config_Init=0,
	C_MQTT_SSL_URCStopCmd,
	C_MQTT_SSL_DeleteCacert,
	C_MQTT_SSL_DeleteClientKey,
	C_MQTT_SSL_DeleteClientCert,
	C_MQTT_SSL_cacert,
	C_MQTT_SSL_UploadCacert,
	C_MQTT_SSLWaitfor_CACERT_EnterMsg,
	C_MQTT_SSL_ClientCert,
	C_MQTT_SSL_UploadClientCert,
	C_MQTT_SSLWaitfor_ClientCert_EnterMsg,
	C_MQTT_SSL_ClientKey,
	C_MQTT_SSL_UploadClientKey,
	C_MQTT_SSLWaitfor_ClientKey_EnterMsg,
	C_MQTT_SSL_CheckCertList,
	C_MQTT_SSL_Cert_Configured,
	C_MQTT_SSL_Cert_ConfigError,
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

extern BOOL IsSSLCertConfigured;

extern void SSL_Upload_Mainfunction(void);

#endif /* MQTT_SSL_H_ */