#include "stm32g0xx.h"                  // Device header
#include "GPIO_Driver.h"
#include "LCD_DRIVER.h"

// LCD control pins
#define LCD_CTRL_EN_PORT    GPIOA
#define LCD_CTRL_EN_PIN     GPIO_PIN_6
#define LCD_CTRL_RS_PORT    GPIOA
#define LCD_CTRL_RS_PIN     GPIO_PIN_5

// LCD data pins
#define LCD_DATA_PORT       GPIOA
#define LCD_DATA_PIN_4      GPIO_PIN_7
#define LCD_DATA_PIN_5      GPIO_PIN_8
#define LCD_DATA_PIN_6      GPIO_PIN_9
#define LCD_DATA_PIN_7      GPIO_PIN_10




void LCD_INIT();
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void delay();
void debugstring(uint8_t *str);

void LCD_INIT()
{
	// Initialize GPIO pins
	
	PORTA_CLOCK_ENABLE();
	RCC->APBENR2 =0;
	RCC->APBENR2 |= 1;
	SYSCFG->CFGR1 = 0;
	SYSCFG->CFGR1 |= (1<<4);//ENABLE PA10
	SYSCFG->CFGR1 |= (1<<3);//ENABLE PA9
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LCD_CTRL_EN_PIN | LCD_CTRL_RS_PIN | LCD_DATA_PIN_4 | LCD_DATA_PIN_5 | LCD_DATA_PIN_6 | LCD_DATA_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_Config(LCD_CTRL_EN_PORT, &GPIO_InitStruct);
	GPIO_Config(LCD_CTRL_RS_PORT, &GPIO_InitStruct);
	GPIO_Config(LCD_DATA_PORT, &GPIO_InitStruct);
	
	// Initialize LCD
	// Configure for 4-bit mode
	lcd_send_cmd(0x30);
	delay();
	lcd_send_cmd(0x30);
	delay();
	lcd_send_cmd(0x30);
	delay();
	lcd_send_cmd(0x02);
	delay();
	lcd_send_cmd(0x28);
	delay();
	lcd_send_cmd(0x0c);
	delay();
	lcd_send_cmd(0x06);
	delay();	
}



// Function to send command to LCD
void lcd_send_cmd(uint8_t cmd)
{
    GPIO_WritePin(LCD_CTRL_RS_PORT, LCD_CTRL_RS_PIN, GPIO_PIN_RESET);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_4, (cmd >> 4) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_5, (cmd >> 5) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_6, (cmd >> 6) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_7, (cmd >> 7) & 0x01);
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_SET);
    delay();
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_RESET);
  
		GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_4, (cmd >> 0) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_5, (cmd >> 1) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_6, (cmd >> 2) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_7, (cmd >> 3) & 0x01);
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_SET);
    delay();
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_RESET);
		GPIOA->ODR = 0;
}

// Function to send data to LCD
void lcd_send_data(uint8_t data)
{
    GPIO_WritePin(LCD_CTRL_RS_PORT, LCD_CTRL_RS_PIN, GPIO_PIN_SET);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_4, (data >> 4) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_5, (data >> 5) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_6, (data >> 6) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_7, (data >> 7) & 0x01);
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_SET);
    delay();
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_RESET);
	
		GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_4, (data >> 0) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_5, (data >> 1) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_6, (data >> 2) & 0x01);
    GPIO_WritePin(LCD_DATA_PORT, LCD_DATA_PIN_7, (data >> 3) & 0x01);
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_SET);
    delay();
    GPIO_WritePin(LCD_CTRL_EN_PORT, LCD_CTRL_EN_PIN, GPIO_PIN_RESET);
		GPIOA->ODR = 0;
}

void delay()
{
		short int i;
    for( i=0 ;i<= 0X2AFF ;i++);
}
//int main()
//{
//	uint8_t val[10] = "HELLO";
//	LCD_INIT();
//	lcd_send_cmd(0x80);
//	delay();
//	debugstring(val);
//	while(1)
//	{
//		lcd_send_cmd(0xC0);
//		delay();
//		lcd_send_data('D');
//	}
//	return 0;
//}
void debugstring(uint8_t *str)
{
	while(*str!= '\0')
	{
		lcd_send_data(*str);
		str++;
		delay();
	}
}