/*
 * 34_lcd.c
 *
 *  Created on: 2014年8月26日
 *      Author: Administrator
 */

#include "msp430.h"
#include "TFT_Screen.h"
#include "char.h"
#include "math.h"
int i,j,c,r;
unsigned char buf;
int a,b;

void DRAW_han1616(int x,int y,const unsigned char* ccc);
void DRAW_NUM4848(int x,int y,const unsigned char* ccc);
void DRAW_han4848(int x,int y,const unsigned char* ccc);
void Delayms(unsigned int time);
void LCD_WriteRegIndex(unsigned char Index);
void LCD_WriteData(unsigned int dat);
void Lcd_SetCursor(unsigned int x,unsigned int y);
void LCD_WriteByteSPI(unsigned char a);
void clrrow(unsigned int rrr,unsigned int color);
void LCD_InvDisplayOn();
void LCD_InvDisplayOff();
void LCD_PutString(unsigned int x, unsigned int y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag);
void LCD_Fill_Pic(unsigned int x, unsigned int y,unsigned int pic_H, unsigned int pic_V, const unsigned char* pic);
void SPILCD_WriteRAM(unsigned int RGB_Code);
void SPILCD_WriteRAM_Prepare(void);
void SPILCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);

/******************************************
函数名：void init_SPI(void)
功能：对SPI进行初始化设置
******************************************/
void init_SPI(void)
{
	P3SEL |= BIT2+BIT1+BIT0;                  //P3.0SIMO---MOSI P3.1SOMI---MISO P3.2---CLK 外围模块功能
    UCB0CTL1 |= UCSWRST;                      //USCI逻辑在复位状态
    UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    //3脚， 8位数据，  SPI主机模式
    UCB0CTL1 |= UCSSEL_2;                     //SMCLK
    UCB0BR0 = 0x00;                           //
    UCB0BR1 = 0;                              //
    UCB0CTL1 &= ~UCSWRST;                     //USCI复位释放工作
    P3DIR|= BIT0+BIT1+BIT2;                   //P3.0 P3.1 P3.2 设置为输出
}

/******************************************
函数名：void init_LCD_hardware(void)
功能：对LCD用到的管脚进行初始化设置
******************************************/
void init_LCD_hardware(void)
{
	P8DIR |= BIT1+BIT2;  //端口初始化  RST---P8.1   RS----P8.2 设置为输出脚
	P2OUT |= BIT6;        //CS----P2.6输出为高
	P2DIR |= BIT6;       //CS----P2.6设置为输出脚
	P3SEL |= BIT2+BIT1+BIT0;    // P3.0 3.1 3.2 外围模块功能脚SPI
}

/******************************************
函数名：void ILI9325_CMO24_Initial(void)
功能：对LCD用到的管脚进行初始化设置
******************************************/
void ILI9325_CMO24_Initial(void)
{
	Delayms(10);       //延时10ms
	CSOUT |=CS;        //片选信号  CS
	Delayms(150);      //延时150ms
	CSOUT &=~CS;
	Delayms(25);       //延时10ms
	RS_RST_OUT&=~RST;  //复位信号  RST
	Delayms(150);      //延时150ms
	RS_RST_OUT|=RST;
	Delayms(150);      //延时150ms

	LCD_WriteRegIndex(0x00B0);//{setc, [107], W, 0x000B0}
    LCD_WriteByteSPI(0x0000);//{setp, [104], W, 0x00000}

    LCD_WriteRegIndex(0x00B3);
    LCD_WriteByteSPI( 0x0002);
    LCD_WriteByteSPI(0x0000);
    LCD_WriteByteSPI(0x0000);
    LCD_WriteByteSPI(0x0000);

    LCD_WriteRegIndex(0x00B4);
    LCD_WriteByteSPI(0x0000);

    LCD_WriteRegIndex(0x00C0);
    LCD_WriteByteSPI(0x0013);//
    LCD_WriteByteSPI( 0x003B);//480
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0003);//02
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0001);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0043);

    LCD_WriteRegIndex( 0x00C1);
    LCD_WriteByteSPI( 0x0008);
    LCD_WriteByteSPI( 0x0012);//CLOCK
    LCD_WriteByteSPI( 0x0008);
    LCD_WriteByteSPI( 0x0008);

    LCD_WriteRegIndex( 0x00C4);
    LCD_WriteByteSPI( 0x0011);
    LCD_WriteByteSPI( 0x0007);
    LCD_WriteByteSPI( 0x0003);
    LCD_WriteByteSPI( 0x0003);

    LCD_WriteRegIndex( 0x00C8);//GAMMA
    LCD_WriteByteSPI( 0x0004);
    LCD_WriteByteSPI( 0x0009);
    LCD_WriteByteSPI( 0x0016);
    LCD_WriteByteSPI( 0x005A);
    LCD_WriteByteSPI( 0x0002);
    LCD_WriteByteSPI( 0x000A);
    LCD_WriteByteSPI( 0x0016);
    LCD_WriteByteSPI( 0x0005);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0032);
    LCD_WriteByteSPI( 0x0005);
    LCD_WriteByteSPI( 0x0016);
    LCD_WriteByteSPI( 0x000A);
    LCD_WriteByteSPI( 0x0053);//43/55
    LCD_WriteByteSPI( 0x0008);
    LCD_WriteByteSPI( 0x0016);
    LCD_WriteByteSPI( 0x0009);
    LCD_WriteByteSPI( 0x0004);
    LCD_WriteByteSPI( 0x0032);
    LCD_WriteByteSPI( 0x0000);

    LCD_WriteRegIndex( 0x002A);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0001);
    LCD_WriteByteSPI( 0x003F);//320

    LCD_WriteRegIndex( 0x002B);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0001);
    LCD_WriteByteSPI( 0x00DF);//480

    LCD_WriteRegIndex( 0x0035);
    LCD_WriteByteSPI( 0x0000);

    LCD_WriteRegIndex( 0x003A);
    LCD_WriteByteSPI( 0x0055);

    LCD_WriteRegIndex( 0x0044);
    LCD_WriteByteSPI( 0x0000);
    LCD_WriteByteSPI( 0x0001);

    LCD_WriteRegIndex( 0x002C);
    LCD_WriteRegIndex( 0x0011);

    Delayms(150);

    LCD_WriteRegIndex( 0x00D0);
    LCD_WriteByteSPI( 0x0007);
    LCD_WriteByteSPI( 0x0007);
    LCD_WriteByteSPI( 0x001E);
    LCD_WriteByteSPI( 0x07);
    LCD_WriteByteSPI( 0x03);
    LCD_WriteRegIndex( 0x00D1);
    LCD_WriteByteSPI( 0x0003);
    LCD_WriteByteSPI( 0x0052);//VCM
    LCD_WriteByteSPI( 0x0010);//VDV

    LCD_WriteRegIndex( 0x00D2);
    LCD_WriteByteSPI( 0x0003);
    LCD_WriteByteSPI( 0x0024);

    LCD_WriteRegIndex(0x00B0);//{setc, [107], W, 0x000B0}
    LCD_WriteByteSPI(0x0000);//{setp, [104], W, 0x00000}

    LCD_WriteRegIndex(0x0029);
    Delayms(10);
    LCD_WriteRegIndex(0x0036);
    LCD_WriteByteSPI(0x0028);
}

void Show_splash(void){
	for(a=0;a<9;a++){
		DRAW_han4848(456-48*a,150,Bug1[a]);}
    for(a=0;a<2;a++){
        DRAW_han4848(360-48*a,220,Bug2[a]);}
    for(a=0;a<6;a++){
        DRAW_NUM4848(264-24*a,220,Bug3[a]);}
    for(a=0;a<4;a++){
        DRAW_han1616(70-16*a,30,SEU[a]);}
    for(a=0;a<11;a++){
        DRAW_han1616(182-16*a,10,BME[a]);}
}

void Show_MenuTheme(void){
    for(a=0;a<14;a++){
    	DRAW_han(464-32*a,300,Theme[a]);}
    for(b=0;b<3;b++){
    	DRAW_NUM1(384-16*b,269,MSP[b]);}
        DRAW_NUM1(384-16*3,269,num[4]);
        DRAW_NUM1(384-16*4,269,num[3]);
        DRAW_NUM1(384-16*5,269,num[0]);

        DRAW_LINE(480,260,0,260,RED);

        DRAW_LINE(480,45,0,45,RED);
 }

void Show_MenuTheme1(void){
    for(a=0;a<14;a++){
    	DRAW_han(464-32*a,300,Theme[a]);}
}

void Show_Select(void){
    for(a=0;a<2;a++){
    	DRAW_han(416-32*a,40,Sure[a]);
    }

    DRAW_han(288,40,Up[0]);

    DRAW_han(224,40,Down[0]);

    for(a=0;a<2;a++){
    	DRAW_han(128-32*a,40,Back[a]);
    }
}

void Show_ButtonS1(void){
    for(a=0;a<2;a++){
    	DRAW_hanfan(416-32*a,40,Sure[a]);
    }
}

void Show_ButtonS2(void){
	DRAW_hanfan(288,40,Up[0]);
}

void Show_ButtonS4(void){
    for(a=0;a<2;a++){
    	DRAW_hanfan(128-32*a,40,Back[a]);
    }
}

void Show_ButtonS3(void){
	DRAW_hanfan(224,40,Down[0]);
}

void Show_ACCELMenuMode(int a){
	if(a==1){
		DRAW_Mifan(448,209,Mi[0]);
        DRAW_hanfan(448,240,ATheme2[0]);
        DRAW_hanfan(416,240,ATheme2[1]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,ATheme3[0]);
        DRAW_han(416,200,ATheme3[1]);}

	if(a==2){
		DRAW_Mi(448,209,Mi[0]);
 	    DRAW_han(448,240,ATheme2[0]);
 	    DRAW_han(416,240,ATheme2[1]);
 	    DRAW_Mifan(448,169,Mi[0]);
 	    DRAW_hanfan(448,200,ATheme3[0]);
 	    DRAW_hanfan(416,200,ATheme3[1]);}
}

void Show_ACCEL_Select(int a){
	if(a==1){
		DRAW_Mifan(448,209,Mi[0]);
        DRAW_hanfan(448,240,ATheme4[0]);
        DRAW_hanfan(416,240,ATheme4[2]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,ATheme4[1]);
        DRAW_han(416,200,ATheme4[2]);}

 	if(a==2){
 		DRAW_Mi(448,209,Mi[0]);
 	    DRAW_han(448,240,ATheme4[0]);
 	    DRAW_han(416,240,ATheme4[2]);
 	    DRAW_Mifan(448,169,Mi[0]);
 	    DRAW_hanfan(448,200,ATheme4[1]);
 	    DRAW_hanfan(416,200,ATheme4[2]);}
}

void Show_TranMenuMode(int a){
	if(a==1){
		DRAW_Mifan(448,209,Mi[0]);
 		DRAW_NUM1fan(432,209,MSP[3]);
 		DRAW_NUM1fan(416,209,MSP[4]);
 		DRAW_NUM1fan(400,209,MSP[5]);
 		DRAW_NUM1fan(384,209,MSP[6]);
 		DRAW_Mi(448,169,Mi[0]);
 		DRAW_NUM1(432,169,MSP[3]);
 		DRAW_NUM1(416,169,MSP[1]);
 		DRAW_NUM1(400,169,MSP[7]);
 		DRAW_Mi(448,129,Mi[0]);
 		DRAW_NUM1(432,129,MSP[7]);
 		DRAW_NUM1(416,129,MSP[6]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_NUM1(432,89,MSP[8]);
        DRAW_NUM1(416,89,MSP[9]);
        DRAW_NUM1(400,89,MSP[8]);
        DRAW_NUM1(384,89,MSP[10]);}

	if(a==2){
        DRAW_Mi(448,209,Mi[0]);
        DRAW_NUM1(432,209,MSP[3]);
        DRAW_NUM1(416,209,MSP[4]);
        DRAW_NUM1(400,209,MSP[5]);
        DRAW_NUM1(384,209,MSP[6]);
        DRAW_Mifan(448,169,Mi[0]);
        DRAW_NUM1fan(432,169,MSP[3]);
        DRAW_NUM1fan(416,169,MSP[1]);
        DRAW_NUM1fan(400,169,MSP[7]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_NUM1(432,129,MSP[7]);
        DRAW_NUM1(416,129,MSP[6]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_NUM1(432,89,MSP[8]);
        DRAW_NUM1(416,89,MSP[9]);
        DRAW_NUM1(400,89,MSP[8]);
        DRAW_NUM1(384,89,MSP[10]);}

	if(a==3){
        DRAW_Mi(448,209,Mi[0]);
        DRAW_NUM1(432,209,MSP[3]);
        DRAW_NUM1(416,209,MSP[4]);
        DRAW_NUM1(400,209,MSP[5]);
        DRAW_NUM1(384,209,MSP[6]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_NUM1(432,169,MSP[3]);
        DRAW_NUM1(416,169,MSP[1]);
        DRAW_NUM1(400,169,MSP[7]);
        DRAW_Mifan(448,129,Mi[0]);
        DRAW_NUM1fan(432,129,MSP[7]);
        DRAW_NUM1fan(416,129,MSP[6]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_NUM1(432,89,MSP[8]);
        DRAW_NUM1(416,89,MSP[9]);
        DRAW_NUM1(400,89,MSP[8]);
        DRAW_NUM1(384,89,MSP[10]);}

	if(a==4){
        DRAW_Mi(448,209,Mi[0]);
        DRAW_NUM1(432,209,MSP[3]);
        DRAW_NUM1(416,209,MSP[4]);
        DRAW_NUM1(400,209,MSP[5]);
        DRAW_NUM1(384,209,MSP[6]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_NUM1(432,169,MSP[3]);
        DRAW_NUM1(416,169,MSP[1]);
        DRAW_NUM1(400,169,MSP[7]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_NUM1(432,129,MSP[7]);
        DRAW_NUM1(416,129,MSP[6]);
        DRAW_Mifan(448,89,Mi[0]);
        DRAW_NUM1fan(432,89,MSP[8]);
        DRAW_NUM1fan(416,89,MSP[9]);
        DRAW_NUM1fan(400,89,MSP[8]);
        DRAW_NUM1fan(384,89,MSP[10]);}
}

void Show_ChanleMode(int a){
	if(a==1){
		DRAW_Mifan(448,209,Mi[0]);
        DRAW_hanfan(448,240,Chanle[0]);
        DRAW_hanfan(416,240,Chanle[1]);
        DRAW_hanfan(384,240,Chanle[2]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,Chanle[0]);
        DRAW_han(416,200,Chanle[1]);
        DRAW_han(384,200,Chanle[3]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_han(448,160,Chanle[0]);
        DRAW_han(416,160,Chanle[1]);
        DRAW_han(384,160,Chanle[4]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_han(448,120,Chanle[0]);
        DRAW_han(416,120,Chanle[1]);
        DRAW_han(384,120,Chanle[5]);}

	if(a==2){
		DRAW_Mi(448,209,Mi[0]);
        DRAW_han(448,240,Chanle[0]);
        DRAW_han(416,240,Chanle[1]);
        DRAW_han(384,240,Chanle[2]);
        DRAW_Mifan(448,169,Mi[0]);
        DRAW_hanfan(448,200,Chanle[0]);
        DRAW_hanfan(416,200,Chanle[1]);
        DRAW_hanfan(384,200,Chanle[3]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_han(448,160,Chanle[0]);
        DRAW_han(416,160,Chanle[1]);
        DRAW_han(384,160,Chanle[4]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_han(448,120,Chanle[0]);
        DRAW_han(416,120,Chanle[1]);
        DRAW_han(384,120,Chanle[5]);}

 	if(a==3){
		DRAW_Mi(448,209,Mi[0]);
        DRAW_han(448,240,Chanle[0]);
        DRAW_han(416,240,Chanle[1]);
        DRAW_han(384,240,Chanle[2]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,Chanle[0]);
        DRAW_han(416,200,Chanle[1]);
        DRAW_han(384,200,Chanle[3]);
        DRAW_Mifan(448,129,Mi[0]);
        DRAW_hanfan(448,160,Chanle[0]);
        DRAW_hanfan(416,160,Chanle[1]);
        DRAW_hanfan(384,160,Chanle[4]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_han(448,120,Chanle[0]);
        DRAW_han(416,120,Chanle[1]);
        DRAW_han(384,120,Chanle[5]);}

 	if(a==4){
		DRAW_Mi(448,209,Mi[0]);
        DRAW_han(448,240,Chanle[0]);
        DRAW_han(416,240,Chanle[1]);
        DRAW_han(384,240,Chanle[2]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,Chanle[0]);
        DRAW_han(416,200,Chanle[1]);
        DRAW_han(384,200,Chanle[3]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_han(448,160,Chanle[0]);
        DRAW_han(416,160,Chanle[1]);
        DRAW_han(384,160,Chanle[4]);
        DRAW_Mifan(448,89,Mi[0]);
        DRAW_hanfan(448,120,Chanle[0]);
        DRAW_hanfan(416,120,Chanle[1]);
        DRAW_hanfan(384,120,Chanle[5]);}
}

void Show_MenuMode(int a){
	if(a==1){
		DRAW_Mifan(448,209,Mi[0]);
        DRAW_hanfan(448,240,Hhan[0]);
        DRAW_hanfan(416,240,Hhan[1]);
        DRAW_Mi(448,169,Mi[0]);
        DRAW_han(448,200,Phan[0]);
        DRAW_han(416,200,Phan[1]);
        DRAW_Mi(448,129,Mi[0]);
        DRAW_han(448,160,Shan[0]);
        DRAW_han(416,160,Shan[1]);
        DRAW_Mi(448,89,Mi[0]);
        DRAW_han(448,120,Ahan[0]);
        DRAW_han(416,120,Ahan[1]);
        DRAW_han(384,120,Ahan[2]);}

 	if(a==2){
 		DRAW_Mi(448,209,Mi[0]);
 	    DRAW_han(448,240,Hhan[0]);
 	    DRAW_han(416,240,Hhan[1]);
 	    DRAW_Mifan(448,169,Mi[0]);
 	    DRAW_hanfan(448,200,Phan[0]);
 	    DRAW_hanfan(416,200,Phan[1]);
 	    DRAW_Mi(448,129,Mi[0]);
 	    DRAW_han(448,160,Shan[0]);
 	    DRAW_han(416,160,Shan[1]);
 	    DRAW_Mi(448,89,Mi[0]);
 	    DRAW_han(448,120,Ahan[0]);
 	    DRAW_han(416,120,Ahan[1]);
 	    DRAW_han(384,120,Ahan[2]);}

 	if(a==3){
 	    DRAW_Mi(448,209,Mi[0]);
 	    DRAW_han(448,240,Hhan[0]);
 	    DRAW_han(416,240,Hhan[1]);
 	    DRAW_Mi(448,169,Mi[0]);
 	    DRAW_han(448,200,Phan[0]);
 	    DRAW_han(416,200,Phan[1]);
 	    DRAW_Mifan(448,129,Mi[0]);
 	    DRAW_hanfan(448,160,Shan[0]);
 	    DRAW_hanfan(416,160,Shan[1]);
 	    DRAW_Mi(448,89,Mi[0]);
 	    DRAW_han(448,120,Ahan[0]);
 	    DRAW_han(416,120,Ahan[1]);
 	    DRAW_han(384,120,Ahan[2]);}

 	if(a==4){
 	    DRAW_Mi(448,209,Mi[0]);
 	    DRAW_han(448,240,Hhan[0]);
 	    DRAW_han(416,240,Hhan[1]);
 	    DRAW_Mi(448,169,Mi[0]);
 	    DRAW_han(448,200,Phan[0]);
 	    DRAW_han(416,200,Phan[1]);
 	    DRAW_Mi(448,129,Mi[0]);
 	    DRAW_han(448,160,Shan[0]);
 	    DRAW_han(416,160,Shan[1]);
 	    DRAW_Mifan(448,89,Mi[0]);
 	    DRAW_hanfan(448,120,Ahan[0]);
 	    DRAW_hanfan(416,120,Ahan[1]);
 	    DRAW_hanfan(384,120,Ahan[2]);}
}

void Show_TMenuTheme(void){
	SPILCD_Clear_Lim(0,100,0,120,WHITE);

	DRAW_LINE(0,120,100,120,RED);
	DRAW_LINE(100,0,100,120,RED);
	DRAW_LINE(0,0,0,120,RED);
}

void Show_ALLTHEME(void){
	DRAW_Heart(432,15,0);
	DRAW_Heart(432,280,1);
	DRAW_han(420,310,HRhan[0]);
	DRAW_han(380,310,HRhan[1]);
	DRAW_han(340,310,HRhan[2]);
	DRAW_han(300,310,HRhan[3]);
	DRAW_Other(320,16,2);
	DRAW_Other(250,274,2);
	DRAW_LINE(15,60,465,60,RED);
}

void transmit(int a){
	int s;
	for(s=0;s<3;s++){
		DRAW_han(99-32*s,40*(3-a)+31,Transmit[s]);}
}

void Complete(int a){
	SPILCD_Clear_Lim(0,99,40*(3-a),40*(3-a)+32,WHITE);
	int s;
	for(s=0;s<2;s++){
		DRAW_han(99-32*s,40*(3-a)+32,Transmit[s+3]);}
}

void DRAW_han1616(int x,int y,const unsigned char* ccc)
{
   //Address_set(x,x+31,y,y+15);
	SPILCD_SetWindow(x,x+15,y,y+15);

      //  SPILCD_WriteRAM(color);

	for (i=0;i<32;i++){
		for(j=0;j<8;j++){
			if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
			else SPILCD_WriteRAM(WHITE);
       //temp>>j;
			}
	}
}

void DRAW_NUM4848(int x,int y,const unsigned char* ccc)
{
   //Address_set(x,x+31,y,y+15);
	SPILCD_SetWindow(x-23,x,y-47,y);

      //  SPILCD_WriteRAM(color);

	for (i=0;i<144;i++){
		for(j=0;j<8;j++){
			if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
			else SPILCD_WriteRAM(WHITE);
       //temp>>j;
			}
	}
}

void DRAW_han4848(int x,int y,const unsigned char* ccc)
{
	SPILCD_SetWindow(x-47,x,y-47,y);
	for (i=0;i<288;i++){
		for(j=0;j<8;j++){
			if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
			else SPILCD_WriteRAM(WHITE);//temp>>j;
			}
	}
}

void DRAW_hanfan(int x,int y,const unsigned char* ccc)
{
	SPILCD_SetWindow(x-31,x,y-31,y);
	for (i=0;i<128;i++){
		for(j=0;j<8;j++){
			if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
			else SPILCD_WriteRAM(BLACK);
       //temp>>j;
			}
	}
}

void DRAW_Mi(int x,int y,const unsigned char* ccc)
{
	SPILCD_SetWindow(x,x+31,y,y+31);
	for (i=0;i<128;i++){
		for(j=0;j<8;j++){
			if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
			else SPILCD_WriteRAM(WHITE);
		}
	}
}

 void DRAW_Mifan(int x,int y,const unsigned char* ccc)
 {
     SPILCD_SetWindow(x,x+31,y,y+31);
       for (i=0;i<128;i++){
 	   for(j=0;j<8;j++){
          if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
          else SPILCD_WriteRAM(BLACK);
 	   }
       }
 }

 void DRAW_NUM1(int x,int y,const unsigned char* ccc)
 {
   //Address_set(x,x+31,y,y+15);
   SPILCD_SetWindow(x,x+15,y,y+31);

      //  SPILCD_WriteRAM(color);

   for (i=0;i<64;i++){
     for(j=0;j<8;j++){
       if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
       else SPILCD_WriteRAM(WHITE);
       //temp>>j;
     }
   }
 }

 void DRAW_NUM1fan(int x,int y,const unsigned char* ccc)
 {
   //Address_set(x,x+31,y,y+15);
   SPILCD_SetWindow(x,x+15,y,y+31);

      //  SPILCD_WriteRAM(color);

   for (i=0;i<64;i++){
     for(j=0;j<8;j++){
       if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
       else SPILCD_WriteRAM(BLACK);
       //temp>>j;
     }
   }
 }
  /*************************************************
  函数名：Lcd光标起点定位函数
  功能：指定320240液晶上的一点作为写数据的起始点
  入口参数：x 坐标 0~239
            y 坐标 0~319
  返回值：无
  *************************************************/
   //函数名：LCD_WR_REG
  //参  数：无
  //        写GRAM数据
  void SPILCD_WriteRAM(unsigned int RGB_Code)
  {
      LCD_WriteData(RGB_Code);
  }
  //函数名：LCD_SetWindow
  //参  数：Xpos:横坐标
  //				Ypos:纵坐标

  //开启反色显示
  void LCD_InvDisplayOn()
  {
    LCD_WriteRegIndex(0x21);
  }
  //关闭反色显示
  void LCD_InvDisplayOff()
  {
    LCD_WriteRegIndex(0x20);
  }

  //函数名：SPILCD_Init
  //参  数：X Y 坐标
  //        在X Y上打点
  void DRAW_POINT(unsigned int x,unsigned int y,unsigned int color)
  {
  	SPILCD_SetWindow(x,x+1,y,y+1);//设置光标位置
  	//SPILCD_WriteRAM_Prepare();     //开始写入GRAM
  	SPILCD_WriteRAM(color);
  }
  void DRAW_LINE(int x1,int y1,int x2,int y2,unsigned int color)
  {
    int t;
  	int xerr=0,yerr=0,delta_x,delta_y,distance;
  	int incx,incy,uRow,uCol;

  	delta_x=x2-x1; //计算坐标增量
  	delta_y=y2-y1;
  	uRow=x1;
  	uCol=y1;
  	if(delta_x>0)incx=1; //设置单步方向
  	else if(delta_x==0)incx=0;//垂直线
  	else {incx=-1;delta_x=-delta_x;}
  	if(delta_y>0)incy=1;
  	else if(delta_y==0)incy=0;//水平线
  	else{incy=-1;delta_y=-delta_y;}
  	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
  	else distance=delta_y;
  	for(t=0;t<=distance+1;t++ )//画线输出
  	{
  		DRAW_POINT(uRow,uCol,color);//画点
  		xerr+=delta_x ;
  		yerr+=delta_y ;
  		if(xerr>distance)
  		{
  			xerr-=distance;
  			uRow+=incx;
  		}
  		if(yerr>distance)
  		{
  			yerr-=distance;
  			uCol+=incy;
  		}
  	}

  }
  void clrrow(unsigned int rrr,unsigned int color)
  {
         SPILCD_SetWindow(0,319,rrr,rrr+1);
         for(i=0;i<320;i++){
         SPILCD_WriteRAM(color);
         }
  }
  void SPILCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
  {
  	unsigned short i,j;
  	//设置窗口
  	SPILCD_SetWindow(xsta,xend,ysta,yend);
  	for(i=xsta;i<xend;i++)
  		for(j=ysta;j<yend;j++)
  		   	SPILCD_WriteRAM(color);	  //显示所填充的颜色.
  	//恢复设置
  	SPILCD_SetWindow(0,SPILCD_W-1,0,SPILCD_H-1);
  }
  //函数名：SPILCD_Clear
  //参  数：Color 颜色
  void SPILCD_Clear(unsigned int Color)
  {
  	unsigned short x,y;
  	SPILCD_SetWindow(0,SPILCD_W-1,0,SPILCD_H-1);
  	for(x=0;x<SPILCD_W;x++)
  		for(y=0;y<SPILCD_H;y++)
  	{
  		SPILCD_WriteRAM(Color);//显示所填充的颜色.
  	}
  }
  void SPILCD_Clear_Lim(unsigned int x1,unsigned int x2,unsigned int y1,unsigned int y2,unsigned int Color)
   {
   	unsigned int x,y;
   	SPILCD_SetWindow(x1,x2,y1,y2);
   	for(x=x1;x<=x2;x++)
   		for(y=y1;y<y2;y++)
   	{
   		SPILCD_WriteRAM(Color);//显示所填充的颜色.
   	}
   }
  //函数名：SPILCD_Clear_Fast
  //参  数：single_Color 单色  只能是0或1，0是黑色，1为白色
  void SPILCD_Clear_Fast(unsigned char single_Color)
  {
  	unsigned int x,y;
    int temp;
  	SPILCD_SetWindow(0,SPILCD_W-1,0,SPILCD_H-1);
             P2OUT|=BIT6;
          if(single_Color) { P3OUT|= BIT1;}
          else  {P3OUT&=~BIT1;}

  	temp=SPILCD_W<<4;
  	for(x=0;x<temp;x++)
  		for(y=0;y<SPILCD_H;y++)
  	{
  	P3OUT&=~BIT2;
          P3OUT|=BIT2;
  	}
  }
  void DrawPixel(unsigned int x,unsigned int y, int Color)
  {
  	SPILCD_SetWindow(x,x,y,y);
    SPILCD_WriteRAM(Color);
  }

  void LCD_PutString(unsigned int x, unsigned int y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) {
  	 unsigned char l=0;
  	while(*s) {
  		PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
  		s+=2;l+=2;
  	}
  	}
  /******************************************
  函数名：Lcd图像填充100*100
  功能：向Lcd指定位置填充图像
  入口参数：
  ******************************************/
  void LCD_Fill_Pic(unsigned int x, unsigned int y,unsigned int pic_H, unsigned int pic_V, const unsigned char* pic)
  {
    unsigned long i;
   	SPILCD_SetWindow(x,x+pic_H-1,y,y+pic_V-1);
  	lcd_RS(1);
  	for (i = 0; i < pic_H*pic_V*2; i++)
  	{
      LCD_WriteByteSPI(pic[i]);
  	}

  }
  void DRAW_NUM(int x,int y,unsigned char n,unsigned char color)
  {
      SPILCD_SetWindow(x,x+15,y,y+31);
      for (i=0;i<64;i++){
 	   for(j=0;j<8;j++){
          if(num[n][i]&(0x01<<j)) SPILCD_WriteRAM(color);
          else SPILCD_WriteRAM(WHITE);
                }
      }
  }

  void DRAW_han(int x,int y,const unsigned char* ccc)
  {
    SPILCD_SetWindow(x-31,x,y-31,y);
    for (i=0;i<128;i++){
      for(j=0;j<8;j++){
        if(*(ccc+i)&(0x01<<j)) SPILCD_WriteRAM(RED);
        else SPILCD_WriteRAM(WHITE);

      }
    }
  }
  void DRAW_Other(int x,int y,unsigned char n)
  {
     SPILCD_SetWindow(x,x+15,y,y+31);
       for (i=0;i<64;i++){
 	   for(j=0;j<8;j++){
          if(other[n][i]&(0x01<<j)) SPILCD_WriteRAM(RED);
          else SPILCD_WriteRAM(WHITE);
  }
      }
  }

  void DRAW_Heart(int x,int y,unsigned char n)
   {
      SPILCD_SetWindow(x,x+31,y,y+27);
        for (i=0;i<112;i++){
  	   for(j=0;j<8;j++){
           if(heart[n][i]&(0x01<<j)) SPILCD_WriteRAM(RED);
           else SPILCD_WriteRAM(WHITE);
   }
       }
   }
  void Delayms(unsigned int time)
   {
   	unsigned short i,j;
   	for(i=0;i<time;i++)
   		for(j=0;j<260;j++)	;
   }
  void LCD_WriteByteSPI(unsigned char a)
  {
        while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
        UCB0TXBUF = a;
        while (!(UCB0IFG&UCTXIFG));

  }
  void LCD_WriteRegIndex(unsigned char Index)
  {
	  RS_RST_OUT&=~RS;                   //RS位
	  	LCD_WriteByteSPI(Index);	//upper eight bits
	  	RS_RST_OUT|=RS;
  }
  void LCD_WriteData(unsigned int dat)
  {
	  RS_RST_OUT|=RS;
	  LCD_WriteByteSPI(dat>>8);		//	start byte RS=1,RW=0----Write a GRAM data
	  LCD_WriteByteSPI(dat);
  }
  void SPILCD_WriteRAM_Prepare(void)
   {
   	LCD_WriteRegIndex(0x22);   //写RAM
   }
  void Lcd_SetCursor(unsigned int x,unsigned int y)
  {
    LCD_WriteRegIndex(0x20);
    LCD_WriteData(x);//水平坐标
    LCD_WriteRegIndex(0x21);
    LCD_WriteData(y);//垂直坐标
  }
  void SPILCD_SetWindow(unsigned int xstat,unsigned int xend,unsigned int ystat,unsigned int yend)
   {
   	 LCD_WriteRegIndex(0x2A);
      LCD_WriteByteSPI(xstat>>8);
      LCD_WriteByteSPI(xstat);
      LCD_WriteByteSPI(xend>>8);
      LCD_WriteByteSPI(xend);

      LCD_WriteRegIndex(0x2B);
      LCD_WriteByteSPI(ystat>>8);
      LCD_WriteByteSPI(ystat);
      LCD_WriteByteSPI(yend>>8);
   	 LCD_WriteByteSPI(yend);

   	 LCD_WriteRegIndex(0x2C);

   }

void NONUSE_LCD(void)
{
	CSOUT |=CS;        //片选信号  CS
    Delayms(15);
}

void USE_LCD(void)
{
	CSOUT &= ~CS;        //片选信号  CS
	Delayms(15);
          //为DA
    UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCB0CTL0 = UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
                                                // Clock polarity high, MSB
    UCB0CTL1 |= UCSSEL_2;                     // SMCLK
    UCB0BR0 = 0x00;                           // /2
    UCB0BR1 = 0;                              //
                             // No modulation
    UCB0CTL1 &= ~UCSWRST;
}


