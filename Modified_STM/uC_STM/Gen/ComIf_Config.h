/******************************************************************
                A Z E T E C H    S O L U T I O N S                 
-------------------------------------------------------------------
        T O O L   G E N E R A T E D   S O U R C E   C O D E        
                    D O   N O T   M O D I F Y                      
-------------------------------------------------------------------
 This file is generated with the Azetech copyrighted software tool 
 No modification should be done in this file without a written     
 consent from Azetech Solutions. Incase of any such instances,     
 Azetech Solutions will not be liable to any damages it cause.     
-------------------------------------------------------------------
 Any Distribution, Alteration, Removal, Disassemble, Decompile or  
 or Reverse Engineer the given software, without written consent   
 from Azetech Solutions by the Licensee or third parties were not  
 allowed. Such instance is considered as a breach and the Licensee 
 is the sole responsible and liable to any damages it cause.       
******************************************************************/

#ifndef __COMIF_CONFIG_H__
#define __COMIF_CONFIG_H__

#include "Platform_Types.h"
#include "ComIf_Config_Elements.h"

/*************************************************/
/* Functional Switches and Parameter Definitions */
/*************************************************/

/* Invalid Index handle */
#define P_COMIF_INVALID_HANDLE                255u

/* ComIf Input Method */
#define S_COMIF_INPUT_METHOD                  COMIF_INPUT_METHOD_POLLING

#define COMIF_MAX_SUPPORTED_DLC               255 // 255 Bytes

#define COMIF_MAX_SUPPORTED_CHKLEN            1   // 1 Byte

#define COMIF_TX_MAIN_FUNCTION_PERIOD         10

#define COMIF_RX_MAIN_FUNCTION_PERIOD         10

/*************************************************/
/*               Macro Definitions               */
/*************************************************/

#if COMIF_MAX_SUPPORTED_DLC < 64
#error "The channel Cloud has more data length in the dynamic section than the supported DLC"
#endif

/* Transmit Message constants for the channel : Cloud */
#define P_COMIF_TXBUFFER_SIZE_CLOUD                    64u

/* Constants for the Tx message IO_Status_Cloud on Cloud channel */

#define ComIf_GetIndex_Cloud_IO_Status_Cloud()                   (0u)
#define ComIf_GetLength_Cloud_IO_Status_Cloud()                  (2u)
#define ComIf_GetShadowBuffer_Cloud_IO_Status_Cloud()            ((UBYTE *)ComIf_ShadowBuffer_Cloud_IO_Status_Cloud)
#define ComIf_Transmit_Cloud_IO_Status_Cloud(x, y)               ComIf_Transmit(C_ComIfChannel_Cloud, 0, ((void *)x), y)
#define ComIf_ForceTransmit_Cloud_IO_Status_Cloud(x, y)          do{ComIfTxMsgCfg_Cloud[0].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_Cloud, 0, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_Cloud_IO_Status_Cloud(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_Cloud, 0, ((void *)x), y)
#define ComIf_TransmitFromBuffer_Cloud_IO_Status_Cloud()         ComIf_Transmit_Cloud_IO_Status_Cloud(ComIf_ShadowBuffer_Cloud_IO_Status_Cloud, 2)
#define ComIf_ForceTransmitFromBuffer_Cloud_IO_Status_Cloud()    ComIf_ForceTransmit_Cloud_IO_Status_Cloud(ComIf_ShadowBuffer_Cloud_IO_Status_Cloud, 2)

/* Debug Measurement Variables for IO_Status_Cloud on Cloud */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[0].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_Cloud_IO_Status_Cloud_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[0].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_Cloud_IO_Status_Cloud_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[0].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_Cloud_IO_Status_Cloud_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message ADC_Values_Cloud on Cloud channel */

#define ComIf_GetIndex_Cloud_ADC_Values_Cloud()                   (1u)
#define ComIf_GetLength_Cloud_ADC_Values_Cloud()                  (6u)
#define ComIf_GetShadowBuffer_Cloud_ADC_Values_Cloud()            ((UBYTE *)ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud)
#define ComIf_Transmit_Cloud_ADC_Values_Cloud(x, y)               ComIf_Transmit(C_ComIfChannel_Cloud, 1, ((void *)x), y)
#define ComIf_ForceTransmit_Cloud_ADC_Values_Cloud(x, y)          do{ComIfTxMsgCfg_Cloud[1].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_Cloud, 1, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_Cloud_ADC_Values_Cloud(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_Cloud, 1, ((void *)x), y)
#define ComIf_TransmitFromBuffer_Cloud_ADC_Values_Cloud()         ComIf_Transmit_Cloud_ADC_Values_Cloud(ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud, 6)
#define ComIf_ForceTransmitFromBuffer_Cloud_ADC_Values_Cloud()    ComIf_ForceTransmit_Cloud_ADC_Values_Cloud(ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud, 6)

/* Debug Measurement Variables for ADC_Values_Cloud on Cloud */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[1].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_Cloud_ADC_Values_Cloud_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[1].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_Cloud_ADC_Values_Cloud_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[1].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_Cloud_ADC_Values_Cloud_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message Cloud_Init on Cloud channel */

#define ComIf_GetIndex_Cloud_Cloud_Init()                   (2u)
#define ComIf_GetLength_Cloud_Cloud_Init()                  (2u)
#define ComIf_GetShadowBuffer_Cloud_Cloud_Init()            ((UBYTE *)ComIf_ShadowBuffer_Cloud_Cloud_Init)
#define ComIf_Transmit_Cloud_Cloud_Init(x, y)               ComIf_Transmit(C_ComIfChannel_Cloud, 2, ((void *)x), y)
#define ComIf_ForceTransmit_Cloud_Cloud_Init(x, y)          do{ComIfTxMsgCfg_Cloud[2].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_Cloud, 2, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_Cloud_Cloud_Init(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_Cloud, 2, ((void *)x), y)
#define ComIf_TransmitFromBuffer_Cloud_Cloud_Init()         ComIf_Transmit_Cloud_Cloud_Init(ComIf_ShadowBuffer_Cloud_Cloud_Init, 2)
#define ComIf_ForceTransmitFromBuffer_Cloud_Cloud_Init()    ComIf_ForceTransmit_Cloud_Cloud_Init(ComIf_ShadowBuffer_Cloud_Cloud_Init, 2)

/* Debug Measurement Variables for Cloud_Init on Cloud */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[2].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_Cloud_Cloud_Init_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[2].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_Cloud_Cloud_Init_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_Cloud[2].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_Cloud_Cloud_Init_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Receive Message constants for the channel : Cloud */

/* Constants for the Rx message IO_cmdData_Cloud on Cloud channel */

#define ComIf_GetIndex_Cloud_IO_cmdData_Cloud()                   (0u)
#define ComIf_GetLength_Cloud_IO_cmdData_Cloud()                  (8u)
#define ComIf_GetBuffer_Cloud_IO_cmdData_Cloud()                  ((UBYTE *)ComIf_RxMessageBuffer_Cloud_IO_cmdData_Cloud)
#define ComIf_GetRxFlags_Cloud_IO_cmdData_Cloud()                 (ComIfRxMsgCfg_Cloud[0].RxFlags)
#define ComIf_IsNewMessageReceived_Cloud_IO_cmdData_Cloud()       ((ComIfRxMsgCfg_Cloud[0].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_Cloud[0].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_Cloud_IO_cmdData_Cloud() (ComIfRxMsgCfg_Cloud[0].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_Cloud_IO_cmdData_Cloud()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_Cloud[0].RxFlags))



#if COMIF_MAX_SUPPORTED_DLC < 100
#error "The channel AVR has more data length in the dynamic section than the supported DLC"
#endif

/* Transmit Message constants for the channel : AVR */
#define P_COMIF_TXBUFFER_SIZE_AVR                    100u

/* Constants for the Tx message IO_cmdData_AVR on AVR channel */

#define ComIf_GetIndex_AVR_IO_cmdData_AVR()                   (0u)
#define ComIf_GetLength_AVR_IO_cmdData_AVR()                  (2u)
#define ComIf_GetShadowBuffer_AVR_IO_cmdData_AVR()            ((UBYTE *)ComIf_ShadowBuffer_AVR_IO_cmdData_AVR)
#define ComIf_Transmit_AVR_IO_cmdData_AVR(x, y)               ComIf_Transmit(C_ComIfChannel_AVR, 0, ((void *)x), y)
#define ComIf_ForceTransmit_AVR_IO_cmdData_AVR(x, y)          do{ComIfTxMsgCfg_AVR[0].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_AVR, 0, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_AVR_IO_cmdData_AVR(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_AVR, 0, ((void *)x), y)
#define ComIf_TransmitFromBuffer_AVR_IO_cmdData_AVR()         ComIf_Transmit_AVR_IO_cmdData_AVR(ComIf_ShadowBuffer_AVR_IO_cmdData_AVR, 2)
#define ComIf_ForceTransmitFromBuffer_AVR_IO_cmdData_AVR()    ComIf_ForceTransmit_AVR_IO_cmdData_AVR(ComIf_ShadowBuffer_AVR_IO_cmdData_AVR, 2)

/* Debug Measurement Variables for IO_cmdData_AVR on AVR */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_AVR[0].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_AVR_IO_cmdData_AVR_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_AVR[0].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_AVR_IO_cmdData_AVR_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_AVR[0].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_AVR_IO_cmdData_AVR_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Receive Message constants for the channel : AVR */

/* Constants for the Rx message AVR_IO_Status on AVR channel */

#define ComIf_GetIndex_AVR_AVR_IO_Status()                   (0u)
#define ComIf_GetLength_AVR_AVR_IO_Status()                  (2u)
#define ComIf_GetBuffer_AVR_AVR_IO_Status()                  ((UBYTE *)ComIf_RxMessageBuffer_AVR_AVR_IO_Status)
#define ComIf_GetRxFlags_AVR_AVR_IO_Status()                 (ComIfRxMsgCfg_AVR[0].RxFlags)
#define ComIf_IsNewMessageReceived_AVR_AVR_IO_Status()       ((ComIfRxMsgCfg_AVR[0].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_AVR[0].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_AVR_AVR_IO_Status() (ComIfRxMsgCfg_AVR[0].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_AVR_AVR_IO_Status()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_AVR[0].RxFlags))

/* Constants for the Rx message ADC_Values_AVR on AVR channel */

#define ComIf_GetIndex_AVR_ADC_Values_AVR()                   (1u)
#define ComIf_GetLength_AVR_ADC_Values_AVR()                  (6u)
#define ComIf_GetBuffer_AVR_ADC_Values_AVR()                  ((UBYTE *)ComIf_RxMessageBuffer_AVR_ADC_Values_AVR)
#define ComIf_GetRxFlags_AVR_ADC_Values_AVR()                 (ComIfRxMsgCfg_AVR[1].RxFlags)
#define ComIf_IsNewMessageReceived_AVR_ADC_Values_AVR()       ((ComIfRxMsgCfg_AVR[1].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_AVR[1].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_AVR_ADC_Values_AVR() (ComIfRxMsgCfg_AVR[1].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_AVR_ADC_Values_AVR()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_AVR[1].RxFlags))



#if COMIF_MAX_SUPPORTED_DLC < 64
#error "The channel DTMF has more data length in the dynamic section than the supported DLC"
#endif

/* Transmit Message constants for the channel : DTMF */
#define P_COMIF_TXBUFFER_SIZE_DTMF                    64u

/* Constants for the Tx message DTMFCommandData on DTMF channel */

#define ComIf_GetIndex_DTMF_DTMFCommandData()                   (0u)
#define ComIf_GetLength_DTMF_DTMFCommandData()                  (2u)
#define ComIf_GetShadowBuffer_DTMF_DTMFCommandData()            ((UBYTE *)ComIf_ShadowBuffer_DTMF_DTMFCommandData)
#define ComIf_Transmit_DTMF_DTMFCommandData(x, y)               ComIf_Transmit(C_ComIfChannel_DTMF, 0, ((void *)x), y)
#define ComIf_ForceTransmit_DTMF_DTMFCommandData(x, y)          do{ComIfTxMsgCfg_DTMF[0].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_DTMF, 0, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_DTMF_DTMFCommandData(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_DTMF, 0, ((void *)x), y)
#define ComIf_TransmitFromBuffer_DTMF_DTMFCommandData()         ComIf_Transmit_DTMF_DTMFCommandData(ComIf_ShadowBuffer_DTMF_DTMFCommandData, 2)
#define ComIf_ForceTransmitFromBuffer_DTMF_DTMFCommandData()    ComIf_ForceTransmit_DTMF_DTMFCommandData(ComIf_ShadowBuffer_DTMF_DTMFCommandData, 2)

/* Debug Measurement Variables for DTMFCommandData on DTMF */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[0].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_DTMF_DTMFCommandData_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[0].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_DTMF_DTMFCommandData_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[0].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_DTMF_DTMFCommandData_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message MN_Data_Message on DTMF channel */

#define ComIf_GetIndex_DTMF_MN_Data_Message()                   (1u)
#define ComIf_GetLength_DTMF_MN_Data_Message()                  (78u)
#define ComIf_GetShadowBuffer_DTMF_MN_Data_Message()            ((UBYTE *)ComIf_ShadowBuffer_DTMF_MN_Data_Message)
#define ComIf_Transmit_DTMF_MN_Data_Message(x, y)               ComIf_Transmit(C_ComIfChannel_DTMF, 1, ((void *)x), y)
#define ComIf_ForceTransmit_DTMF_MN_Data_Message(x, y)          do{ComIfTxMsgCfg_DTMF[1].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_DTMF, 1, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_DTMF_MN_Data_Message(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_DTMF, 1, ((void *)x), y)
#define ComIf_TransmitFromBuffer_DTMF_MN_Data_Message()         ComIf_Transmit_DTMF_MN_Data_Message(ComIf_ShadowBuffer_DTMF_MN_Data_Message, 78)
#define ComIf_ForceTransmitFromBuffer_DTMF_MN_Data_Message()    ComIf_ForceTransmit_DTMF_MN_Data_Message(ComIf_ShadowBuffer_DTMF_MN_Data_Message, 78)

/* Debug Measurement Variables for MN_Data_Message on DTMF */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[1].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_DTMF_MN_Data_Message_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[1].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_DTMF_MN_Data_Message_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_DTMF[1].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_DTMF_MN_Data_Message_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Receive Message constants for the channel : DTMF */

/* Constants for the Rx message AVR_Message on DTMF channel */

#define ComIf_GetIndex_DTMF_AVR_Message()                   (0u)
#define ComIf_GetLength_DTMF_AVR_Message()                  (51u)
#define ComIf_GetBuffer_DTMF_AVR_Message()                  ((UBYTE *)ComIf_RxMessageBuffer_DTMF_AVR_Message)
#define ComIf_GetRxFlags_DTMF_AVR_Message()                 (ComIfRxMsgCfg_DTMF[0].RxFlags)
#define ComIf_IsNewMessageReceived_DTMF_AVR_Message()       ((ComIfRxMsgCfg_DTMF[0].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_DTMF[0].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_DTMF_AVR_Message() (ComIfRxMsgCfg_DTMF[0].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_DTMF_AVR_Message()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_DTMF[0].RxFlags))

/* Constants for the Rx message AVR_Call on DTMF channel */

#define ComIf_GetIndex_DTMF_AVR_Call()                   (1u)
#define ComIf_GetLength_DTMF_AVR_Call()                  (1u)
#define ComIf_GetBuffer_DTMF_AVR_Call()                  ((UBYTE *)ComIf_RxMessageBuffer_DTMF_AVR_Call)
#define ComIf_GetRxFlags_DTMF_AVR_Call()                 (ComIfRxMsgCfg_DTMF[1].RxFlags)
#define ComIf_IsNewMessageReceived_DTMF_AVR_Call()       ((ComIfRxMsgCfg_DTMF[1].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_DTMF[1].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_DTMF_AVR_Call() (ComIfRxMsgCfg_DTMF[1].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_DTMF_AVR_Call()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_DTMF[1].RxFlags))



#define COMIF_DYNAMIC_DATA_LENGTH_ENABLED

/*************************************************/
/*               Type Definitions                */
/*************************************************/

typedef struct __ComIfTxFlags
{
    /* This structure must not exceed 8 bytes */
    
    /* Control Bits */
	UBYTE EnableAggregation      : 1; // If set, then the message will be aggregated instead of transmitting
	UBYTE ForceTransmit          : 1; // If set, then the aggregation enabled messages will be sent directly
	
	/* Status Bits */
	UBYTE PendingForTransmission : 1; // If set, then the message will be sent in next aggregation cycle
	UBYTE AggregatedInBuffer     : 1; // If set, then the message has been copied to the channel data buffer
	UBYTE PendingForConfirmation : 1; // If set, then the message will receive confirmation after the aggregation transmit cycle is completed
	
}ComIfTxFlags;

typedef struct __ComIfTxMessageConfig
{
	UBYTE ID;
	UBYTE Length;
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
	UBYTE DynLength;
#endif
	UBYTE ChecksumLength;
	ComIfTxFlags TxFlags;
	UBYTE * ShadowBuffer;
	void (*TxConfCallback)(void);
	UBYTE (*TxCallback)(UBYTE *, UBYTE *);
}ComIfTxMessageConfig;

typedef struct __ComIfRxFlags
{
    /* This structure must not exceed 8 bytes */
    
	/* Status Bits */
	UBYTE ReceptionStarted   : 1; // If set, then the message has been started receiving
	UBYTE NewMessageReceived : 1; // If set, then the message has been received completely and waiting for the RxCbk to be called
	UBYTE ErrorInReception   : 1; // If set, then the message has been received, but there is an error in reception
	UBYTE RxRequest          : 1; // If set, then the message is requested by the application
	UBYTE RxRequestPlaced    : 1; // If set, then the message reception request has been placed to the underlying module
	
}ComIfRxFlags;

typedef struct __ComIfRxMessageConfig
{
	UBYTE ID;
	UBYTE Length;
	UBYTE ChecksumLength;
	ComIfRxFlags RxFlags;
	UBYTE CurRxngIdx;
	UBYTE * MessageBuffer;
	void (*RxCallback)(UBYTE, UBYTE*);
}ComIfRxMessageConfig;

typedef enum __ComIf_ChannelType
{
	C_ComIfChannel_Cloud = 0,
	C_ComIfChannel_AVR = 1,
	C_ComIfChannel_DTMF = 2,

	C_ComIfChannel_TOTAL = 3
}ComIf_ChannelType;

typedef UBYTE (*ComIf_ChannelTransmitType)(UWORD, void*);

typedef void (*ComIf_ErrorNotificationType)(ULONG, ULONG);

typedef UBYTE (*ComIf_ChannelRxRequestType)(UBYTE);

typedef struct __ComIfChannelFlags
{
	/* Reception related flag */
	UBYTE IsReceiving          : 1;
	UBYTE Delimit              : 1;
	UBYTE DLCVerified          : 1;
	UBYTE IsRxRequestEnabled   : 1;
	
	UBYTE IsTransmitting       : 1;
	UBYTE IsAggregationEnabled : 1;
	UBYTE HasAggregatedMsgs    : 1;
	
}ComIfChannelFlags;

#ifdef COMIF_TX_AGGREGATION_ENABLED
typedef struct __ComIf_TxAggCtrl_t
{
	UWORD TxCycle;
	UWORD TxCycleCounter;

	UWORD BufferLimit;
	UWORD CurrentBufferLength;
	
}ComIf_TxAggCtrl_t;
#endif

typedef struct __ComIf_ChannelConfigType
{
	ComIf_ChannelTransmitType Transmit;
	ComIf_ErrorNotificationType ErrorCallback;

#ifdef COMIF_RX_REQUEST_ENABLED
	ComIf_ChannelRxRequestType RequestReception;
	UWORD RxRequestTimeout;
	UWORD RxRequestTimeoutCounter;
#endif

	UBYTE * TxBuffer;
	UWORD TxBufferLength;
	
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
	UBYTE DynamicMaxDataLength;
#endif
	UBYTE DataTxfrMethod;
	
	ComIfTxMessageConfig * TxMessages;
	UBYTE TotalTxMessages;
	
	ComIfRxMessageConfig * RxMessages;
	UBYTE TotalRxMessages;

	UBYTE RxMsgIndex;
	UBYTE RxMsgLength;
	ComIfChannelFlags ChannelFlags;

#ifdef COMIF_TX_AGGREGATION_ENABLED
	ComIf_TxAggCtrl_t * TxAggCtrl;
#endif
	
}ComIf_ChannelConfigType;

/*************************************************/
/*               Global Variables                */
/*************************************************/

extern ComIf_ChannelConfigType ComIf_ChannelConfig[];

/* Channel Buffers and Configuration */

/* For the Channel: Cloud */
extern ComIfTxMessageConfig ComIfTxMsgCfg_Cloud[];
extern UBYTE ComIfChannelTxBuffer_Cloud[];
extern UBYTE ComIf_ShadowBuffer_Cloud_IO_Status_Cloud[];
extern UBYTE ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud[];
extern UBYTE ComIf_ShadowBuffer_Cloud_Cloud_Init[];
extern ComIfRxMessageConfig ComIfRxMsgCfg_Cloud[];
extern UBYTE ComIf_RxMessageBuffer_Cloud_IO_cmdData_Cloud[];

/* For the Channel: AVR */
extern ComIfTxMessageConfig ComIfTxMsgCfg_AVR[];
extern UBYTE ComIfChannelTxBuffer_AVR[];
extern UBYTE ComIf_ShadowBuffer_AVR_IO_cmdData_AVR[];
extern ComIfRxMessageConfig ComIfRxMsgCfg_AVR[];
extern UBYTE ComIf_RxMessageBuffer_AVR_AVR_IO_Status[];
extern UBYTE ComIf_RxMessageBuffer_AVR_ADC_Values_AVR[];

/* For the Channel: DTMF */
extern ComIfTxMessageConfig ComIfTxMsgCfg_DTMF[];
extern UBYTE ComIfChannelTxBuffer_DTMF[];
extern UBYTE ComIf_ShadowBuffer_DTMF_DTMFCommandData[];
extern UBYTE ComIf_ShadowBuffer_DTMF_MN_Data_Message[];
extern ComIfRxMessageConfig ComIfRxMsgCfg_DTMF[];
extern UBYTE ComIf_RxMessageBuffer_DTMF_AVR_Message[];
extern UBYTE ComIf_RxMessageBuffer_DTMF_AVR_Call[];

/**********************************************************/
/* Inline Function Definitions                            */
/**********************************************************/

static inline void ComIf_SetMessageProcessed(ComIfRxFlags * RxFlags)
{
	if(RxFlags != NULL)
	{
		RxFlags->NewMessageReceived = FALSE;
#ifdef COMIF_RX_REQUEST_ENABLED
		RxFlags->RxRequest = FALSE;
		RxFlags->RxRequestPlaced = FALSE;
#endif
	}
}

/*************************************************/
/*          Extern Function Declarations         */
/*************************************************/

extern UBYTE ComIf_Rx_Request(ComIf_ChannelType ChannelIndex, UBYTE MsgIndex);
extern UBYTE ComIf_Rx_CancelRequest(ComIf_ChannelType ChannelIndex, UBYTE MsgIndex);

/* Channel Specific Rx Indication Functions */
extern UBYTE ComIf_RxIndication_Cloud(char * DataString, UWORD Length);
extern UBYTE ComIf_RxIndication_AVR(UBYTE DataByte);
extern UBYTE ComIf_RxIndication_DTMF(UBYTE DataByte);

#endif /* __COMIF_CONFIG_H__ */

