#include "stm32g0xx.h"                  // Device header
#include "Includes.h"
#include GPIO_DRIVER_H
#include LCD_DRIVER_H


void DebugStringRow1(char *data);
void DebugStringRow2(char *data);

//void delay();
//void display_cmd(unsigned char a);
//void display_data(unsigned char a);
//void display_string(unsigned char *dat);
//void lcd_init();
//void switch_to_4bit(unsigned char c);
//void lcd_main();


//void delay()
//{
//		short int i;
//    for( i=0 ;i <= 0X2AFF ;i++);
//}

//void display_data(unsigned char a)
//{
//  short int  temp;
//	temp = 0x0780;
//	temp &= (a<<3);
//	RSDATA;//data mode
//  GPIOA->ODR = temp|(1<<5); //command tranfer to LCD display
//  ENON; //Enable to send data
//  delay();
//  ENOFF; //disable display data showing
//  
//	temp = 0x0780;
//	temp &= (a<<7);
//	RSDATA;//data mode
//  GPIOA->ODR = temp|(1<<5); //command tranfer to LCD display
//  ENON; //Enable to send data
//  delay();
//  ENOFF; //disable display data showing
//}

//void display_cmd(unsigned char a)
//{
//  short int  temp;
//	temp = 0x0780;
//	temp &= (a<<3);
//	RSCMD;//data mode
//  GPIOA->ODR = temp; //command tranfer to LCD display
//  ENON; //Enable to send data
//  delay();
//  ENOFF; //disable display data showing

//	temp = 0x0780;
//	temp &= (a<<7);
//  RSCMD;//data mode
//  GPIOA->ODR = temp; //command tranfer to LCD display
//  ENON; //Enable to send data
//  delay();
//  ENOFF; //disable display data showing
//}


//void display_string(unsigned char *dat)
//{
//    while(*dat)
//    {
//     display_data(*dat++); 
//    }  
//}

//void switch_to_4bit(unsigned char c)
//{
//  RSCMD;//cmd mode
//  GPIOA->ODR = c; //command tranfer to LCD display
//  ENON; //Enable to send data
//  delay();
//  ENOFF; //disable display data showing
//}


//void lcd_init()
//{
//	GPIO_InitTypeDef PA = {0};
//	
//  /* GPIO Ports Clock Enable */
//	PORTA_CLOCK_ENABLE();
//	GPIOA->ODR = 0;
//	RCC->APBENR2 |= 1;
//	SYSCFG->CFGR1 = 0;
//	SYSCFG->CFGR1 |= (1<<4);//ENABLE PA10
//	SYSCFG->CFGR1 |= (1<<3);//ENABLE PA9
//	GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);
//	
//  /*Configure GPIO pin : PA5 */
//  PA.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
//  PA.Mode = GPIO_MODE_OUTPUT_PP;
//  PA.Pull = GPIO_NOPULL;
//  PA.Speed = GPIO_SPEED_FREQ_LOW;
//  
//	GPIO_Config(GPIOA, &PA);

//	/*Configure GPIO pin Output Level */
////	GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
////	GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
////	GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
////	GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
////	GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
////	GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
//	display_cmd(0x28);
//	display_cmd(0x01);
//	display_cmd(0x0f);
//	display_cmd(0x06);
//	display_cmd(0x01);
////  display_cmd(0x38);//ENABLE 2nd ROW access
////	display_cmd(0x01);
////	display_cmd(0x02);
////	display_cmd(0x0c);//LCD ON
//}

//void lcd_main()
//{
//	unsigned char val[10] = "AZETECH";
//  lcd_init();
//	display_cmd(0x80);//set Display to 1st Row
//	display_string(val);
//	while(1)
//	{
////		GPIO_TogglePin(GPIOA, GPIO_PIN_5);
////		delay();
//	}
//}   
//int main()
//{
//	lcd_main();
//}
void DebugStringRow1(char *data)
{
	
}
void DebugStringRow2(char *data)
{
	
}