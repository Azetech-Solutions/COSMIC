
#include <avr/io.h>
#include<avr/interrupt.h>
#include "Includes.h"
#include SYSOS_H

#define F_CPU 8000000UL

#define TIM1PRESCALER_1024 ((1<<CS10)|(1<<CS12))

void TIM1_Init(void)
{
	/* Initialize TIMER1 to use the overflow ISR */
	TCCR1B =  TIM1PRESCALER_1024;
	TIMSK |= (1 << TOIE1);
	TCNT1 = 65458;
	sei();
	return;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 65458;
	FUN_SYS_Timer_x1();
}