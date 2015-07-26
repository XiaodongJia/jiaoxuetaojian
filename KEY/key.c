/*
 * key.c
 *
 *  Created on: 2014-7-28
 *      Author: admin
 */
#include "msp430f5529.h"
#include "key.h"
#include "delay.h"

volatile int BUTTON_S1=0;
volatile int BUTTON_S2=0;
volatile int BUTTON_S3=0;
volatile int BUTTON_S4=0;
volatile int buttonsPressed=0;

/******************************************
函数名：void init_key(void)
功能：对行列式键盘用到的管脚进行初始化设置
******************************************/
void init_key(void)
{
	P1DIR |= (BIT3+BIT2);  //P1.2 P1.3 设置为输出
	P1OUT &= ~(BIT3+BIT2); //P1.2 P1.3 输出位低
	P1DIR &= ~(BIT4+BIT5); //P1.4 P1.5 设置为输入
	P1REN |= (BIT4+BIT5);  //P1.4 P1.5 寄存器使能
	P1OUT |= (BIT4+BIT5);  //P1.4 P1.5 置高
	P1IES |= (BIT4+BIT5);  //P1.4 P1.5 下降沿触发中断
	P1IFG &= ~(BIT4+BIT5); //P1.4 P1.5 中断标志清零
	P1IE  |= (BIT4+BIT5);  //P1.4 P1.5 中断使能
}

//P1口中断，P1.4 P1.5 下降沿触发中断
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
	if((P1IN&0x30)!=0x30)  //判断P1.4 P1.5 输入为零
	{
		int i;
		for(i=0;i<2;i++){
		myDelay();}        //延时去抖动20ms
		if((P1IN&0x30)!=0x30)
		{
			scankey();
		}
	}
	P1IFG=0x00;            //清除中断标志
}

/******************************************
函数名：void scankey(void)
功能：对行列式键盘的按键进行判断
******************************************/
void scankey(void)
{
	unsigned char key;
	P1OUT |= (BIT2+BIT4+BIT5); //P1.2 P1.4 P1.5 置高，扫描第一行
	P1OUT &= ~(BIT3);          //P1.3 置低
	key=(P1IN&0x3C);           //P1.2 P1.3 P1.4 P1.5 输入寄存器
	switch(key)
	{
	case 0x24:                 //P1.4 输入为低，即P1.3行的P1.4列按键按下，得到按键B2按下
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=1;
		BUTTON_S3=0;
		BUTTON_S4=0;
		break;
	case 0x14:                 //P1.5 输入为低，即P1.3行的P1.5列按键按下，得到按键B4按下
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=0;
		BUTTON_S3=0;
		BUTTON_S4=1;
		break;
	}
	P1OUT |= (BIT3+BIT4+BIT5); //P1.3 P1.4 P1.5 置高，扫描第二行
	P1OUT &= ~(BIT2);          //P1.2 置低
	key=(P1IN&0x3C);           //P1.2 P1.3 P1.4 P1.5 输入寄存器
	switch(key)
	{
	case 0x28:                 //P1.4 输入为低，即P1.2行的P1.4列按键按下，得到按键B1按下
		buttonsPressed=1;
		BUTTON_S1=1;
		BUTTON_S2=0;
		BUTTON_S3=0;
		BUTTON_S4=0;
		break;
	case 0x18:                 //P1.5 输入为低，即P1.2行的P1.5列按键按下，得到按键B3按下
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=0;
		BUTTON_S3=1;
		BUTTON_S4=0;
		break;
	}
	P1OUT=0xf0;                //键盘复位设置为等待下次按键
}








































