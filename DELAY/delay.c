/*
 * delay.c
 *
 *  Created on: 2014年9月12日
 *      Author: lenovo
 */
#include <msp430f5529.h>
#include "delay.h"

/******************************************
函数名：void myDelay(void)
功能：利用TA定时器计数
******************************************/
void myDelay(void)
{
	TA1CTL &=~ TAIFG;                 //清除TA中断标志位
	TA1CCR0 =328;                     //100hz（10ms）
	TA1CTL = TASSEL_1 + MC_1 + TACLR; // ACLK, 单调增模式, 定时器清零（复位TAR）
	while((TA1CTL & TAIFG)==0);
	TA1CTL &=~ TAIFG;
}

void myDelay1(void)
{
    TA1CTL &=~ TAIFG;
    TA1CCR0 =1; //115hz//5khz
    TA1CTL = TASSEL_1 + MC_1 + TACLR; // SMCLK, contmode, clear TAR
    while((TA1CTL & TAIFG)==0);
    TA1CTL &=~ TAIFG;
}
