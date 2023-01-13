#ifndef LCD_H
#define LCD_H


#define rs_hi (PORTB|=(1<<PB3))
#define en_hi (PORTB|=(1<<PB4))

#define d4_hi (PORTD|=(1<<PD4))
#define d5_hi (PORTD|=(1<<PD5))
#define d6_hi (PORTD|=(1<<PD6))
#define d7_hi (PORTD|=(1<<PD7))

#define rs_lo (PORTB&=~(1<<PB3))
#define en_lo (PORTB&=~(1<<PB4))

#define d4_lo (PORTD&=~(1<<PD4))
#define d5_lo (PORTD&=~(1<<PD5))
#define d6_lo (PORTD&=~(1<<PD6))
#define d7_lo (PORTD&=~(1<<PD7))

extern void LCD_Init(void);
extern void lcd_cmd(unsigned char a);
extern void lcd_dat(unsigned char a);
extern void lcd_string(char *a);
extern void lcd_data(unsigned char a);
extern void lcd_cur_pos(unsigned char b, unsigned char c);
extern void lcd_clrscr(void);

extern void lcd_num_2dig(unsigned int aa);
extern void lcd_num_3dig(unsigned int aa);
extern void lcd_num_point(unsigned int aa);

extern void DebugStringRow1(char *data);
extern void DebugStringRow2(char *data);

#endif