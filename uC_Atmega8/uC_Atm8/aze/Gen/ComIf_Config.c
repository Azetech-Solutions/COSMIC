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

#include "ComIf.h"

/******************************************************/
/*           Global Variable Definitions              */
/******************************************************/

/* Channel Declarations */

/* -------------------------------------------------- */
/* For STM32 Channel */
/* -------------------------------------------------- */

extern UBYTE STM32_Transmit(UWORD Length, void * Data);
extern void STM32_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1);

UBYTE ComIfChannelTxBuffer_STM32[100];

UBYTE ComIf_ShadowBuffer_STM32_ADC_Values_AVR[6];
UBYTE ComIf_ShadowBuffer_STM32_AVR_IO_Status[2];
UBYTE ComIf_ShadowBuffer_STM32_AVR_Message[51];
UBYTE ComIf_ShadowBuffer_STM32_AVR_Call[1];

ComIfTxMessageConfig ComIfTxMsgCfg_STM32[4] = {
	{
		/* ADC_Values_AVR */
		/* ID */        0xAD,
		/* Length */    6,
	#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* DynLength */ 0,
	#endif
		/* CHKLEN */    1,
		/* Tx Flags */  {
							0, // EnableAggregation
							0, // Force Transmit
							0, // PendingForTransmission
							0, // AggregatedInBuffer
							0  // PendingForConfirmation
						},
		/* Buffer */    ComIf_ShadowBuffer_STM32_ADC_Values_AVR,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* AVR_IO_Status */
		/* ID */        0x5D,
		/* Length */    2,
	#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* DynLength */ 0,
	#endif
		/* CHKLEN */    1,
		/* Tx Flags */  {
							0, // EnableAggregation
							0, // Force Transmit
							0, // PendingForTransmission
							0, // AggregatedInBuffer
							0  // PendingForConfirmation
						},
		/* Buffer */    ComIf_ShadowBuffer_STM32_AVR_IO_Status,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* AVR_Message */
		/* ID */        0x51,
		/* Length */    51,
	#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* DynLength */ 0,
	#endif
		/* CHKLEN */    1,
		/* Tx Flags */  {
							0, // EnableAggregation
							0, // Force Transmit
							0, // PendingForTransmission
							0, // AggregatedInBuffer
							0  // PendingForConfirmation
						},
		/* Buffer */    ComIf_ShadowBuffer_STM32_AVR_Message,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* AVR_Call */
		/* ID */        0xCA,
		/* Length */    1,
	#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* DynLength */ 0,
	#endif
		/* CHKLEN */    1,
		/* Tx Flags */  {
							0, // EnableAggregation
							0, // Force Transmit
							0, // PendingForTransmission
							0, // AggregatedInBuffer
							0  // PendingForConfirmation
						},
		/* Buffer */    ComIf_ShadowBuffer_STM32_AVR_Call,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
};

extern void IO_cmdData_AVRRxcbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_STM32_IO_cmdData_AVR[3];
extern void DTMF_Command_DataRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_STM32_DTMF_Command_Data[3];
extern void Stored_MobNumsRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_STM32_Stored_MobNums[79];

ComIfRxMessageConfig ComIfRxMsgCfg_STM32[3] = {
	{
		/* IO_cmdData_AVR */
		/* ID */      0xCD,
		/* Length */  2,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_STM32_IO_cmdData_AVR,
		/* RxCbk */   &IO_cmdData_AVRRxcbk
	},
	{
		/* DTMF_Command_Data */
		/* ID */      0xDC,
		/* Length */  2,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_STM32_DTMF_Command_Data,
		/* RxCbk */   &DTMF_Command_DataRxCbk
	},
	{
		/* Stored_MobNums */
		/* ID */      0xC0,
		/* Length */  78,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_STM32_Stored_MobNums,
		/* RxCbk */   &Stored_MobNumsRxCbk
	},
};


ComIf_ChannelConfigType ComIf_ChannelConfig[C_ComIfChannel_TOTAL] = 
{
    {	                                    /* For STM32 ComIf Channel */
    	/* Transmit Function */             &STM32_Transmit,
    	/* Error Notification Function */   &STM32_ComIf_ErrorNotification,
#ifdef COMIF_RX_REQUEST_ENABLED
		/* Rx Request Function */           NULL,
		/* Rx Timeout Time */               0,
		/* Rx Timeout Time Counter */       0,
#endif
    	/* Transmit Buffer */               ComIfChannelTxBuffer_STM32,
    	/* Transmit Buffer Length */        100,
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* Dynamic Max Data Length */       100,
#endif
    	/* Data Transfer Method */          COMIF_DATA_TXFR_METHOD_BYTES,
		/* Tx Message Config Pointer */     ComIfTxMsgCfg_STM32,
		/* Total Number of Tx Messages */   4,
		/* Rx Message Config Pointer */     ComIfRxMsgCfg_STM32,
		/* Total Number of Rx Messages */   3,
		/* Index of Receiving Message */    P_COMIF_INVALID_HANDLE,
		/* Length of Receiving Message */   0,
		/* Channel Specific Flags */
		{
			0,  // IsReceiving
			0,  // Delimit
			0,  // DLCVerified
			0,  // IsRxRequestEnabled
			0,  // IsTransmitting
			0,  // IsAggregationEnabled
			0,  // HasAggregatedMsgs
		},

#ifdef COMIF_TX_AGGREGATION_ENABLED
	#ifdef TX_AGGREGATION_ENABLED_STM32
        /* Tx Aggregation Control */        &(ComIf_TxAggCtrl_STM32)
	#else
        /* Tx Aggregation Control */        NULL
	#endif
#endif
    },
};

/***************************************************/
/* Function Definitions                            */
/***************************************************/
/* Channel Specific Rx Indication Functions */

UBYTE ComIf_RxIndication_STM32(UBYTE DataByte)
{
	return ComIf_RxIndication(((UBYTE)C_ComIfChannel_STM32), DataByte);
}

