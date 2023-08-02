/*
 * CS_IOT.c
 *
 * Created: 26-11-2022 13:22:27
 *  Author: Admin
 */ 

#include "Includes.h"
#include CS_IOT_H
#include PLATFORM_TYPES_H

UBYTE DTMF_Transmit(UWORD Length, void * Data)
{
	return 1;
}


void DTMFCallMessageCbk(UBYTE Length, UBYTE *Data)
{
	
}

void DTMFMessageRxCbk(UBYTE Length, UBYTE *Data)
{
	
}
