/*
 * bt.c
 *
 *  Created on: 2014年9月14日
 *      Author: lenovo
 */
#include "msp430.h"
#include "heartrate.h"
#include "accel.h"
#include "spo2h.h"
#include "bloodpreasure.h"

char degc=0;
unsigned char RxBuf[20];
unsigned char RxBuf1[20];
unsigned int RxPoint;
unsigned int RxPoint1;
unsigned int startflag;
unsigned int endflag;
unsigned int accel_start;
unsigned int accel_end;
unsigned int Next;

void Init_UART1(void)
{
	                    // **Initialize USCI state machine**/
	P3SEL |= BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
    UCA0BR0 = 3;                           // 32kHz/9600=3.41 (see User's Guide)
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = 0x4a;//UCBRS_0+UCBRF_13+UCOS16;               // Modulation UCBRSx=3, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	RxPoint=0;
	startflag=0;
	endflag=0;
}

void Init_UART2(void)
{
	                    // **Initialize USCI state machine**/
	P3SEL |= BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;                     // CLK = ACLK
    UCA0BR0 = 0x23;//3;                           // 32kHz/9600=3.41 (see User's Guide)
	UCA0BR1 = 0x08;//0x00;                           //
	UCA0MCTL = 0x00;//UCBRS_0+UCBRF_13+UCOS16;               // Modulation UCBRSx=3, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	RxPoint=0;
	startflag=0;
	endflag=0;
	accel_start=0;
	accel_end=0;
	RxPoint1=0;
}

void Init_UART3(void)
{
	UCA0CTL1 |= UCSWRST;                     // **Initialize USCI state machine**
}

void btSend_char(char * buf, unsigned int len)
{
    unsigned int i = 0;

    // Write each byte in buf to USCI TX buffer, which sends it out
    while (i < len)
    {
        UCA0TXBUF = *(buf+((i++)));

        // Wait until each bit has been clocked out...
         while(!(UCTXIFG==(UCTXIFG & UCA0IFG))&&((UCA0STAT & UCBUSY)==UCBUSY));
    }
}

/*****************************************************************************
USART1接收中断函数
******************************************************************************/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	  switch(__even_in_range(UCA0IV,4))
	  {
	  case 0:break;                             // Vector 0 - no interrupt
	  case 2:                                   // Vector 2 - RXIFG

		  if(startflag==0){
			  if(UCA0RXBUF==0x33)
			  {
				  startflag=1;
				  RxPoint=0;
			  }
		  }

		  if(accel_start==0){
			  if(UCA0RXBUF==0x11)
			  {
				  accel_start=1;
			      RxPoint1=0;
		      }
		  }
		  if((startflag==1)&&(UCA0RXBUF==0x44 || RxPoint==9)) {
			  endflag=1;
		  }

		  if((accel_start==1)&&(UCA0RXBUF==0x66 || RxPoint1==7)) {
			  accel_end=1;
		  }

		  RxBuf[RxPoint]=UCA0RXBUF;
		  RxPoint++;
		  RxBuf1[RxPoint1]=UCA0RXBUF;
		  RxPoint1++;

		  if(accel_end==1)
		  {
//			  UCA0IE &= ~UCRXIE;
			  accel_start=0;
			  RxPoint1=0;
			  accel_start=0;
		  }

		  if(endflag==1)
		  {
			  RxPoint=0;
			  startflag=0;
			  endflag=0;

			    unsigned int bt_transmit;
			    unsigned int bt_select;

		    	if(RxBuf[1]==0x20)
		    	{
		    		if(RxBuf[2]==0x00)
		    		{
		    			bt_transmit=1;
		    		}else if(RxBuf[2]==0x01)
		    		{
		    			bt_transmit=3;
		    		}else if(RxBuf[2]==0x02)
		    		{
		    			bt_transmit=2;
		    		}else if(RxBuf[2]==0x03)
		    		{
		    			bt_transmit=4;
		    		}
		    	}

		    	if(RxBuf[3]==0x21)
		    	{
		    		if(RxBuf[4]==0x00)
		    		{
		    			select_chanle(1);
		    		}else if(RxBuf[4]==0x01)
		    		{
		    			select_chanle(2);
		    		}else if(RxBuf[4]==0x02)
		    		{
		    			select_chanle(3);
		    		}else if(RxBuf[4]==0x03)
		    		{
		    			select_chanle(4);
		    		}
		    	}

		    	if(RxBuf[5]==0x22)
		    	{
		    		if(RxBuf[6]==0x00)
		    		{
		    			menu_select=0;
		    			HR(bt_transmit);
//		    			transmit_menu();
//		    			chanle_menu();
		    		}else if(RxBuf[6]==0x01)
		    		{
		    			menu_select=1;
		    			BP(bt_transmit);
//		    			transmit_menu();
//		    			chanle_menu();
		    		}else if(RxBuf[6]==0x02)
		    		{
		    			menu_select=2;
		            	NONUSE_LCD();
		            	int_dac();
		            	on_red();
		            	USE_LCD();
		            	SPO2H(bt_transmit);
//		            	transmit_menu();
//		            	chanle_menu();
		    		}else if(RxBuf[6]==0x03)
		    		{
		    			menu_select=3;
		    		}
		    	}

		    	if((menu_select==3)&&(RxBuf[7]==0x23))
		    	{
		    		if(RxBuf[8]==0x00)
		    		{
		    			accel_select=0;
		    			ACCEL(bt_transmit);
//		    			ACCELNAME(bt_transmit);
		    		}else if(RxBuf[8]==0x01)
		    		{
		    			accel_select=0;
		    			WALK(bt_transmit);
//		    			ACCELNAME(bt_transmit);
		    		}else if(RxBuf[8]==0x02)
		    		{
		    			accel_select=1;
		    			ACCEL2(bt_transmit);
//		    			ACCELNAME(bt_transmit);
		    		}else if(RxBuf[8]==0x03)
		    		{
		    			accel_select=1;
		    			WALK2(bt_transmit);
//		    			ACCELNAME(bt_transmit);
		    		}
		    	}
		  }

	    break;
	  case 4:
//		  while (!(UCA0IFG&UCTXIFG));
		  UCA0TXBUF = degc;                  // TX -> RXed character
		  UCA0IE &= ~UCTXIE;
//		  while (!(UCA0IFG&UCTXIFG));
		  break;                             // Vector 4 - TXIFG
	  default: break;
}
}
