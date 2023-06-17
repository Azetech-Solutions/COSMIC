#ifndef LCD_H
#define LCD_H

#define rs_hi (PORTD|=(1<<PD3))
#define en_hi (PORTD|=(1<<PD4))

#define d4_hi (PORTB|=(1<<PB6))
#define d5_hi (PORTB|=(1<<PB7))
#define d6_hi (PORTD|=(1<<PD5))
#define d7_hi (PORTD|=(1<<PD6))

#define rs_lo (PORTD&=~(1<<PD3))
#define en_lo (PORTD&=~(1<<PD4))

#define d4_lo (PORTB&=~(1<<PB6))
#define d5_lo (PORTB&=~(1<<PB7))
#define d6_lo (PORTD&=~(1<<PD5))
#define d7_lo (PORTD&=~(1<<PD6))


extern void lcd_init(void);
extern void lcd_cmd(unsigned char a);
extern void lcd_dat(unsigned char a);
extern void lcd_string(unsigned char *a);
extern void lcd_data(unsigned char a);
extern void lcd_cur_pos(unsigned char b, unsigned char c);
//extern void lcd_num_disp(unsigned int aa);
extern void lcd_clrscr(void);
extern void lcd_num_2dig(unsigned int aa);
extern void lcd_num_3dig(unsigned int aa);
void lcd_num_point(unsigned int aa);
char * ftoa(float f, int * status);
#endif