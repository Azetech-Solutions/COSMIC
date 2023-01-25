#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"




void LCD_Init()
{
	DDRD=0XFF;
	PORTD=0x00;
	lcd_cursor();
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x0c);
	lcd_cmd(0x06);
	lcd_clrscr();
	lcd_cmd(0x80);
	lcd_cur_pos(1,1);
	lcd_string("AZETECH");
}

void lcd_cursor()
{
	lcd_data(0x02&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(2);
	en_lo;
	
	lcd_data((0x02<<4)&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(2);
	en_lo;
}

void lcd_clrscr()
{
	lcd_data(0x01&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(2);
	en_lo;
	
	lcd_data((0x01<<4)&0xf0);
	rs_lo;
	en_hi;
	_delay_ms(2);
	en_lo;
}

void lcd_data(unsigned char a)
{
	if(a&0x10) {d4_hi;} else {d4_lo;}
	if(a&0x20) {d5_hi;} else {d5_lo;}
	if(a&0x40) {d6_hi;} else {d6_lo;}
	if(a&0x80) {d7_hi;} else {d7_lo;}
}



void lcd_cmd(unsigned char a)
{
	lcd_data(a&0xf0);
	rs_lo;
	en_hi;
	_delay_us(50);
	en_lo;
	
	lcd_data((a<<4)&0xf0);
	rs_lo;
	en_hi;
	_delay_us(50);
	en_lo;
}

void lcd_dat(unsigned char a)
{
	lcd_data(a&0xf0);
	rs_hi;
	en_hi;
	_delay_us(50);
	en_lo;
	
	lcd_data((a<<4)&0xf0);
	rs_hi;
	en_hi;
	_delay_us(50);
	en_lo;
}



void lcd_cur_pos(unsigned char b, unsigned char c)
{
	if(b==1) {lcd_cmd(0x80+(c-1));}
	if(b==2) {lcd_cmd(0xc0+(c-1));}
	if(b==3) {lcd_cmd(0x94+(c-1));}
	if(b==4) {lcd_cmd(0xd4+(c-1));}
}

void lcd_string(char *a)
{
	while(*a!='\0')
	{
		lcd_dat(*a++);
	}
}


void lcd_num_2dig(unsigned int aa)
{
	char a1;
	a1=aa%10;
	aa=aa/10;
	lcd_dat(aa+48);
	lcd_dat(a1+48);
}

void lcd_num_3dig(unsigned int aa)
{
	char a1,a2;
	a1=aa%10;
	aa=aa/10;
	a2=aa%10;
	aa=aa/10;
	lcd_dat(aa+48);
	lcd_dat(a2+48);
	lcd_dat(a1+48);
}

void lcd_num_point(unsigned int aa)
{
	char a1,a2;
	a1=aa%10;
	aa=aa/10;
	a2=aa%10;
	aa=aa/10;
	lcd_dat(aa+48);
	lcd_dat(a2+48);
	lcd_string(".");
	lcd_dat(a1+48);
}
/*
char * ftoa(float f, int * status)
{
	static char		buf[17];
	char *			cp = buf;
	unsigned long	l, rem;

	if(f < 0) {
		*cp++ = '-';
		f = -f;
	}
	l = (unsigned long)f;
	f -= (float)l;
	rem = (unsigned long)(f * 1e6);
	sprintf(cp, "%lu.%6.6lu", l, rem);
	return buf;
}
*/
/*
void num_2dig(unsigned int aa)
{
	char a1;
	a1=aa%10;
	aa=aa/10;
	str[0]=(aa+48);
	str[1]=(a1+48);
	str[2]='\0';
}

void num_3dig(unsigned int aa)
{
	char a1=0,a2=0;
	a1=aa%10;
	aa=aa/10;
	a2=aa%10;
	aa=aa/10;
	str[0]=(aa+48);
	str[1]=(a2+48);
	str[2]=(a1+48);
	str[3]='\0';
}

void num_point(unsigned int aa)
{
	char a1,a2;
	a1=aa%10;
	aa=aa/10;
	a2=aa%10;
	aa=aa/10;
	str[0]=(aa+48);
	str[1]=(a2+48);
	str[2]='.';
	str[3]=(a1+48);
	str[4]='\0';
}*/


void DebugStringRow1(char *data)
{
	lcd_cmd(0x80);
	lcd_string(data);
}

void DebugStringRow2(char *data)
{
	lcd_cmd(0xc0);
	lcd_string(data);
}