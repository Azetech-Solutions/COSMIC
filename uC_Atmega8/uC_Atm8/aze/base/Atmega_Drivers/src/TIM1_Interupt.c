#include "Includes.h"
#include TIM1_INTERRUPT_H
#include UART_H

void TIM1_Init(void)
{
	/* Initialize TIMER1 to use the overflow ISR */
	TCCR1B =  PRESCALER_8;
	TIMSK |= (1 << TOIE1);
	TCNT1 = 15535;
	return;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 15535;
	PORTD  ^= (1<<7);
}