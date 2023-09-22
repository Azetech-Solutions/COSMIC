
#include "Includes.h"
#include COMIF_SIGNALS_H


UBYTE ComIf_GetBitMaskValue(UBYTE BitLength)
{
	UBYTE Mask = 0x01; // Default 1 Bit Mask

	if(BitLength > 8)
	{
		BitLength = 8;
	}

    while(--BitLength)
    {
        Mask <<= 1;
        Mask |= 0x01;
    }

	return Mask;
}

BOOL UpdateSignalToBuffer(UBYTE Index,void * Data)
{
    BOOL retval = TRUE;

    UBYTE Buffer[4] = {0};

    SignalDatas_ST *Signal = &SignalInfo[Index];

    switch(Signal->DataType)
    {
        case INT8 :
        {
            if((Signal->BitLength > 0) && (Signal->BitLength <= 8 ))
            {
                Buffer[0]= (((SBYTE)*((SBYTE*)Data)) & 0xFF);
                Buffer[1] = Buffer[2] = Buffer[3] = 0;
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case UINT8:
        {
            if((Signal->BitLength > 0) && (Signal->BitLength <= 8 ))
            {
                Buffer[0] = (((UBYTE)*((UBYTE*)Data)) & 0xFF);
                Buffer[1] = Buffer[2] = Buffer[3] = 0;
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case INT16:
        {
            if((Signal->BitLength > 8) && (Signal->BitLength <= 16 ))
            {
                Buffer[0] = (SBYTE)(((*((SWORD*)Data)) & 0xFF00) >> 8);
                Buffer[1] = (SBYTE)((*((SWORD*)Data)) & 0x00FF);
                Buffer[2] = Buffer[3] = 0;
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}            
						}
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case UINT16:
        {
            if((Signal->BitLength > 8) && (Signal->BitLength <= 16 ))
            {
                Buffer[0] = (UBYTE)(((*((UWORD*)Data)) & 0xFF00) >> 8);
                Buffer[1] = (UBYTE)((*((UWORD*)Data)) & 0x00FF);
                Buffer[2] = Buffer[3] = 0;
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}            
						}
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case INT32:
        {
            if((Signal->BitLength > 8) && (Signal->BitLength <= 32 ))
            {
                Buffer[3] = (UBYTE)((*((SLONG*)Data)) & 0x00FF);
                Buffer[2] = (UBYTE)(((*((SLONG*)Data)) & 0xFF00) >> 8);
                Buffer[1] = (UBYTE)(((*((SLONG*)Data)) & 0xFF0000) >> 16);
                Buffer[0] = (UBYTE)(((*((SLONG*)Data)) & 0xFF000000) >> 24);
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}            
						}
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case UINT32:
        {
            if((Signal->BitLength > 16) && (Signal->BitLength <= 24 ))
            {

                Buffer[2] = (UBYTE)((*((ULONG*)Data)) & 0x00FF);
                Buffer[1] = (UBYTE)(((*((ULONG*)Data)) & 0xFF00) >> 8);
                Buffer[0] = (UBYTE)(((*((ULONG*)Data)) & 0xFF0000) >> 16);
            }

            if((Signal->BitLength > 24) && (Signal->BitLength <= 32 ))
            {
                Buffer[3] = (UBYTE)((*((ULONG*)Data)) & 0x00FF);
                Buffer[2] = (UBYTE)(((*((ULONG*)Data)) & 0xFF00) >> 8);
                Buffer[1] = (UBYTE)(((*((ULONG*)Data)) & 0xFF0000) >> 16);
                Buffer[0] = (UBYTE)(((*((ULONG*)Data)) & 0xFF000000) >> 24);
            }
							if(Signal->Endiannes == LITTLE)
							{
								uint32_t b0,b1,b2,b3;

								b0 = Buffer[3];
								b1 = Buffer[2];
								b2 = Buffer[1];
								b3 = Buffer[0];
								
								Buffer[0] = b0;
								Buffer[1] = b1;
								Buffer[2] = b2;
								Buffer[3] = b3; 
							}						
        }
        break;
        default:
        {
            retval = FALSE;
            break;
        }
    }

    UBYTE MSByte = (UBYTE)(Signal->D_StartBitPosition / 8);

    UBYTE SourceMSBitPosition;

    UBYTE SourceLSBitPosition;

    if(Signal->BitLength > 8)
    {
        if(Signal->BitLength %8 )
        {
            SourceMSBitPosition = (7 - (8 - (Signal->BitLength % 8)));
            SourceLSBitPosition = ((Signal->BitLength - (SourceMSBitPosition+1))%8);
        }
        else
        {
            SourceMSBitPosition = 7;
            SourceLSBitPosition = 0;
        }
    }
    else
    {
        SourceMSBitPosition = (7 - (8 - Signal->BitLength));
        SourceLSBitPosition = ((SourceMSBitPosition+1)- Signal->BitLength);
    }

    UBYTE MSBitPositionInByte = (UBYTE)(Signal->D_StartBitPosition % 8);
    UBYTE RemainingBitsInMSByte = 8 - (MSBitPositionInByte + 1);
    UBYTE LSByte = MSByte;
    UBYTE LSBitPositionInByte = 1;
    UBYTE PresenceInBytes = 0;

    if(Signal->BitLength > (MSBitPositionInByte + 1))
    {
        // If the signal is spread across multiple byte boundaries, find the LSByte and update
        UBYTE RemainingSignalLength = Signal->BitLength - (MSBitPositionInByte + 1);

        LSByte += ((UBYTE)(RemainingSignalLength / 8) + 1);

        if(RemainingSignalLength % 8)
        {
             LSBitPositionInByte = 8 - (RemainingSignalLength % 8);
        }
        else
        {
            LSBitPositionInByte = 0;
        }
    }
    else
    {
        // If the signal is within the same boundary, the update the Bit position
        LSBitPositionInByte = (MSBitPositionInByte + 1) - Signal->BitLength;
    }

    if((MSBitPositionInByte > 3) && (Signal->BitLength < 8))
    {
       PresenceInBytes = 1;
    }
    else
    {
       PresenceInBytes = ((LSByte - MSByte) + 1);
    }

    if(PresenceInBytes == 1)
    {
        UBYTE Mask =  ComIf_GetBitMaskValue(Signal->BitLength);
        UBYTE LSBMask = ComIf_GetBitMaskValue(LSBitPositionInByte);

        Signal->Buffer[MSByte] &= ~(Mask << LSBitPositionInByte);

        Signal->Buffer[MSByte] |= ((Buffer[0]  << LSBitPositionInByte) & (Mask << LSBitPositionInByte));
    }
    else
    {
        //If Multi Byte Boundary signals
        UBYTE MSBMask = ComIf_GetBitMaskValue(MSBitPositionInByte + 1);
        UBYTE LSBMask = ComIf_GetBitMaskValue(LSBitPositionInByte);

        UBYTE SMSBMask = ComIf_GetBitMaskValue(SourceMSBitPosition - MSBitPositionInByte);

        for( int i= MSByte, j = 0; i <= LSByte; i++, j++)
        {
            if(MSBitPositionInByte > SourceMSBitPosition)
            {
                if(i==MSByte)
                {
                    Signal->Buffer[i] &= ~MSBMask;

                    Signal->Buffer[i] |= (UBYTE)((Buffer[j] << LSBitPositionInByte) & MSBMask);
                }
                else
                {
                    UBYTE MSK = ~ComIf_GetBitMaskValue(LSBitPositionInByte);

                    Signal->Buffer[i] &= ~(ComIf_GetBitMaskValue(8-LSBitPositionInByte)<<LSBitPositionInByte);

                    Signal->Buffer[i] |= (UBYTE)((Buffer[j] << LSBitPositionInByte) & MSK );

                }

                if(i != LSByte)
                {
                    Signal->Buffer[i] &=  ~LSBMask;//1001 1001 & 0000 0000 - 0000 0000

                    Signal->Buffer[i] |=  (UBYTE)(((Buffer[j+1]   >> (8 - LSBitPositionInByte))) & LSBMask);//altered
                }
            }
            else
            {
                if(i == MSByte)
                {
                    Signal->Buffer[i] &= ~MSBMask;

                    Signal->Buffer[i] |= (UBYTE)((Buffer[j]  >> (SourceMSBitPosition - MSBitPositionInByte)) & MSBMask);                }
                else
                {
                    if(i != LSByte)
                    {
                        Signal->Buffer[i] &= ~LSBMask;

                        Signal->Buffer[i] |= (UBYTE)((Buffer[j]  >> (SourceMSBitPosition - MSBitPositionInByte)) & LSBMask);
                    }
                }

                if(SourceMSBitPosition != MSBitPositionInByte)
                {
                    if(i != LSByte)
                    {
                        Signal->Buffer[i+1] &= ~(SMSBMask << (8-(SourceMSBitPosition - MSBitPositionInByte)));
                        Signal->Buffer[i+1] |= (UBYTE)(((Buffer[j] << LSBitPositionInByte) & ~LSBMask));//altered
                    }
                }

            }
        }
    }

    return retval;
}

BOOL GetSignalFromBuffer(UBYTE Index,void * data)
{
    BOOL retval = TRUE;

    UBYTE Buffer[4] = {0};

    SignalDatas_ST *Signal = &SignalInfo[Index];

    UBYTE MSByte = (UBYTE)(Signal->D_StartBitPosition / 8);

    UBYTE SourceMSBitPosition;

    UBYTE SourceLSBitPosition;

    if(Signal->BitLength > 8)
    {
        if(Signal->BitLength %8 )
        {
            SourceMSBitPosition = (7 - (8 - (Signal->BitLength % 8)));
            SourceLSBitPosition = ((Signal->BitLength - (SourceMSBitPosition+1))%8);
        }
        else
        {
            SourceMSBitPosition = 7;
            SourceLSBitPosition = 0;
        }
    }
    else
    {
        SourceMSBitPosition = (7 - (8 - Signal->BitLength));
        SourceLSBitPosition = ((SourceMSBitPosition+1)- Signal->BitLength);
    }

    UBYTE MSBitPositionInByte = (UBYTE)(Signal->D_StartBitPosition % 8);
    UBYTE RemainingBitsInMSByte = 8 - (MSBitPositionInByte + 1);
    UBYTE LSByte = MSByte;
    UBYTE LSBitPositionInByte = 1;
    UBYTE PresenceInBytes = 0;

    if(Signal->BitLength > (MSBitPositionInByte + 1))
    {
        // If the signal is spread across multiple byte boundaries, find the LSByte and update
        UBYTE RemainingSignalLength = Signal->BitLength - (MSBitPositionInByte + 1);

        LSByte += ((UBYTE)(RemainingSignalLength / 8) + 1);

        if(RemainingSignalLength % 8)
        {
             LSBitPositionInByte = 8 - (RemainingSignalLength % 8);
        }
        else
        {
            LSBitPositionInByte = 0;
        }
    }
    else
    {
        // If the signal is within the same boundary, the update the Bit position
        LSBitPositionInByte = (MSBitPositionInByte + 1) - Signal->BitLength;
    }

    if((MSBitPositionInByte > 3) && (Signal->BitLength < 8))
    {
       PresenceInBytes = 1;
    }
    else
    {
       PresenceInBytes = ((LSByte - MSByte) + 1);
    }

    if(PresenceInBytes == 1)
    {
        UBYTE Mask =  ComIf_GetBitMaskValue(Signal->BitLength);
        UBYTE LSBMask = ComIf_GetBitMaskValue(LSBitPositionInByte);

        Buffer[0] &= ~(Mask);

        Buffer[0] |= ((Signal->Buffer[MSByte]  >> LSBitPositionInByte) & Mask);
    }
    else
    {
        //If Multi Byte Boundary signals
        UBYTE MSBMask = ComIf_GetBitMaskValue(MSBitPositionInByte + 1);
        UBYTE LSBMask = ComIf_GetBitMaskValue(LSBitPositionInByte);
        UBYTE SB_MSBDIFF  = SourceMSBitPosition - MSBitPositionInByte;
        UBYTE SMSBMask = ComIf_GetBitMaskValue(SB_MSBDIFF);

        for( int i= 0, j = MSByte; i <= LSByte; i++, j++)
        {
            if(MSBitPositionInByte > SourceMSBitPosition)
            {
                if(i==0)
                {
                    Buffer[i] &= ~(MSBMask >> LSBitPositionInByte);

                    Buffer[i] |= (UBYTE)((Signal->Buffer[j] >> LSBitPositionInByte) & (MSBMask >> LSBitPositionInByte));
                }
                else
                {
                    Buffer[i] &= ((~LSBMask) >> LSBitPositionInByte);

                    Buffer[i] |= (UBYTE)((Signal->Buffer[j] >> LSBitPositionInByte) & ((~LSBMask) >> LSBitPositionInByte));
                }

                if(i != LSByte)
                {
                    Buffer[i+1] &= ~(MSBMask << (8 - LSBitPositionInByte));//1001 1001 & 0000 0000 - 0000 0000
                    Buffer[i+1] |=  ((UBYTE)(Signal->Buffer[j]   << (8 - LSBitPositionInByte)) & (MSBMask << (8 - LSBitPositionInByte)));
                }
            }
            else
            {
                if(i == 0)
                {
                    Buffer[i] &= SMSBMask;//((~SMSBMask) << SB_MSBDIFF);

                    Buffer[i] |= (UBYTE)((Signal->Buffer[j]  << SB_MSBDIFF) & (MSBMask << SB_MSBDIFF));
                }
                else
                {
                    if(i != LSByte)
                    {
                        Buffer[i] &= SMSBMask;//((~SMSBMask) << SB_MSBDIFF);
                        Buffer[i] |= (UBYTE)((Signal->Buffer[j]  << SB_MSBDIFF) & (LSBMask << SB_MSBDIFF));
                    }
                }

                if(SourceMSBitPosition != MSBitPositionInByte)
                {
                    if(i != LSByte)
                    {
                        Buffer[i] &= (LSBMask << SB_MSBDIFF);
                        Buffer[i] |= (UBYTE)(((Signal->Buffer[j+1] >> LSBitPositionInByte)) & SMSBMask);
                    }
                }

            }
        }
    }

    switch(Signal->DataType)
    {
        case INT8 :
        {
            if(Signal->BitLength <= 8 )
            {
              *((SBYTE*)(data)) = (SBYTE) (Buffer[0] & 0xFF);
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case UINT8:
        {
            if(Signal->BitLength <= 8 )
            {
              *((SBYTE*)(data)) = (SBYTE) (Buffer[0] & 0xFF);
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case INT16:
        {
            if((Signal->BitLength > 8) && (Signal->BitLength <= 16 ))
            {
                *((SWORD*)(data))   = (SWORD) (Buffer[0] & 0xFF);
                *((SWORD*)(data))  |= (SWORD) ((Buffer[1] & 0xFF) << 8);
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case UINT16:
        {
            if((Signal->BitLength > 8) && (Signal->BitLength <= 16 ))
            {
                *((UWORD*)(data))  = (UWORD) (Buffer[1] & 0xFF);
                *((UWORD*)(data)) |= (UWORD) ((Buffer[0] & 0xFF) << 8);
            }
            else
            {
                retval = FALSE;//Invalid Settings or value
            }
        }
        break;
        case INT32:
        {
            if((Signal->BitLength > 16) && (Signal->BitLength <= 24 ))
            {
                *((SLONG*)(data))  = (SLONG) (Buffer[2] & 0xFF);
                *((SLONG*)(data)) |= (SLONG) ((Buffer[1] & 0xFF) << 8);
                *((SLONG*)(data)) |= (SLONG) ((Buffer[0] & 0xFF) << 16);
            }

            if((Signal->BitLength > 24) && (Signal->BitLength <= 32 ))
            {
                *((SLONG*)(data))  = (SLONG) (Buffer[3] & 0xFF);
                *((SLONG*)(data)) |= (SLONG) ((Buffer[2] & 0xFF) << 8);
                *((SLONG*)(data)) |= (SLONG) ((Buffer[1] & 0xFF) << 16);
                *((SLONG*)(data)) |= (SLONG) ((Buffer[0] & 0xFF) << 24);
            }
        }
        break;
        case UINT32:
        {
            if((Signal->BitLength > 16) && (Signal->BitLength <= 24 ))
            {
                *((ULONG*)(data))  = (ULONG) (Buffer[2] & 0xFF);
                *((ULONG*)(data)) |= (ULONG) ((Buffer[1] & 0xFF) << 8);
                *((ULONG*)(data)) |= (ULONG) ((Buffer[0] & 0xFF) << 16);
            }

            if((Signal->BitLength > 24) && (Signal->BitLength <= 32 ))
            {
                *((ULONG*)(data))  = (ULONG) (Buffer[3] & 0xFF);
                *((ULONG*)(data)) |= (ULONG) ((Buffer[2] & 0xFF) << 8);
                *((ULONG*)(data)) |= (ULONG) ((Buffer[1] & 0xFF) << 16);
                *((ULONG*)(data)) |= (ULONG) ((Buffer[0] & 0xFF) << 24);
            }
        }
        break;
        default:
        {
            retval = FALSE;
            break;
        }
    }

    return retval;
}
