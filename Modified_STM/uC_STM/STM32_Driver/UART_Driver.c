/***************************************************/
/* Header Inclusions                               */
/***************************************************/
#include "stm32g0xx.h"
#include "Includes.h"
#include PLATFORM_TYPES_H
#include SIMCOM_H
#include UART_DRIVER_H
#include GPIO_DRIVER_H

/***************************************************/
/* Function Declarations                           */
/***************************************************/

void GPIOBPORTUART1();
void USART_Init(void);
void AVR_SendData(unsigned char Data);
void USART2_Pin_Init(void);
void USART2_Init(void);
void SIM_Send_Data(unsigned char Data);
void USART_String(const char* data);

/***************************************************/
/* Function Definitions                            */
/***************************************************/
BOOL IsUartCommunicationOkay = FALSE;


void GPIOBPORTUART1()
{
	GPIO_InitTypeDef PB;
	
	PORTB_CLOCK_ENABLE();
	RCC->APBENR2 |= (1<<14);
	
  GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
	
  /*Configure GPIO pins : PA2 PA3 */
  PB.Pin = GPIO_PIN_6|GPIO_PIN_7;
  PB.Mode = GPIO_MODE_AF_PP;
  PB.Pull = GPIO_NOPULL;
  PB.Speed = GPIO_SPEED_FREQ_LOW;
  PB.Alternate = GPIO_AF1_USART1;
	GPIO_Config(GPIOB,&PB);	
	
}

void USART_Init(void)
{
	GPIOBPORTUART1();
		
	/* FIFO mode is Enabled*/
	USART1->CR1 |= USART_CR1_FIFOEN ;
	USART1->CR3 |= (	USART_CR3_TXFTCFG_1 |	 //TXFIFO reaches 1/4 of its depth
										USART_CR3_RXFTCFG_1	);  //Receive FIFO reaches 1/4 of its depth
	
	 /*M1: Word length & M0: Word length*/
	USART1->CR1 &= (~USART_CR1_M0)|(~USART_CR1_M1);
	
		/*Oversampling 8*/
	USART1->CR1 |= USART_CR1_OVER8;
		/*Baud Rate Config For Baurate 115200*/
	USART1->BRR = 0x3412;
	/* Rx & Tx Enable*/
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE ) ; 
	
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	/* Stop Bits */
	USART1->CR2 &= ~USART_CR2_STOP;
	/*Enable the USART*/
	USART1->CR1 |= USART_CR1_UE;
	/*USART1 Interrupt Enable*/
	NVIC_EnableIRQ(USART1_IRQn);
	
	IsUartCommunicationOkay = TRUE;
}

void USART1_IRQHandler()
{
	if(USART1->ISR & (1<<5))
	{
		unsigned char data = 0;
		data = USART1->RDR;
		AvrStatus_Buffer_EnQueue(data);
	}
}

void AVR_SendData(unsigned char Data)
{
	while(!(USART1->ISR & (1<<7)));
	USART1->TDR = Data;
}

void USART2_Pin_Init(void)
{
	
	GPIO_InitTypeDef PA;
	PORTA_CLOCK_ENABLE();
	RCC_USART2_CLK_ENABLE();
	
	GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  /*Configure GPIO pins : PA2 PA3 */
  PA.Pin = GPIO_PIN_2|GPIO_PIN_3;
  PA.Mode = GPIO_MODE_AF_PP;
  PA.Pull = GPIO_NOPULL;
  PA.Speed = GPIO_SPEED_FREQ_LOW;
  PA.Alternate = GPIO_AF1_USART2;
	GPIO_Config(GPIOA,&PA);	
}


void USART2_Init(void)
{
	USART2_Pin_Init();
		
	/* FIFO mode is Enabled*/
	USART2->CR1 |= USART_CR1_FIFOEN ;
	USART2->CR3 |= (	USART_CR3_TXFTCFG_1 |	 //TXFIFO reaches 1/4 of its depth
										USART_CR3_RXFTCFG_1	);  //Receive FIFO reaches 1/4 of its depth
	
	 /*M1: Word length & M0: Word length*/
	USART2->CR1 &= (~USART_CR1_M0)|(~USART_CR1_M1);
	
		/*Oversampling 8*/
	USART2->CR1 |= USART_CR1_OVER8;
		/*Baud Rate Config For Baurate 115200*/
	USART2->BRR = 0x453;
	/* Rx & Tx Enable*/
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE ) ; 
	
	USART2->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	/* Stop Bits */
	USART2->CR2 &= ~USART_CR2_STOP;
	/*Enable the USART*/
	USART2->CR1 |= USART_CR1_UE;
	/*USART2 Interrupt Enable*/
	NVIC_EnableIRQ(USART2_IRQn);

}


void SIM_Send_Data(unsigned char Data)
{
	while(!(USART2->ISR & (1<<7)))
	{
		GPIOA->ODR &= ~(1<<1);
	}
	USART2->TDR = Data;
}

unsigned char UART2_TransmitByte(unsigned char Data)
{
	unsigned char retval = 0;
	
	while(!(USART1->ISR & (1<<7)));
	
	retval = 1;
	
	USART1->TDR = Data;
	
	return retval;
}

void USART2_IRQHandler()
{
	if(USART2->ISR & USART_ISR_RXNE_RXFNE)
	{
		unsigned char data = USART2->RDR;
		SIMCOM_Data_Read(data);
	}
}

void USART_String(const char* data)
{
	while(*data)
	{
		SIM_Send_Data(*(data++));
	}
}

void USART1_String(const char* data)
{
	while(*data)
	{
		AVR_SendData(*(data++));
	}
}



