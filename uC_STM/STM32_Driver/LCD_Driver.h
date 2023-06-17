#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H


#define RSCMD GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
#define RSDATA GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
#define ENON  GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
#define ENOFF GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

extern void DebugStringRow1(unsigned char *data);
extern void DebugStringRow2(unsigned char *data);

#endif
