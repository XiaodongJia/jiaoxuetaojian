/*
 * 34_lcd.h
 *
 *  Created on: 2014年8月26日
 *      Author: Administrator
 */

#ifndef TFT_SCREEN_H_
#define TFT_SCREEN_H_

#define WHITE						0xFFFF
#define BLACK						0x0000
#define BLUE						0x001F
#define BRED						0XF81F
#define GRED						0XFF00
#define GBLUE						0X07FF
#define RED							0xF800
#define MAGENTA						0xF81F
#define GREEN						0x07E0
#define CYAN						0x7FFF
#define YELLOW						0xFFE0
#define BROWN						0XBC40 //棕色
#define BRRED						0XFC07 //棕红色
#define GRAY						0X8430 //灰色
#define SPILCD_W       480
#define SPILCD_H       320

#define RS_RST_OUT    P8OUT
#define CSOUT        P2OUT
#define RS           BIT2
#define RST          BIT1
#define CS           BIT6

extern void init_SPI(void);
extern void init_LCD_hardware(void);
extern void ILI9325_CMO24_Initial(void);
extern void SPILCD_SetWindow(unsigned int xstat,unsigned int xend,unsigned int ystat,unsigned int yend);
extern void Show_splash(void);
extern void Show_MenuTheme(void);
extern void Show_MenuTheme1(void);
extern void Show_MenuMode(int a);
extern void Show_ChanleMode(int a);
extern void Show_ACCELMenuMode(int a);
extern void Show_ACCEL_Select(int a);
extern void Show_TMenuMode(int a);
extern void Show_Select(void);
extern void Show_ButtonS1(void);
extern void Show_ButtonS2(void);
extern void Show_ButtonS3(void);
extern void Show_ButtonS4(void);
extern void Show_ALLTHEME(void);
extern void Show_TMenuTheme(void);
extern void Show_TranMenuMode(int a);
extern void transmit(int a);
extern void Complete(int a);
extern void DRAW_han(int x,int y,const unsigned char* ccc);
extern void DRAW_hanfan(int x,int y,const unsigned char* ccc);
extern void DRAW_Mi(int x,int y,const unsigned char* ccc);
extern void DRAW_Mifan(int x,int y,const unsigned char* ccc);
extern void DRAW_NUM1(int x,int y,const unsigned char* ccc);
extern void DRAW_NUM1fan(int x,int y,const unsigned char* ccc);
extern void DRAW_POINT(unsigned int x,unsigned int y,unsigned int color);
extern void DRAW_LINE(int x1,int y1,int x2,int y2,unsigned int color);
extern void DrawPixel(unsigned int x,unsigned int y, int Color);
extern void SPILCD_Clear(unsigned int Color);
extern void SPILCD_Clear_Lim(unsigned int x1,unsigned int x2,unsigned int y1,unsigned int y2,unsigned int Color);
extern void SPILCD_Clear_Fast(unsigned char single_Color);
extern void DRAW_NUM(int x,int y,unsigned char n,unsigned char color);
extern void DRAW_Other(int x,int y,unsigned char n);
extern void DRAW_Heart(int x,int y,unsigned char n);
extern void NONUSE_LCD(void);
extern void USE_LCD(void);
extern void drawbpletter(void);



#endif /* 34_LCD_H_ */
