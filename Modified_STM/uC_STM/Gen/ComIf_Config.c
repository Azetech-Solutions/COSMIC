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
/* For Cloud Channel */
/* -------------------------------------------------- */

extern UBYTE Cloud_Transmit(UWORD Length, void * Data);
extern void Cloud_ComIf_ErrorNotification(ULONG Debug0, ULONG Debug1);

UBYTE ComIfChannelTxBuffer_Cloud[64];

UBYTE ComIf_ShadowBuffer_Cloud_IO_Status_Cloud[2];
UBYTE ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud[6];
UBYTE ComIf_ShadowBuffer_Cloud_Cloud_Init[1];

ComIfTxMessageConfig ComIfTxMsgCfg_Cloud[3] = {
	{
		/* IO_Status_Cloud */
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
		/* Buffer */    ComIf_ShadowBuffer_Cloud_IO_Status_Cloud,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* ADC_Values_Cloud */
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
		/* Buffer */    ComIf_ShadowBuffer_Cloud_ADC_Values_Cloud,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* Cloud_Init */
		/* ID */        0xCD,
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
		/* Buffer */    ComIf_ShadowBuffer_Cloud_Cloud_Init,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
};

extern void IO_cmdData_CloudRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_Cloud_IO_cmdData_Cloud[3];

ComIfRxMessageConfig ComIfRxMsgCfg_Cloud[1] = {
	{
		/* IO_cmdData_Cloud */
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
		/* Buffer */  ComIf_RxMessageBuffer_Cloud_IO_cmdData_Cloud,
		/* RxCbk */   &IO_cmdData_CloudRxCbk
	},
};

/* -------------------------------------------------- */
/* For AVR Channel */
/* -------------------------------------------------- */

extern UBYTE AVR_Transmit(UWORD Length, void * Data);

UBYTE ComIfChannelTxBuffer_AVR[100];

UBYTE ComIf_ShadowBuffer_AVR_IO_cmdData_AVR[2];
UBYTE ComIf_ShadowBuffer_AVR_DTMFCommandData[2];
UBYTE ComIf_ShadowBuffer_AVR_Stored_MobNums[78];

ComIfTxMessageConfig ComIfTxMsgCfg_AVR[3] = {
	{
		/* IO_cmdData_AVR */
		/* ID */        0xCD,
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
		/* Buffer */    ComIf_ShadowBuffer_AVR_IO_cmdData_AVR,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* DTMFCommandData */
		/* ID */        0xDC,
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
		/* Buffer */    ComIf_ShadowBuffer_AVR_DTMFCommandData,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* Stored_MobNums */
		/* ID */        0xC0,
		/* Length */    78,
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
		/* Buffer */    ComIf_ShadowBuffer_AVR_Stored_MobNums,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
};

extern void AVR_IO_StatusRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_AVR_AVR_IO_Status[3];
extern void ADC_RxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_AVR_ADC_Values_AVR[7];
extern void TextMessageRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_AVR_AVR_TextMessage[52];
extern void AVR_CallRequestRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_AVR_AVR_Call[2];

ComIfRxMessageConfig ComIfRxMsgCfg_AVR[4] = {
	{
		/* AVR_IO_Status */
		/* ID */      0x5D,
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
		/* Buffer */  ComIf_RxMessageBuffer_AVR_AVR_IO_Status,
		/* RxCbk */   &AVR_IO_StatusRxCbk
	},
	{
		/* ADC_Values_AVR */
		/* ID */      0xAD,
		/* Length */  6,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_AVR_ADC_Values_AVR,
		/* RxCbk */   &ADC_RxCbk
	},
	{
		/* AVR_TextMessage */
		/* ID */      0x51,
		/* Length */  51,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_AVR_AVR_TextMessage,
		/* RxCbk */   &TextMessageRxCbk
	},
	{
		/* AVR_Call */
		/* ID */      0xCA,
		/* Length */  1,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_AVR_AVR_Call,
		/* RxCbk */   &AVR_CallRequestRxCbk
	},
};


ComIf_ChannelConfigType ComIf_ChannelConfig[C_ComIfChannel_TOTAL] = 
{
    {	                                    /* For Cloud ComIf Channel */
    	/* Transmit Function */             &Cloud_Transmit,
    	/* Error Notification Function */   &Cloud_ComIf_ErrorNotification,
#ifdef COMIF_RX_REQUEST_ENABLED
		/* Rx Request Function */           NULL,
		/* Rx Timeout Time */               0,
		/* Rx Timeout Time Counter */       0,
#endif
    	/* Transmit Buffer */               ComIfChannelTxBuffer_Cloud,
    	/* Transmit Buffer Length */        64,
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* Dynamic Max Data Length */       64,
#endif
    	/* Data Transfer Method */          COMIF_DATA_TXFR_METHOD_STRING,
		/* Tx Message Config Pointer */     ComIfTxMsgCfg_Cloud,
		/* Total Number of Tx Messages */   3,
		/* Rx Message Config Pointer */     ComIfRxMsgCfg_Cloud,
		/* Total Number of Rx Messages */   1,
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
	#ifdef TX_AGGREGATION_ENABLED_CLOUD
        /* Tx Aggregation Control */        &(ComIf_TxAggCtrl_Cloud)
	#else
        /* Tx Aggregation Control */        NULL
	#endif
#endif
    },
    {	                                    /* For AVR ComIf Channel */
    	/* Transmit Function */             &AVR_Transmit,
		/* Error Notification Function */   NULL,
#ifdef COMIF_RX_REQUEST_ENABLED
		/* Rx Request Function */           NULL,
		/* Rx Timeout Time */               0,
		/* Rx Timeout Time Counter */       0,
#endif
    	/* Transmit Buffer */               ComIfChannelTxBuffer_AVR,
    	/* Transmit Buffer Length */        100,
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* Dynamic Max Data Length */       100,
#endif
    	/* Data Transfer Method */          COMIF_DATA_TXFR_METHOD_BYTES,
		/* Tx Message Config Pointer */     ComIfTxMsgCfg_AVR,
		/* Total Number of Tx Messages */   3,
		/* Rx Message Config Pointer */     ComIfRxMsgCfg_AVR,
		/* Total Number of Rx Messages */   4,
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
	#ifdef TX_AGGREGATION_ENABLED_AVR
        /* Tx Aggregation Control */        &(ComIf_TxAggCtrl_AVR)
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

UBYTE ComIf_RxIndication_Cloud(char * DataString, UWORD Length)
{
	return ComIf_RxIndication_ASCIIString(((UBYTE)C_ComIfChannel_Cloud), DataString, Length);
}

UBYTE ComIf_RxIndication_AVR(UBYTE DataByte)
{
	return ComIf_RxIndication(((UBYTE)C_ComIfChannel_AVR), DataByte);
}

