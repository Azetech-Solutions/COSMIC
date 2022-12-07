/*
 * TIM1_Interrupt.h
 *
 * Created: 24-11-2022 16:21:49
 *  Author: Admin
 */ 


#ifndef TIM1_INTERRUPT_H_
#define TIM1_INTERRUPT_H_

#include <avr/io.h>
#include<avr/interrupt.h>

extern void TIM1_Init(void); 

#define F_CPU 4000000UL

#define TIM1PRESCALER_1024 ((1<<CS10)|(1<<CS12))

#define PRESCALER_8    ((0<<CS10)|(1<<CS11)|(0<<CS12))


#endif /* TIM1_INTERRUPT_H_ */