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

UBYTE ComIf_ShadowBuffer_Cloud_TxCommandData[8];
UBYTE ComIf_ShadowBuffer_Cloud_StatusData[2];

ComIfTxMessageConfig ComIfTxMsgCfg_Cloud[2] = {
	{
		/* TxCommandData */
		/* ID */        0xCD,
		/* Length */    8,
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
		/* Buffer */    ComIf_ShadowBuffer_Cloud_TxCommandData,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
	{
		/* StatusData */
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
		/* Buffer */    ComIf_ShadowBuffer_Cloud_StatusData,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
};

extern void CloudRxCommandDataRxCbk(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_Cloud_RxCommandData[9];

ComIfRxMessageConfig ComIfRxMsgCfg_Cloud[1] = {
	{
		/* RxCommandData */
		/* ID */      0xCD,
		/* Length */  8,
		/* CHKLEN */  1,
		/* RxFlags */ {
						0, // ReceptionStarted
						0, // NewMessageReceived
						0, // ErrorInReception
						0, // RxRequest
						0  // RxRequestPlaced
					  },
		/* RxngIdx */ 0,
		/* Buffer */  ComIf_RxMessageBuffer_Cloud_RxCommandData,
		/* RxCbk */   &CloudRxCommandDataRxCbk
	},
};

/* -------------------------------------------------- */
/* For Avr Channel */
/* -------------------------------------------------- */

extern UBYTE AvrTransmitFunc(UWORD Length, void * Data);

UBYTE ComIfChannelTxBuffer_Avr[64];

UBYTE ComIf_ShadowBuffer_Avr_AvrTxFunc[2];

ComIfTxMessageConfig ComIfTxMsgCfg_Avr[1] = {
	{
		/* AvrTxFunc */
		/* ID */        0x48,
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
		/* Buffer */    ComIf_ShadowBuffer_Avr_AvrTxFunc,
		/* TxConfCbk */ NULL,
		/* TxCbk */     NULL
	},
};

extern void AvrStatusRxFunc(UBYTE Length, UBYTE *Data);
UBYTE ComIf_RxMessageBuffer_Avr_AvrStatusData[3];

ComIfRxMessageConfig ComIfRxMsgCfg_Avr[1] = {
	{
		/* AvrStatusData */
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
		/* Buffer */  ComIf_RxMessageBuffer_Avr_AvrStatusData,
		/* RxCbk */   &AvrStatusRxFunc
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
		/* Total Number of Tx Messages */   2,
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
    {	                                    /* For Avr ComIf Channel */
    	/* Transmit Function */             &AvrTransmitFunc,
		/* Error Notification Function */   NULL,
#ifdef COMIF_RX_REQUEST_ENABLED
		/* Rx Request Function */           NULL,
		/* Rx Timeout Time */               0,
		/* Rx Timeout Time Counter */       0,
#endif
    	/* Transmit Buffer */               ComIfChannelTxBuffer_Avr,
    	/* Transmit Buffer Length */        64,
#ifdef COMIF_DYNAMIC_DATA_LENGTH_ENABLED
		/* Dynamic Max Data Length */       64,
#endif
    	/* Data Transfer Method */          COMIF_DATA_TXFR_METHOD_BYTES,
		/* Tx Message Config Pointer */     ComIfTxMsgCfg_Avr,
		/* Total Number of Tx Messages */   1,
		/* Rx Message Config Pointer */     ComIfRxMsgCfg_Avr,
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
	#ifdef TX_AGGREGATION_ENABLED_AVR
        /* Tx Aggregation Control */        &(ComIf_TxAggCtrl_Avr)
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

UBYTE ComIf_RxIndication_Avr(UBYTE DataByte)
{
	return ComIf_RxIndication(((UBYTE)C_ComIfChannel_Avr), DataByte);
}

