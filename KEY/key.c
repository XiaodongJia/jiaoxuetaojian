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
��������void init_key(void)
���ܣ�������ʽ�����õ��ĹܽŽ��г�ʼ������
******************************************/
void init_key(void)
{
	P1DIR |= (BIT3+BIT2);  //P1.2 P1.3 ����Ϊ���
	P1OUT &= ~(BIT3+BIT2); //P1.2 P1.3 ���λ��
	P1DIR &= ~(BIT4+BIT5); //P1.4 P1.5 ����Ϊ����
	P1REN |= (BIT4+BIT5);  //P1.4 P1.5 �Ĵ���ʹ��
	P1OUT |= (BIT4+BIT5);  //P1.4 P1.5 �ø�
	P1IES |= (BIT4+BIT5);  //P1.4 P1.5 �½��ش����ж�
	P1IFG &= ~(BIT4+BIT5); //P1.4 P1.5 �жϱ�־����
	P1IE  |= (BIT4+BIT5);  //P1.4 P1.5 �ж�ʹ��
}

//P1���жϣ�P1.4 P1.5 �½��ش����ж�
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
	if((P1IN&0x30)!=0x30)  //�ж�P1.4 P1.5 ����Ϊ��
	{
		int i;
		for(i=0;i<2;i++){
		myDelay();}        //��ʱȥ����20ms
		if((P1IN&0x30)!=0x30)
		{
			scankey();
		}
	}
	P1IFG=0x00;            //����жϱ�־
}

/******************************************
��������void scankey(void)
���ܣ�������ʽ���̵İ��������ж�
******************************************/
void scankey(void)
{
	unsigned char key;
	P1OUT |= (BIT2+BIT4+BIT5); //P1.2 P1.4 P1.5 �øߣ�ɨ���һ��
	P1OUT &= ~(BIT3);          //P1.3 �õ�
	key=(P1IN&0x3C);           //P1.2 P1.3 P1.4 P1.5 ����Ĵ���
	switch(key)
	{
	case 0x24:                 //P1.4 ����Ϊ�ͣ���P1.3�е�P1.4�а������£��õ�����B2����
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=1;
		BUTTON_S3=0;
		BUTTON_S4=0;
		break;
	case 0x14:                 //P1.5 ����Ϊ�ͣ���P1.3�е�P1.5�а������£��õ�����B4����
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=0;
		BUTTON_S3=0;
		BUTTON_S4=1;
		break;
	}
	P1OUT |= (BIT3+BIT4+BIT5); //P1.3 P1.4 P1.5 �øߣ�ɨ��ڶ���
	P1OUT &= ~(BIT2);          //P1.2 �õ�
	key=(P1IN&0x3C);           //P1.2 P1.3 P1.4 P1.5 ����Ĵ���
	switch(key)
	{
	case 0x28:                 //P1.4 ����Ϊ�ͣ���P1.2�е�P1.4�а������£��õ�����B1����
		buttonsPressed=1;
		BUTTON_S1=1;
		BUTTON_S2=0;
		BUTTON_S3=0;
		BUTTON_S4=0;
		break;
	case 0x18:                 //P1.5 ����Ϊ�ͣ���P1.2�е�P1.5�а������£��õ�����B3����
		buttonsPressed=1;
		BUTTON_S1=0;
		BUTTON_S2=0;
		BUTTON_S3=1;
		BUTTON_S4=0;
		break;
	}
	P1OUT=0xf0;                //���̸�λ����Ϊ�ȴ��´ΰ���
}








































