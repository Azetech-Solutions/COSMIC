/*
 * adc.c
 *
 * Created: 28-08-2018 20:02:52
 *  Author: MDRP
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "Includes.h"
#include ADC_H

void adc_init()
{
	ADMUX =(1<<REFS0)|(1<<REFS1);	/*internal reference*/
	ADCSRA=(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);//prescaler
	//ADCSRA=(1<<ADFR);	/*free running mode*/
	ADCSRA=(1<<ADEN);	/*adc enable*/
}

unsigned int adc_read(unsigned char ch)
{
	ch &= 0b00000111;
	ADMUX=(ADMUX & 0XF0)|ch;
	_delay_us(100);
	ADCSRA|=(1<<ADSC);	//start adc conversion
	while(!(ADCSRA & (1<<ADIF)));
	//ADCSRA|=(1<<ADIF);
	//while(ADCSRA & (1<<ADSC));
	
	int Ain,AinLow;
	AinLow = (int)ADCL;
	Ain = (int)ADCH*256;
	Ain = Ain + AinLow;
	ADC=Ain;
	return(Ain);
	
	//int temp=ADCL;
	//return((ADCH<<8)+temp);
	/*
	ADMUX=ADMUX|(ch & 0xF0);	
	_delay_us(50);
	ADCSRA |= (1<<ADSC);		
	while((ADCSRA&(1<<ADIF))==0);	
	_delay_us(10);
	
	int Ain,AinLow;
	AinLow = (int)ADCL;				
	Ain = (int)ADCH*256;		
	Ain = Ain + AinLow;
	ADC=Ain;
	return(Ain);*/
}