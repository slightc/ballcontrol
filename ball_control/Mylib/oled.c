#include "oled.h" 
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "math.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "can2.h"
#include "usart3.h"
#include "ov7670.h"

#define LCD_RST_1  HAL_GPIO_WritePin(LCD_GPIO,LCD_RST_PIN,GPIO_PIN_SET)
#define LCD_RST_0  HAL_GPIO_WritePin(LCD_GPIO,LCD_RST_PIN,GPIO_PIN_RESET)

#define LCD_DC_1   HAL_GPIO_WritePin(LCD_GPIO,LCD_DC_PIN,GPIO_PIN_SET)
#define LCD_DC_0   HAL_GPIO_WritePin(LCD_GPIO,LCD_DC_PIN,GPIO_PIN_RESET)

#define LCD_SCL_1  HAL_GPIO_WritePin(LCD_GPIO,LCD_SCL_PIN,GPIO_PIN_SET) //D0
#define LCD_SCL_0  HAL_GPIO_WritePin(LCD_GPIO,LCD_SCL_PIN,GPIO_PIN_RESET)

#define LCD_SDA_1  HAL_GPIO_WritePin(LCD_GPIO,LCD_SDA_PIN,GPIO_PIN_SET) //D1
#define LCD_SDA_0  HAL_GPIO_WritePin(LCD_GPIO,LCD_SDA_PIN,GPIO_PIN_RESET)

extern CAMERA_BUFFER_TYPE camera_buffer[CAMERA_BUFFER_H][CAMERA_BUFFER_W]; 
extern CAMERA_BUFFER_TYPE camera_frame[CAMERA_BUFFER_H][CAMERA_BUFFER_W];
extern CAMERA_BUFFER_TYPE camera_diff[CAMERA_BUFFER_H][CAMERA_BUFFER_W];

const unsigned char F8X16[]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//!1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//"2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//#3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//%5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//&6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//'7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//(8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//)9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//*10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//,12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//-13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//.14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,///15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//016
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//117
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//218
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//319
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//420
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//521
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//622
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//723
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//824
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//925
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//:26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//;27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//<28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//=29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//>30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//?31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//]61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//`64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//|92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//}93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~94
};


void oled_delay_ms(unsigned int t)
{
//	int i;
//	int a;
//	for( i=0;i<t;i++)
//	{
//		a = 42000;
//		while(a--);
//	}
		HAL_Delay(t);
	
}

void oled_delay_us(unsigned int t)
{
	int i=0;
	for( i=0;i<t;i++)
	{
		int a=40;
	//	int a=0;
		while(a--);
	}
	////i++;
}

void LCD_WrDat(unsigned char data)
{
	unsigned char i=8;
	LCD_DC_1;
    LCD_SCL_0;
    oled_delay_us(1);
    while(i--)
     {
       if(data&0x80) LCD_SDA_1;
       else          LCD_SDA_0;
       LCD_SCL_1; 
       oled_delay_us(1);         
       LCD_SCL_0;;    
       data<<=1;
     }
}

void LCD_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	LCD_DC_0;
    LCD_SCL_0;
    oled_delay_us(1);
    while(i--)
     {
       if(cmd&0x80)	 LCD_SDA_1;
       else          LCD_SDA_0;
       LCD_SCL_1;
       oled_delay_us(1);
       LCD_SCL_0;
       cmd<<=1;
     } 	
}

void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(((x&0xf0)>>4)|0x10);
  	LCD_WrCmd(x&0x0f); 
} 

void OLED_Clear(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<128;x++)
			LCD_WrDat(0);
	}
}

/***************************************************************/
void SetStartColumn(unsigned char d)
{
	LCD_WrCmd(0x00+d%16);   // Set Lower Column Start Address for Page Addressing Mode
						    // Default => 0x00
	LCD_WrCmd(0x10+d/16);   // Set Higher Column Start Address for Page Addressing Mode
						    // Default => 0x10
}
/***************************************************************/
void SetAddressingMode(unsigned char d)
{
	LCD_WrCmd(0x20);	// Set Memory Addressing Mode
	LCD_WrCmd(d);	    // Default => 0x02
						// 0x00 => Horizontal Addressing Mode
						// 0x01 => Vertical Addressing Mode
						// 0x02 => Page Addressing Mode
}
/***************************************************************/
void SetColumnAddress(unsigned char a, unsigned char b)
{
	LCD_WrCmd(0x21);	// Set Column Address
	LCD_WrCmd(a);	    // Default => 0x00 (Column Start Address)
	LCD_WrCmd(b);	    // Default => 0x7F (Column End Address)
}
/***************************************************************/
void SetPageAddress(unsigned char a, unsigned char b)
{
	LCD_WrCmd(0x22);	//Set Page Address
	LCD_WrCmd(a);		//Default => 0x00 (Page Start Address)
	LCD_WrCmd(b);	    //Default => 0x07 (Page End Address)
}
/***************************************************************/
void SetStartLine(unsigned char d)
{
	LCD_WrCmd(0x40|d);	// Set Display Start Line
						// Default => 0x40 (0x00)
}
/***************************************************************/
void SetContrastControl(unsigned char d)
{
	LCD_WrCmd(0x81);	//Set Contrast Control
	LCD_WrCmd(d);		//Default => 0x7F
}
/***************************************************************/
void Set_Charge_Pump(unsigned char d)
{
	LCD_WrCmd(0x8D);	//Set Charge Pump
	LCD_WrCmd(0x10|d);	//Default => 0x10
						//0x10 (0x00) => Disable Charge Pump
						//0x14 (0x04) => Enable Charge Pump
}
/***************************************************************/
void Set_Segment_Remap(unsigned char d)
{
	LCD_WrCmd(0xA0|d);  //Set Segment Re-Map
						//Default => 0xA0
						//0xA0 (0x00) => Column Address 0 Mapped to SEG0
						//0xA1 (0x01) => Column Address 0 Mapped to SEG127
}
/***************************************************************/
void Set_Entire_Display(unsigned char d)
{
	LCD_WrCmd(0xA4|d);  //Set Entire Display On / Off
						//Default => 0xA4
						//0xA4 (0x00) => Normal Display
						//0xA5 (0x01) => Entire Display On
}
/***************************************************************/
void Set_Inverse_Display(unsigned char d)
{
	LCD_WrCmd(0xA6|d);  //Set Inverse Display On/Off
						//Default => 0xA6
						//0xA6 (0x00) => Normal Display
						//0xA7 (0x01) => Inverse Display On
}
/***************************************************************/
void Set_Multiplex_Ratio(unsigned char d)
{
	LCD_WrCmd(0xA8);	//Set Multiplex Ratio
	LCD_WrCmd(d);	    //Default => 0x3F (1/64 Duty)
}
/***************************************************************/
void Set_Display_On_Off(unsigned char d)
{
	LCD_WrCmd(0xAE|d);  //Set Display On/Off
						//Default => 0xAE
						//0xAE (0x00) => Display Off
						//0xAF (0x01) => Display On
}
/***************************************************************/
void SetStartPage(unsigned char d)
{
	LCD_WrCmd(0xB0|d);	//Set Page Start Address for Page Addressing Mode
						//Default => 0xB0 (0x00)
}
/***************************************************************/
void Set_Common_Remap(unsigned char d)
{
	LCD_WrCmd(0xC0|d);	//Set COM Output Scan Direction
						//Default => 0xC0
						//0xC0 (0x00) => Scan from COM0 to 63
						//0xC8 (0x08) => Scan from COM63 to 0
}
/***************************************************************/
void Set_Display_Offset(unsigned char d)
{
	LCD_WrCmd(0xD3);    //Set Display Offset
	LCD_WrCmd(d);	    //Default => 0x00
}
/***************************************************************/
void Set_Display_Clock(unsigned char d)
{
	LCD_WrCmd(0xD5);	//Set Display Clock Divide Ratio / Oscillator Frequency
	LCD_WrCmd(d);	    //Default => 0x80
						//D[3:0] => Display Clock Divider
						//D[7:4] => Oscillator Frequency
}
/***************************************************************/
void Set_Precharge_Period(unsigned char d)
{
	LCD_WrCmd(0xD9);	//Set Pre-Charge Period
	LCD_WrCmd(d);		//Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//D[7:4] => Phase 2 Period in 1~15 Display Clocks
}
/***************************************************************/
void Set_Common_Config(unsigned char d)
{
	LCD_WrCmd(0xDA);	//Set COM Pins Hardware Configuration
	LCD_WrCmd(0x02|d);	//Default => 0x12 (0x10)
						//Alternative COM Pin Configuration
						//Disable COM Left/Right Re-Map
}
/***************************************************************/
void Set_VCOMH(unsigned char d)
{
	LCD_WrCmd(0xDB);	//Set VCOMH Deselect Level
	LCD_WrCmd(d);	    //Default => 0x20 (0.77*VCC)
}
/***************************************************************/
void Set_NOP(void)
{
	LCD_WrCmd(0xE3);	// Command for No Operation
}

//void LCD_P8x16Str(unsigned char x,unsigned char y,char ch[])
//{
//    char c=0,i=0,j=0;
//    while (ch[j]!='\0')
//     {
//       c =ch[j]-32;
//       if(x>120){x=0;y+=2;}
//			 
//       LCD_Set_Pos(x,y);  
//			 
//       for(i=0;i<8;i++)     
//				LCD_WrDat(F8X16[c*16+i]);
//			 
//       LCD_Set_Pos(x,y+1);    
//  	   for(i=0;i<8;i++)     
//  	   LCD_WrDat(F8X16[c*16+i+8]);  
//  	   x+=8;
//  	   j++;
//     }
//}

void LCD_P8x16Str(unsigned char x,unsigned char y,char ch[])
{
    char c=0,i=0,j=0;
    while (ch[j]!='\0')
     {
			 
				c =ch[j]-32;
			 if(ch[j]=='\r')
				 x+=16;
			 else if(ch[j]=='\n')
				{x=0;y+=2;} 
			 
       if(x>120){x=0;y+=2;}
			 
			 if(ch[j]>=32)
			 { 
				 LCD_Set_Pos(x,y);  
				 
				 for(i=0;i<8;i++)     
					LCD_WrDat(F8X16[c*16+i]);
				 
				 LCD_Set_Pos(x,y+1);    
				 for(i=0;i<8;i++)     
				 LCD_WrDat(F8X16[c*16+i+8]);  
				 x+=8; 	   
			 }
			 j++;
     }
}

void OLED_Print(unsigned char x, unsigned char y, char ch[])
{
	char ch2[3];
	unsigned char ii=0;
	while(ch[ii] != '\0')
	{
			ch2[0] = ch[ii];
			ch2[1] = '\0';
			LCD_P8x16Str(x,y,ch2);
			x += 8;
			ii+= 1;
	}
}


/**
  * @brief  lcd_printf 
  * @param  void
  * @retval void
  */
void OLED_Printf(unsigned char x,unsigned char y,const char *fmt,...)
{
	static unsigned char last_len[4]={0,0,0,0};	
	static char LCD_BUF[128]={0}; 
	unsigned char len;
	unsigned char i;
	__va_list ap;
	
	va_start(ap,fmt);
	vsprintf((char *)LCD_BUF,fmt,ap);
	va_end(ap);
	
	len=strlen((char *)LCD_BUF)+2*y;
	for(i=len;i<last_len[x];i++)
	{
		LCD_BUF[i-2*y]=' ';
	}
	LCD_BUF[i-2*y]=0;
//	OLED_Print(x,y,LCD_BUF);
	LCD_P8x16Str(x,y,LCD_BUF);
	last_len[x]=len;
}

void OLED_PrintN(unsigned char x, unsigned char y, char *fmt, int num)
{
	char buffer[20];
	unsigned x_data;
	itoa((int)num, buffer, 10);

	x_data = x + strlen(fmt) * 8 + strlen(buffer) * 8;
	LCD_WrCmd(0xb0 + y);
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10);

	LCD_Set_Pos(x_data, y);
	for (char i = x_data; i < 128; i++)
		LCD_WrDat(0);
	LCD_Set_Pos(x_data, y+1);
	for (char i = x_data; i < 128; i++)
		LCD_WrDat(0);

	LCD_P8x16Str(x, y, fmt);
	LCD_P8x16Str(x + strlen(fmt) * 8, y, buffer);

}

void OLED_Configuration(void)        
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__LCD_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin   = LCD_RST_PIN | LCD_DC_PIN | LCD_SCL_PIN | LCD_SDA_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(LCD_GPIO, &GPIO_InitStruct); 

	LCD_SCL_1;
	LCD_RST_0;
	oled_delay_ms(200);
	LCD_RST_1;

    Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
    Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec
    Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
    Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
    SetStartLine(0x00);			    // Set Mapping RAM Display Start Line (0x00~0x3F)
    Set_Charge_Pump(0x04);		    // Enable Embedded DC/DC Converter (0x00/0x04)
    SetAddressingMode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
    Set_Segment_Remap(0x01);		// Set SEG/Column Mapping     
    Set_Common_Remap(0x08);			// Set COM/Row Scan Direction 
    Set_Common_Config(0x10);		// Set Sequential Configuration (0x00/0x10)
    SetContrastControl(0xCF);	    // Set SEG Output Current
    Set_Precharge_Period(0xF1);		// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    Set_VCOMH(0x40);			    // Set VCOM Deselect Level
    Set_Entire_Display(0x00);		// Disable Entire Display On (0x00/0x01)
    Set_Inverse_Display(0x00);		// Disable Inverse Display On (0x00/0x01)  
    Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
    OLED_Clear();
	LCD_Set_Pos(0,0);
}

void plot(unsigned char data[])
{
	unsigned char y,x,i,j;	
	unsigned char disp[4][64];
	
//	for(j=0;j<64;j++)//���Ҳ���
//	{
//		data[j]=15*(sin(j*12.0/180.0*3.14159)+1);
//	}
	
	for(j=0;j<64;j++)
		for(i=0;i<4;i++)
		{
			//disp[i][j]=__RBIT(0x01<<data[j])>>(i*8);
		}
	for(y=4;y<8;y++)
	{
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(0x10);
  	LCD_WrCmd(0x00);
		for(x=0;x<64;x++)
		{
			LCD_WrDat(disp[y-4][x]);	
		}
	}

}

void plot_128(unsigned char data[])
{
	unsigned char y,x,i,j;	
	unsigned char disp[4][128];
	
//	for(j=0;j<64;j++)//���Ҳ���
//	{
//		data[j]=15*(sin(j*12.0/180.0*3.14159)+1);
//	}
	
	for(j=0;j<128;j++)
		for(i=0;i<4;i++)
		{
			disp[i][j]=__RBIT(0x01<<data[j])>>(i*8);
		}
	for(y=4;y<8;y++)
	{
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(0x10);
  	LCD_WrCmd(0x00);
		for(x=0;x<128;x++)
		{
			LCD_WrDat(disp[y-4][x]);	
		}
	}

}


void plot1(unsigned char data[])
{
	unsigned char y,x,i,j;	
	unsigned char disp[4][64];
	
//	for(j=0;j<64;j++)//���Ҳ���
//	{
//		data[j]=15*(sin(j*12.0/180.0*3.14159)+1);
//	}
	
	for(j=0;j<64;j++)
		for(i=0;i<4;i++)
		{
			disp[i][j]=__RBIT(0x01<<data[j])>>(i*8);
		}
	for(y=4;y<8;y++)
	{
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(((64&0xf0)>>4)|0x10);
  	LCD_WrCmd(64&0x0f);
		for(x=0;x<64;x++)
		{
			LCD_WrDat(disp[y-4][x]);	
		}
	}

}
void line(unsigned char x)
{
	unsigned char y,i,j;	
	
	for(y=4;y<8;y++)
	{
  	LCD_WrCmd(0xb0+y);
  	LCD_WrCmd(((x&0xf0)>>4)|0x10);
  	LCD_WrCmd(x&0x0f);
		LCD_WrDat(0xAA);	
	}
}

unsigned char show_block(int8_t x, int8_t y, int8_t x_now, int8_t line)
{
	int8_t set_x = 30, set_y = 33, out = 0, a = y+ set_y - line * 8;
	//y += set_y;
	if (x_now>(x+set_x-3) && x_now<(x + set_x + 3))
	{
		if (a >= 0 && a <= 7)
		{
			out = 0x1f << a;
		}
		else if ((int8_t)a > -5 && (int8_t)a < 0)
		{
			out = 0x1f >> (-a);
		}
	}

	return out;
}

unsigned char show_block2(int8_t x, int8_t y, int8_t x_now, int8_t line)
{
	int8_t set_x = 30, set_y = 34, out = 0, a = y + set_y - line * 8;
	//y += set_y;
	if (x_now > (x + set_x - 2) && x_now < (x + set_x + 2) )
	{
		if (a >= 0 && a <= 7)
		{
			out = 0x07 << a;
		}
		else if ((int8_t)a > -3 && (int8_t)a < 0)
		{
			out = 0x07 >> (-a);
		}
	}

	return out;
}

unsigned char show_block3(int8_t x, int8_t y, int8_t x_now, int8_t line)
{
	int8_t set_x = 30, set_y = 34, out = 0, a = y + set_y - line * 8;
	//y += set_y;
	if (x_now > (x + set_x - 2) && x_now < (x + set_x + 2) && x_now != (x + set_x))
	{
		if (a >= 0 && a <= 7)
		{
			out = 0x05 << a;
		}
		else if ((int8_t)a > -3 && (int8_t)a < 0)
		{
			out = 0x05 >> (-a);
		}
	}

	return out;
}

//void DisplayMap(char line)
//{
//	unsigned char y, x, i, j, a;
//	int8_t get_x = (int8_t)(Place_real[CH_X] *0.014286);
//	int8_t get_y = -(int8_t)(Place_real[CH_Y] *0.014286);
//
//		LCD_Set_Pos(0, line);
//		for (x = 0; x < 64; x++)
//		{
//			//
//			a = 0;
//			a |= show_block(get_x, get_y, x, line);
//			if (block_count > 0)
//			{
//				for (int i = 0; i < block_count; i++)
//				{
//					int8_t get_x = (int8_t)(blocks[i].real_x * 0.014286);
//					int8_t get_y = -(int8_t)(blocks[i].real_y * 0.014286);
//
//					if (blocks[i].height==1)
//					{
//						a |= show_block3(get_x, get_y, x, line);
//					}
//					else
//					{
//						a |= show_block2(get_x, get_y, x, line);
//					}
//				}
//			}
//			//a |= show_block2(-5, 0, x, line);
//			
//
//			if (line == 0)
//			{
//				if (x == 32 || x == 42)
//				{
//					a |= 0xfc;
//				} 
//				else if (x > 32 && x < 42)
//				{
//					a |= 0x04;
//				}
//			}
//			else if (line >0 && line < 4)
//			{
//				if (x == 32 || x == 42)
//					a |= 0xff;
//			}
//			else if (line == 4)
//			{
//				if (x == 2 || x == 42)
//					a |= 0xff;
//				else if (x > 2 && x < 33)
//				{
//					a |= 0x01;
//				}
//			}
//			else if (line > 4 && line < 7)
//			{
//				if (x == 2 || x == 42)
//					a |= 0xff;
//			}
//			else if (line == 7)
//			{
//				if (x == 2 || x == 42)
//					a |= 0x3f;
//				else if (x > 2 && x < 42)
//				{
//					a |= 0x20;;
//				}
//			}
//			LCD_WrDat(a);
//		}
//}

void oled_camera_display()
{
	uint16_t i, j, k, shift;
	uint16_t y, x;
	uint8_t data = 0;
	//unsigned char disp[8][128];

	//for (i = 0; i < 80; i++) {
	//	for (j=0;j<60;j++)
	//	{
	//		//if (val[i * 2][j * 2])
	//		{

	//		}
	//	}
	//}

	

	//	for(j=0;j<64;j++)//���Ҳ���
	//	{
	//		data[j]=15*(sin(j*12.0/180.0*3.14159)+1);
	//	}
	i = 0;
	for (y = 0; y < 7; y++)
	{
		LCD_WrCmd(0xb0 + y);
		LCD_WrCmd(((0 & 0xf0) >> 4) | 0x10);
		LCD_WrCmd(0 & 0x0f);

		for (x = 0; x < 60; x++)
		{
			uint8_t p_bit = 0;

			

			data = 0;
			for (shift = 0; shift < 8; shift++)
			{
				if (x % 2 == 0)
				{
					p_bit = (camera_frame[y * 32+shift*4][x/2] >> 7) & 0x01;
				}
				else {
					p_bit = (camera_frame[y * 32 + shift * 4][x/2] >> 3) & 0x01;
				}

				data |= p_bit <<shift;
			}

			//data = camera_buffer[x*5][(7-y)*5];
			//data = 0xAA;
			//data = __RBIT(data);
			LCD_WrDat(~data);
		}
	}
	
}