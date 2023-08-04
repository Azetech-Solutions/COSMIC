/*
 * Cosmic_App.h
 *
 * Created: 02-08-2023 20:42:05
 *  Author: Admin
 */ 


#ifndef COSMIC_APP_H_
#define COSMIC_APP_H_


extern void IOControls();
extern void Send_Message(UBYTE MobileNum_Index,char *str);
extern void Send_ADC_Datas();
extern void Call_Request(BOOL RequestType,UBYTE MN_Index);


#endif /* COSMIC_APP_H_ */