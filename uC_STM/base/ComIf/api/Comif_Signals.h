#ifndef __SIGNALS_H_
#define __SIGNALS_H_

#include "Includes.h"
#include COMIF_CONFIG_H

typedef enum
{
    BIG,
    LITTLE
}Endiannes_EN;

typedef enum
{
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32
}DataType_EN;


typedef struct
{
    UBYTE D_StartBitPosition;//Destination
    UBYTE BitLength;
    Endiannes_EN Endiannes;
    DataType_EN DataType;
    UBYTE *Buffer;
}SignalDatas_ST;

SignalDatas_ST SignalInfo[2]={
    {
        /**< D_StartBitPosition */0,
        /**< BitLength*/32,
        /**< Endiannes */BIG,
        /**< DataType */UINT32,
        /**< Buffer */0
    },
    {
        /**< D_StartBitPosition*/32,
        /**< BitLength*/24,
        /**< Endiannes */BIG,
        /**< DataType */UINT32,
        /**< Buffer */0
    }

};

extern BOOL UpdateSignalToBuffer(UBYTE Index,void * Data);
extern BOOL GetSignalFromBuffer(UBYTE Index,void * data);

#endif // __SIGNALS_H_
