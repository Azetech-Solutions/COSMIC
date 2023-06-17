
#ifndef USART2_H
#define USART2_H
#include "stm32g0xx.h"                 

#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */

/***********************************************************/
/*                 Macro Definitions                       */
/***********************************************************/

/*	Macros to enable Clock	*/
#define RCC_USART2_CLK_ENABLE()	(RCC->APBENR1 |= (1<<17))



/**************************************************/
/***********			UART				  *****/
/**************************************************/

/*	Bit definition for USART SR register		*/

#define USART_REG_SR_TXE_FLAG	((uint32_t)(1<<7))
#define USART_REG_SR_TC_FLAG	((uint32_t)(1<<6))
#define USART_REG_SR_RXNE_FLAG	((uint32_t)(1<<5))
#define USART_REG_SR_IDLE_FLAG	((uint32_t)(1<<4))
#define USART_REG_SR_ORE_FLAG	((uint32_t)(1<<3))
#define USART_REG_SR_NE_FLAG	((uint32_t)(1<<2))
#define USART_REG_SR_FE_FLAG	((uint32_t)(1<<1))
#define USART_REG_SR_PE_FLAG	((uint32_t)(1<<0))

/*	Bit definition for USART BRR register		*/
#define USART_REG_BRR_MANTISSA ((uint32_t)(0xC3<<4))
#define USART_REG_BRR_FRACTION ((uint32_t)(5<<0))
#define USART_REG_BRR_APB1_BAUDRATE_9600 ((uint32_t)(0xC35))
#define USART_REG_BRR_APB1_BAUDRATE_57600 ((uint32_t)(0x208))
#define USART_REG_BRR_APB1_BAUDRATE_115200 ((uint32_t)(0x104))
#define USART_REG_BRR_APB2_BAUDRATE_9600 ((uint32_t)(0x186A))
#define USART_REG_BRR_APB2_BAUDRATE_57600 ((uint32_t)(0x411))
#define USART_REG_BRR_APB2_BAUDRATE_115200 ((uint32_t)(0x208))

/*	Bit definition for USART CR1 register		*/
#define USART_REG_CR1_OVER8 ((uint32_t)(1<<4))
#define USART_REG_CR1_USART_EN	((uint32_t)(1<<13))
#define USART_REG_CR1_WL_1S8B	((uint32_t)(1<<12))
#define USART_REG_CR1_TX_ENABLE	((uint32_t)(1<<3))
#define USART_REG_CR1_RX_ENABLE	((uint32_t)(1<<2))
#define USART_REG_CR1_RXNE_ENABLE ((uint32_t)(1<<5))

/*	Bit definition for USART CR2 register		*/
#define USART_STOPBITS_1	((uint32_t)0x00)
#define USART_STOPBITS_HALF	((uint32_t)0x01)
#define USART_STOPBITS_2	((uint32_t)0x02)
#define USART_STOPBITS_1_HALF	((uint32_t)0x03)
#define USART_REG_CR2_STOP ((USART_STOPBITS_1)<<12)


extern void USART_Init(void);
static void USART_Pin_Init(void);
extern unsigned char USART_Transmit_Byte(unsigned char Data);
extern void USART_String(const char* data);
extern void UART_main();
#endif /*USART_H*/