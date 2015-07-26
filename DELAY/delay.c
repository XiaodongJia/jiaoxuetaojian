/*
 * delay.c
 *
 *  Created on: 2014��9��12��
 *      Author: lenovo
 */
#include <msp430f5529.h>
#include "delay.h"

/******************************************
��������void myDelay(void)
���ܣ�����TA��ʱ������
******************************************/
void myDelay(void)
{
	TA1CTL &=~ TAIFG;                 //���TA�жϱ�־λ
	TA1CCR0 =328;                     //100hz��10ms��
	TA1CTL = TASSEL_1 + MC_1 + TACLR; // ACLK, ������ģʽ, ��ʱ�����㣨��λTAR��
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
