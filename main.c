#include <msp430f5529.h>
#include "key.h"
#include "TFT_Screen.h"
#include "delay.h"
#include "heartrate.h"
#include "accel.h"
#include "spo2h.h"
#include "bloodpreasure.h"
#include "bt.h"
#include "HAL_PMM.h"
#include "HAL.h"
#include <string.h>

// USB API #includes
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "USB_app/usbConstructs.h"
/*
 * main.c
 */
//extern unsigned int startflag,endflag;

void select_chanle(char chanle);
extern void transmit_menu(void);
void chanle_menu(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// �رտ��Ź�

    __enable_interrupt();       //ʹ��ȫ���ж�

    init_key();                 //��ʼ�������Ĺܽ�����

    init_LCD_hardware();        //��ʼ��LCDҺ����ʾ���Ĺܽ�����

    SetVCore(3);                //��VCoreΪ���,���Vcore��ѹ����߼��������㱶Ƶ����ú���λ��HAL_PMM.H��

    initClocks(20000000);       //��ʼ��ʱ��20MHz

    init_SPI();                 //��ʼ��SPI

    ILI9325_CMO24_Initial();
    SPILCD_SetWindow(0,480-1,0,320-1);
    SPILCD_Clear(WHITE);

	P6DIR |= BIT3+BIT4;
    Show_splash();

    int ai;
	for(ai=0;ai<200;ai++)
	{
		myDelay();
	}

    SPILCD_Clear(WHITE);
    Show_MenuTheme();
    Show_MenuMode(1);
    Show_Select();



    Init_UART2();
    UCA0IE |= UCRXIE;

    int as1=1;

    buttonsPressed=0;

    USB_setup(TRUE,TRUE);

    while (!buttonsPressed)
    {
    	buttonsPressed = 0;

    	for(ai=0;ai<30;ai++)
    	{
    		myDelay();
    	}

    	if (buttonsPressed & BUTTON_S3)
        {
        	Show_ButtonS3();
        	if(as1==4){
        		as1=1;
        		Show_MenuMode(as1);
        	}else{
        		if(as1==1){
        			as1=2;
        		    Show_MenuMode(as1);
        		}else{
        			if(as1==2){
        				as1=3;
        		        Show_MenuMode(as1);
        			}else{
        				if(as1==3){
        					as1=4;
        		            Show_MenuMode(as1);
        				}
        			}
        		}
        	}
        	buttonsPressed=0;
        	Show_Select();
        }

        if (buttonsPressed & BUTTON_S2)
        {
        	Show_ButtonS2();
        	if(as1==2){
        		as1=1;
        		Show_MenuMode(as1);
        	}else{
        		if(as1==3){
        			as1=2;
        		    Show_MenuMode(as1);
        		}else{
        			if(as1==4){
        				as1=3;
        		        Show_MenuMode(as1);
        			}else{
        				if(as1==1){
        					as1=4;
        					Show_MenuMode(as1);
        				}
        			}
        		}
        	}
        	buttonsPressed=0;
        	Show_Select();
        }

        if (buttonsPressed & BUTTON_S1)
        {
        	Init_UART2();
        	Show_ButtonS1();
        	if(as1==1){
        	    menu_select=1;
        	}
        	if(as1==2){
        	    menu_select=2;
        	}
        	if(as1==3){
        	    menu_select=3;
        	}
        	if(as1==4){
        	    menu_select=4;
        	}

        	chanle_menu();

        	as1=1;
        	buttonsPressed=0;
        	SPILCD_Clear(WHITE);

            Show_MenuTheme();
            Show_MenuMode(1);
            Show_Select();
        }

        if (buttonsPressed & BUTTON_S4){
        	Show_ButtonS4();
        	Show_MenuMode(as1);
        	buttonsPressed=0;
        	Show_Select();
        }
    }
}

void chanle_menu(void){
    SPILCD_Clear(WHITE);
    Show_MenuTheme();
    Show_ChanleMode(1);
    Show_Select();

    int as1=1;

    buttonsPressed=0;

    while (!(buttonsPressed & BUTTON_S4))
    {
        buttonsPressed = 0;
        int ai;
        for(ai=0;ai<30;ai++)
        {
            myDelay();
        }
        if (buttonsPressed & BUTTON_S3)
        {
            Show_ButtonS3();
            if(as1==4){
                as1=1;
                Show_ChanleMode(as1);
            }else{
                if(as1==1){
                    as1=2;
                    Show_ChanleMode(as1);
                }else{
                    if(as1==2){
                        as1=3;
                        Show_ChanleMode(as1);
                    }else{
                        if(as1==3){
                            as1=4;
                            Show_ChanleMode(as1);
                        }
                    }
                }
            }
            buttonsPressed=0;
            Show_Select();
        }

        if (buttonsPressed & BUTTON_S2)
        {
            Show_ButtonS2();
            if(as1==2){
                as1=1;
                Show_ChanleMode(as1);
            }else{
                if(as1==1){
                    as1=4;
                    Show_ChanleMode(as1);
                }else{
                    if(as1==4){
                        as1=3;
                        Show_ChanleMode(as1);
                    }else{
                        if(as1==3){
                            as1=2;
                            Show_ChanleMode(as1);
                        }
                    }
                }
            }
            buttonsPressed=0;
            Show_Select();
        }

        if (buttonsPressed & BUTTON_S1)
        {
            Show_ButtonS1();
            if(as1==1){
            	select_chanle(1);
            }
            if(as1==2){
            	select_chanle(2);
            }
            if(as1==3){
            	select_chanle(3);
            }
            if(as1==4){
            	select_chanle(4);
            }
            if(menu_select==3) {
            	NONUSE_LCD();
            	int_dac();
            	on_red();
            	USE_LCD();
            }
            transmit_menu();

            as1=1;
            buttonsPressed=0;
            BUTTON_S4=0;
            SPILCD_Clear(WHITE);

            Show_MenuTheme();
            Show_ChanleMode(1);
            Show_Select();
        }
    }
    if(buttonsPressed & BUTTON_S4){
    	Show_ButtonS4();

    }
}

void transmit_menu(void){
    SPILCD_Clear(WHITE);
    Show_MenuTheme();
    Show_TranMenuMode(1);
    Show_Select();

    int as1=1;

    int tran_mode;

    buttonsPressed=0;

    while (!(buttonsPressed & BUTTON_S4))
    {
        buttonsPressed = 0;
        int ai;
        for(ai=0;ai<30;ai++)
        {
            myDelay();
        }
        if (buttonsPressed & BUTTON_S3)
        {
            Show_ButtonS3();
            if(as1==4){
                as1=1;
                Show_TranMenuMode(as1);
            }else{
                if(as1==1){
                    as1=2;
                    Show_TranMenuMode(as1);
                }else{
                    if(as1==2){
                        as1=3;
                        Show_TranMenuMode(as1);
                    }else{
                        if(as1==3){
                            as1=4;
                            Show_TranMenuMode(as1);
                        }
                    }
                }
            }
            buttonsPressed=0;
            Show_Select();
        }

        if (buttonsPressed & BUTTON_S2)
        {
            Show_ButtonS2();
            if(as1==2){
                as1=1;
                Show_TranMenuMode(as1);
            }else{
                if(as1==1){
                    as1=4;
                    Show_TranMenuMode(as1);
                }else{
                    if(as1==4){
                        as1=3;
                        Show_TranMenuMode(as1);
                    }else{
                        if(as1==3){
                            as1=2;
                            Show_TranMenuMode(as1);
                        }
                    }
                }
            }
            buttonsPressed=0;
            Show_Select();
        }

        if (buttonsPressed & BUTTON_S1)
        {
            Show_ButtonS1();
            if(as1==1){

                tran_mode=1;
                tran_mode_SP=1;
            }
            if(as1==2){

                tran_mode=2;
                tran_mode_SP=2;
            }
            if(as1==3){
                tran_mode=3;
                tran_mode_SP=3;
            }
            if(as1==4){
                tran_mode=4;
                tran_mode_SP=4;
            }

            if(menu_select==1){
            	UCA0IE &= ~UCRXIE;
                HR(tran_mode);
                menu_select=1;
            }
            if(menu_select==2){
                BP(tran_mode);
                menu_select=2;
            }
            if(menu_select==3){
                 SPO2H(tran_mode);
                 menu_select=3;
            }
            if(menu_select==4){
                ACCELMENU(tran_mode);
                menu_select=4;
            }

            as1=1;
            tran_mode=1;
            buttonsPressed=0;
            BUTTON_S4=0;
            SPILCD_Clear(WHITE);

            Show_MenuTheme();
            Show_TranMenuMode(1);
            Show_Select();
        }
    }
    if(buttonsPressed & BUTTON_S4){
        Show_ButtonS4();
    }
}

void select_chanle(char chanle){
	switch (chanle){
	case 0x01:
		P6OUT |= BIT3;
		P6OUT &=~BIT4;
		break;
	case 0x02:
		P6OUT |= BIT4;
		P6OUT &=~BIT3;
		break;
	case 0x03:
		P6OUT |=0x18;
		break;
	case 0x04:
		P6OUT &=~0x18;
		break;

	default:break;

	}


}
