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

#if COMIF_MAX_SUPPORTED_DLC < 100
#error "The channel STM32 has more data length in the dynamic section than the supported DLC"
#endif

/* Transmit Message constants for the channel : STM32 */
#define P_COMIF_TXBUFFER_SIZE_STM32                    100u

/* Constants for the Tx message ADC_Values_AVR on STM32 channel */

#define ComIf_GetIndex_STM32_ADC_Values_AVR()                   (0u)
#define ComIf_GetLength_STM32_ADC_Values_AVR()                  (6u)
#define ComIf_GetShadowBuffer_STM32_ADC_Values_AVR()            ((UBYTE *)ComIf_ShadowBuffer_STM32_ADC_Values_AVR)
#define ComIf_Transmit_STM32_ADC_Values_AVR(x, y)               ComIf_Transmit(C_ComIfChannel_STM32, 0, ((void *)x), y)
#define ComIf_ForceTransmit_STM32_ADC_Values_AVR(x, y)          do{ComIfTxMsgCfg_STM32[0].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_STM32, 0, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_STM32_ADC_Values_AVR(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_STM32, 0, ((void *)x), y)
#define ComIf_TransmitFromBuffer_STM32_ADC_Values_AVR()         ComIf_Transmit_STM32_ADC_Values_AVR(ComIf_ShadowBuffer_STM32_ADC_Values_AVR, 6)
#define ComIf_ForceTransmitFromBuffer_STM32_ADC_Values_AVR()    ComIf_ForceTransmit_STM32_ADC_Values_AVR(ComIf_ShadowBuffer_STM32_ADC_Values_AVR, 6)

/* Debug Measurement Variables for ADC_Values_AVR on STM32 */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[0].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_STM32_ADC_Values_AVR_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[0].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_STM32_ADC_Values_AVR_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[0].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_STM32_ADC_Values_AVR_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message AVR_IO_Status on STM32 channel */

#define ComIf_GetIndex_STM32_AVR_IO_Status()                   (1u)
#define ComIf_GetLength_STM32_AVR_IO_Status()                  (2u)
#define ComIf_GetShadowBuffer_STM32_AVR_IO_Status()            ((UBYTE *)ComIf_ShadowBuffer_STM32_AVR_IO_Status)
#define ComIf_Transmit_STM32_AVR_IO_Status(x, y)               ComIf_Transmit(C_ComIfChannel_STM32, 1, ((void *)x), y)
#define ComIf_ForceTransmit_STM32_AVR_IO_Status(x, y)          do{ComIfTxMsgCfg_STM32[1].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_STM32, 1, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_STM32_AVR_IO_Status(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_STM32, 1, ((void *)x), y)
#define ComIf_TransmitFromBuffer_STM32_AVR_IO_Status()         ComIf_Transmit_STM32_AVR_IO_Status(ComIf_ShadowBuffer_STM32_AVR_IO_Status, 2)
#define ComIf_ForceTransmitFromBuffer_STM32_AVR_IO_Status()    ComIf_ForceTransmit_STM32_AVR_IO_Status(ComIf_ShadowBuffer_STM32_AVR_IO_Status, 2)

/* Debug Measurement Variables for AVR_IO_Status on STM32 */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[1].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_IO_Status_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[1].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_IO_Status_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[1].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_IO_Status_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message AVR_Message on STM32 channel */

#define ComIf_GetIndex_STM32_AVR_Message()                   (2u)
#define ComIf_GetLength_STM32_AVR_Message()                  (51u)
#define ComIf_GetShadowBuffer_STM32_AVR_Message()            ((UBYTE *)ComIf_ShadowBuffer_STM32_AVR_Message)
#define ComIf_Transmit_STM32_AVR_Message(x, y)               ComIf_Transmit(C_ComIfChannel_STM32, 2, ((void *)x), y)
#define ComIf_ForceTransmit_STM32_AVR_Message(x, y)          do{ComIfTxMsgCfg_STM32[2].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_STM32, 2, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_STM32_AVR_Message(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_STM32, 2, ((void *)x), y)
#define ComIf_TransmitFromBuffer_STM32_AVR_Message()         ComIf_Transmit_STM32_AVR_Message(ComIf_ShadowBuffer_STM32_AVR_Message, 51)
#define ComIf_ForceTransmitFromBuffer_STM32_AVR_Message()    ComIf_ForceTransmit_STM32_AVR_Message(ComIf_ShadowBuffer_STM32_AVR_Message, 51)

/* Debug Measurement Variables for AVR_Message on STM32 */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[2].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Message_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[2].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Message_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[2].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Message_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Constants for the Tx message AVR_Call on STM32 channel */

#define ComIf_GetIndex_STM32_AVR_Call()                   (3u)
#define ComIf_GetLength_STM32_AVR_Call()                  (1u)
#define ComIf_GetShadowBuffer_STM32_AVR_Call()            ((UBYTE *)ComIf_ShadowBuffer_STM32_AVR_Call)
#define ComIf_Transmit_STM32_AVR_Call(x, y)               ComIf_Transmit(C_ComIfChannel_STM32, 3, ((void *)x), y)
#define ComIf_ForceTransmit_STM32_AVR_Call(x, y)          do{ComIfTxMsgCfg_STM32[3].TxFlags.ForceTransmit = TRUE; ComIf_Transmit(C_ComIfChannel_STM32, 3, ((void *)x), y); } while(0)
#define ComIf_UpdateTxShadowBuffer_STM32_AVR_Call(x, y)   ComIf_UpdateTxShadowBuffer(C_ComIfChannel_STM32, 3, ((void *)x), y)
#define ComIf_TransmitFromBuffer_STM32_AVR_Call()         ComIf_Transmit_STM32_AVR_Call(ComIf_ShadowBuffer_STM32_AVR_Call, 1)
#define ComIf_ForceTransmitFromBuffer_STM32_AVR_Call()    ComIf_ForceTransmit_STM32_AVR_Call(ComIf_ShadowBuffer_STM32_AVR_Call, 1)

/* Debug Measurement Variables for AVR_Call on STM32 */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[3].TxFlags.PendingForTransmission */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Call_PendingForTransmission */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[3].TxFlags.PendingForConfirmation */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Call_PendingForConfirmation */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */

/* [[DEBUG_MEASUREMENT */
/* NAME=ComIfTxMsgCfg_STM32[3].TxFlags.AggregatedInBuffer */
/* MODELNAME=ComIfTxMsgCfg_STM32_AVR_Call_AggregatedInBuffer */
/* SIZE=1 */
/* HEADER=ComIf_Config.h */
/* DEBUG_MEASUREMENT]] */


/* Receive Message constants for the channel : STM32 */

/* Constants for the Rx message IO_cmdData_AVR on STM32 channel */

#define ComIf_GetIndex_STM32_IO_cmdData_AVR()                   (0u)
#define ComIf_GetLength_STM32_IO_cmdData_AVR()                  (2u)
#define ComIf_GetBuffer_STM32_IO_cmdData_AVR()                  ((UBYTE *)ComIf_RxMessageBuffer_STM32_IO_cmdData_AVR)
#define ComIf_GetRxFlags_STM32_IO_cmdData_AVR()                 (ComIfRxMsgCfg_STM32[0].RxFlags)
#define ComIf_IsNewMessageReceived_STM32_IO_cmdData_AVR()       ((ComIfRxMsgCfg_STM32[0].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_STM32[0].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_STM32_IO_cmdData_AVR() (ComIfRxMsgCfg_STM32[0].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_STM32_IO_cmdData_AVR()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_STM32[0].RxFlags))

/* Constants for the Rx message DTMF_Command_Data on STM32 channel */

#define ComIf_GetIndex_STM32_DTMF_Command_Data()                   (1u)
#define ComIf_GetLength_STM32_DTMF_Command_Data()                  (2u)
#define ComIf_GetBuffer_STM32_DTMF_Command_Data()                  ((UBYTE *)ComIf_RxMessageBuffer_STM32_DTMF_Command_Data)
#define ComIf_GetRxFlags_STM32_DTMF_Command_Data()                 (ComIfRxMsgCfg_STM32[1].RxFlags)
#define ComIf_IsNewMessageReceived_STM32_DTMF_Command_Data()       ((ComIfRxMsgCfg_STM32[1].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_STM32[1].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_STM32_DTMF_Command_Data() (ComIfRxMsgCfg_STM32[1].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_STM32_DTMF_Command_Data()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_STM32[1].RxFlags))

/* Constants for the Rx message Stored_MobNums on STM32 channel */

#define ComIf_GetIndex_STM32_Stored_MobNums()                   (2u)
#define ComIf_GetLength_STM32_Stored_MobNums()                  (78u)
#define ComIf_GetBuffer_STM32_Stored_MobNums()                  ((UBYTE *)ComIf_RxMessageBuffer_STM32_Stored_MobNums)
#define ComIf_GetRxFlags_STM32_Stored_MobNums()                 (ComIfRxMsgCfg_STM32[2].RxFlags)
#define ComIf_IsNewMessageReceived_STM32_Stored_MobNums()       ((ComIfRxMsgCfg_STM32[2].RxFlags.NewMessageReceived == TRUE) && (ComIfRxMsgCfg_STM32[2].RxFlags.ReceptionStarted == FALSE))
#define ComIf_IsMessageReceptionHasError_STM32_Stored_MobNums() (ComIfRxMsgCfg_STM32[2].RxFlags.ErrorInReception)
#define ComIf_SetMessageProcessesed_STM32_Stored_MobNums()      ComIf_SetMessageProcessed(&(ComIfRxMsgCfg_STM32[2].RxFlags))



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
	C_ComIfChannel_STM32 = 0,

	C_ComIfChannel_TOTAL = 1
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

/* For the Channel: STM32 */
extern ComIfTxMessageConfig ComIfTxMsgCfg_STM32[];
extern UBYTE ComIfChannelTxBuffer_STM32[];
extern UBYTE ComIf_ShadowBuffer_STM32_ADC_Values_AVR[];
extern UBYTE ComIf_ShadowBuffer_STM32_AVR_IO_Status[];
extern UBYTE ComIf_ShadowBuffer_STM32_AVR_Message[];
extern UBYTE ComIf_ShadowBuffer_STM32_AVR_Call[];
extern ComIfRxMessageConfig ComIfRxMsgCfg_STM32[];
extern UBYTE ComIf_RxMessageBuffer_STM32_IO_cmdData_AVR[];
extern UBYTE ComIf_RxMessageBuffer_STM32_DTMF_Command_Data[];
extern UBYTE ComIf_RxMessageBuffer_STM32_Stored_MobNums[];

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
extern UBYTE ComIf_RxIndication_STM32(UBYTE DataByte);

#endif /* __COMIF_CONFIG_H__ */

