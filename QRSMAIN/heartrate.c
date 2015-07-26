// Basic MSP430 and driverLib #includes
#include "msp430.h"
#include <string.h>
#include "driverlib.h"
#include "HAL_PMM.h"

// USB API #includes
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "USB_app/usbConstructs.h"

// Application #includes
#include "BCUart.h"           // Include the backchannel UART "library"
#include "hal.h"              // Modify hal.h to select your hardware
#include "TFT_Screen.h"
#include "ADC12.h"
//#include "QRS.h"
#include "heartrate.h"
#include "key.h"
#include "bt.h"
#include "delay.h"
#include "stdlib.h"

//volatile int tran_mode;
volatile int menu_select;
volatile int index11;      //原始采样数据下标
volatile int d11 = 0;
volatile int point_color;
volatile  int results[1500];	// 存放原始采样数据
             //存放转换之后的LCD显示数据
volatile int results1[1517];
volatile int HRate;


unsigned int    HeartRate;                 //心率
unsigned char   str1[5];

#define Num_of_ECG 480
#define pinghua 5
unsigned int ECG_Faddr;               //心电处理程序用的数据首地址
volatile int index_ECG;

volatile unsigned int CRCresult;
volatile unsigned char CRCtmp,CRCindex;
volatile const unsigned int CRC_TA[256]={
        0x0000,0x1189,0x2312,0x329B,0x4624,0x57AD,0x6536,0x74BF,
        0x8C48,0x9DC1,0xAF5A,0xBED3,0xCA6C,0xDBE5,0xE97E,0xF8F7,
        0x1081,0x0108,0x3393,0x221A,0x56A5,0x472C,0x75B7,0x643E,
        0x9CC9,0x8D40,0xBFDB,0xAE52,0xDAED,0xCB64,0xF9FF,0xE876,
        0x2102,0x308B,0x0210,0x1399,0x6726,0x76AF,0x4434,0x55BD,
        0xAD4A,0xBCC3,0x8E58,0x9FD1,0xEB6E,0xFAE7,0xC87C,0xD9F5,
        0x3183,0x200A,0x1291,0x0318,0x77A7,0x662E,0x54B5,0x453C,
        0xBDCB,0xAC42,0x9ED9,0x8F50,0xFBEF,0xEA66,0xD8FD,0xC974,
        0x4204,0x538D,0x6116,0x709F,0x0420,0x15A9,0x2732,0x36BB,
        0xCE4C,0xDFC5,0xED5E,0xFCD7,0x8868,0x99E1,0xAB7A,0xBAF3,
        0x5285,0x430C,0x7197,0x601E,0x14A1,0x0528,0x37B3,0x263A,
        0xDECD,0xCF44,0xFDDF,0xEC56,0x98E9,0x8960,0xBBFB,0xAA72,
        0x6306,0x728F,0x4014,0x519D,0x2522,0x34AB,0x0630,0x17B9,
        0xEF4E,0xFEC7,0xCC5C,0xDDD5,0xA96A,0xB8E3,0x8A78,0x9BF1,
        0x7387,0x620E,0x5095,0x411C,0x35A3,0x242A,0x16B1,0x0738,
        0xFFCF,0xEE46,0xDCDD,0xCD54,0xB9EB,0xA862,0x9AF9,0x8B70,
        0x8408,0x9581,0xA71A,0xB693,0xC22C,0xD3A5,0xE13E,0xF0B7,
        0x0840,0x19C9,0x2B52,0x3ADB,0x4E64,0x5FED,0x6D76,0x7CFF,
        0x9489,0x8500,0xB79B,0xA612,0xD2AD,0xC324,0xF1BF,0xE036,
        0x18C1,0x0948,0x3BD3,0x2A5A,0x5EE5,0x4F6C,0x7DF7,0x6C7E,
        0xA50A,0xB483,0x8618,0x9791,0xE32E,0xF2A7,0xC03C,0xD1B5,
        0x2942,0x38CB,0x0A50,0x1BD9,0x6F66,0x7EEF,0x4C74,0x5DFD,
        0xB58B,0xA402,0x9699,0x8710,0xF3AF,0xE226,0xD0BD,0xC134,
        0x39C3,0x284A,0x1AD1,0x0B58,0x7FE7,0x6E6E,0x5CF5,0x4D7C,
        0xC60C,0xD785,0xE51E,0xF497,0x8028,0x91A1,0xA33A,0xB2B3,
        0x4A44,0x5BCD,0x6956,0x78DF,0x0C60,0x1DE9,0x2F72,0x3EFB,
        0xD68D,0xC704,0xF59F,0xE416,0x90A9,0x8120,0xB3BB,0xA232,
        0x5AC5,0x4B4C,0x79D7,0x685E,0x1CE1,0x0D68,0x3FF3,0x2E7A,
        0xE70E,0xF687,0xC41C,0xD595,0xA12A,0xB0A3,0x8238,0x93B1,
        0x6B46,0x7ACF,0x4854,0x59DD,0x2D62,0x3CEB,0x0E70,0x1FF9,
        0xF78F,0xE606,0xD49D,0xC514,0xB1AB,0xA022,0x92B9,0x8330,
        0x7BC7,0x6A4E,0x58D5,0x495C,0x3DE3,0x2C6A,0x1EF1,0x0F78};



static const unsigned char HRhan1[2][128]={
  		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x0F,0x00,0x80,0xFF,0x1F,0x00,0x80,0x03,0x38,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x08,0x01,0x30,0x3C,0x1C,0x01,0x30,0x1C,0x1C,0x00,0x30,0x0C,0x18,0x00,0x30,0x0E,0x38,0x00,0x30,0x06,0x30,0x00,0x30,0x06,0x60,0x00,0x30,0x02,0x40,0x00,0x30,0x02,0x80,0x00,0x30,0x02,0x00,0x00,0x30,0x00,
  		0x00,0x00,0x30,0x00,0x00,0x60,0x38,0x00,0x00,0x60,0x20,0x00,0x00,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xC0,0x01,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"心",4*/

  		{0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0xFC,0xFF,0xFF,0x3F,0x18,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x88,0x01,0x08,0x20,0x08,0x39,0x18,0x30,0xFC,0x3F,0x0C,0x60,0x18,0x08,0x03,0xC0,0x31,0x84,0x00,0x00,0x02,0x43,0x00,0x00,0x80,0x01,0x00,0x00,0xC4,0x30,0x00,0x00,0xE3,0x3F,0x01,
  		0x80,0x31,0x90,0x03,0xE0,0x38,0x0C,0x07,0x40,0x10,0x06,0x0C,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0xF8,0xFF,0xFF,0x1F,0x30,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"率",5*/
  };
/* You have a choice between implementing this as a CDC USB device, or a HID-
 * Datapipe device.  With CDC, the USB device presents a COM port on the host;
 * you interact with it with a terminal application, like Hyperterminal or
 * Docklight.  With HID-Datapipe, you interact with it using the Java HID Demo
 * App available within the MSP430 USB Developers Package.
 *
 * By default, this app uses CDC.  The HID calls are included, but commented
 * out.
 *
 * See the F5529 LaunchPad User's Guide for simple instructions to convert
 * this demo to HID-Datapipe.  For deeper information on CDC and HID-Datapipe,
 * see the USB API Programmer's Guide in the USB Developers Package.
 */

// Global variables
/*WORD rxByteCount;                        // Momentarily stores the number of bytes received
BYTE buf_bcuartToUsb[BC_RXBUF_SIZE];     // Same size as the UART's rcv buffer
BYTE buf_usbToBcuart[128];               // This can be any size*/
#define num_of_data  480;
/*****************BlueTooth****************/
void  ADC12_IO_Init();
void HRCaculate();
char ffffff=0;

void HR(int transmit_mode)
{
    //unsigned char results2[992];

	USE_LCD();

    d11=0;

    Init_UART2();
    UCA0IE |= UCRXIE;

    buttonsPressed=0;
    SPILCD_Clear(WHITE);

    Show_ALLTHEME();
    DRAW_han(420,48,HRhan1[0]);
    DRAW_han(380,48,HRhan1[1]);

//    PMM_setVCore(PMM_BASE, PMM_CORE_LEVEL_2);
      initClocks(20000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz


/****************初始化*********************/
    ADC12_IO_Init();       //初始化ADC12

    results1[0]=0x53;
    results1[1]=0x54;
    results1[2]=0x41;
    results1[3]=0x52;
    results1[4]=0x54;
    results1[5]=0x48;
    results1[6]=0x52;
    //7/8和504/505存心率
    results1[489]='A';
    results1[490]='A';
    results1[491]='A';
    results1[492]='A';
    results1[493]='A';
    results1[494]='E';
    results1[495]='N';
    results1[496]='D';

    results1[497]='S';
    results1[498]='T';
    results1[499]='A';
    results1[500]='R';
    results1[501]='T';
    results1[502]='H';
    results1[503]='R';

    results1[986]='A';
    results1[987]='A';
    results1[988]='A';
    results1[989]='A';
    results1[990]='A';
    results1[991]='E';
    results1[992]='N';
    results1[993]='D';


    //_NOP();

    index11=9;
    d11=0;
   	bcUartInit();
//   	 bcUartSend_char(results1, 496);
//   	bcUartSend(results1, 496);

   ADC12CTL0 |= ADC12SC;             // 开始转化
   ADC12CTL0 &=~ ADC12SC;
//   USB_setup(TRUE,TRUE);
//   if(ffffff==0){USB_setup(TRUE,TRUE);
//   ffffff=1;
//   }
   _EINT();
   while(!(buttonsPressed & BUTTON_S4))
   	    {
        __bis_SR_register(LPM0_bits + GIE);  //低功耗，使能全局中断

        while((!BUTTON_S4)&(index_ECG == 0xEE));
        unsigned int trans_start,trans_end;
        if(index_ECG == 0x22)   //传480-960 处理0-480
        {
          index_ECG=0xEE;
          ECG_Faddr = 9;
          trans_start=497;
          trans_end=994;
        }
        else if(index_ECG == 0x33)  //写0-480 处理480-960
        {
          index_ECG=0xEE;
          ECG_Faddr = 506;
          trans_start=0;
          trans_end=497;
        }
          unsigned int temp1,temp10,temp100,i;
          HRCaculate();
          temp1=HRate%10;             //计算心率个位
          temp10=(HRate%100-temp1)/10;    //计算心率十位
          temp100=(HRate-temp10*10-temp1)/100;    //计算心率百位

          _DINT();
          SPILCD_Clear_Lim(280,300,20,53,WHITE);         //如果上一次是三位数的话 先清除高位
          if(temp100>0)    DRAW_NUM(280,20,temp100,BLUE);  //显示心率百位
          DRAW_NUM(240,20,temp10,BLUE);         //显示心率十位
          DRAW_NUM(200,20,temp1,BLUE);       //显示心率个位
//          HRate=130;
          if(HRate>=127){
          results1[ECG_Faddr-2]=0x7F;
          results1[ECG_Faddr-1]=(char)(HRate-127);}
          else{
        	  results1[ECG_Faddr-2]=(char)(HRate);
        	  results1[ECG_Faddr-1]=0;
          }
          _EINT();


//          long temp_final;
//          unsigned int a;
//          char b[5];
//          CRCresult=0;
//          for(i=trans_start+8;i<trans_start+488;i++)      //转换成字符  504=496+8   984=496+488
//          {
//              results1[i]=results1[i]>>4;
//          CRCtmp = CRCresult%256;
//          CRCindex = CRCtmp^results1[i];
//          CRCtmp = CRCresult/256;   //除以256
//          CRCresult = CRCtmp;
//          CRCresult = CRCresult^CRC_TA[CRCindex];
//          }
//         a=CRCresult;
//         ltoa(a,b);
//         results1[trans_start+488]=b[0];
//         results1[trans_start+489]=b[1];
//         results1[trans_start+490]=b[2];
//         results1[trans_start+491]=b[3];
//         results1[trans_start+492]=b[4];

/*             temp_final=(long)(temp1+temp10*10+temp100*100);
             ltoa(temp_final,b);
             if(temp_final>=0&&temp_final<=9)
             {
             c[0]='0';
             c[1]='0';
             c[2]=b[0];
             }
             if(temp_final>=10&&temp_final<=99)
             {
             c[0]='0';
             c[1]=b[0];
             c[2]=b[1];
             }
             if(temp_final>=100&&temp_final<=999)
             {
             c[0]=b[0];
             c[1]=b[1];
             c[2]=b[2];
             }
*/

//                    unsigned int a;
//                    char b[5];
//                    CRCresult=0;
         for(i=trans_start+9;i<trans_start+489;i++)      //转换成字符
         {
             results1[i]=results1[i]>>5;
//                       CRCtmp = CRCresult%256;
//                       CRCindex = CRCtmp^results1[i];
//                       CRCtmp = CRCresult/256;   //除以256
//                       CRCresult = CRCtmp;
//                       CRCresult = CRCresult^CRC_TA[CRCindex];
         }
//                  a=CRCresult;
//                  ltoa(a,b);
//                  results1[trans_start+488]=b[0];
//                  results1[trans_start+489]=b[1];
//                  results1[trans_start+490]=b[2];
//                  results1[trans_start+491]=b[3];
//                  results1[trans_start+492]=b[4];

   	     	 if(transmit_mode==1){
//   	     		bcUartInit();
   	     		bcUartSend(results1+trans_start, 497);
   	     	    //hidSendDataInBackground((results1+trans_start),992, HID0_INTFNUM,10000);
   	     	 }


             if(transmit_mode==2){
            	 UCA0IE &=~ UCRXIE;
            	 hidSendDataInBackground((results1+trans_start),994, HID0_INTFNUM,10000);
            	 UCA0IV|=2;
            	 UCA0IE |= UCRXIE;
             }
   	     	 if(transmit_mode==3){
                 //Init_UART1();
                 int i=0;
 //                 int j;
//                 for(j=0;j<49;j++){
                 for(i=trans_start;i<trans_start+497;i++)      //转换成字符
                     {

//                     degc=results1[i];
//                     UCA0IE |= UCTXIE;
                     UCA0TXBUF = results1[i];
                             // Wait until each bit has been clocked out...
                     while(!(UCTXIFG==(UCTXIFG & UCA0IFG))&&((UCA0STAT & UCBUSY)==UCBUSY));
//                     __delay_cycles(2000000);
                     int a,k;
                     for(a=0;a<10;a++)
                     {
                          for(k=0;k<80;k++);
                     }

//                     if(i==trans_start+num1*20+20)
//                     {
//                    	 UCA0IE |= UCRXIE;
//                     }

                     }
//                                      int a,k;
//                                      for(a=0;a<10;a++)
//                                      {
//                                           for(k=0;k<50;k++);
//                                      }
//                 }
//                 for(i=trans_start+486;i<trans_start+496;i++)      //转换成字符
//                     {
//
//                     UCA0TXBUF = results1[i];
//                             // Wait until each bit has been clocked out...
//                     while(!(UCTXIFG==(UCTXIFG & UCA0IFG))&&((UCA0STAT & UCBUSY)==UCBUSY));
//
//                     }
//                 UCA0IE |= UCRXIE;

//                 int a,k;
//                 for(a=0;a<200;a++)
//                 {
//                      for(k=0;k<50;k++);
//                 }

//                 __delay_cycles(2000000);

             }
//             ADC12IE = ADC12IE12;
//             for(i=trans_start+480;i<trans_start+496;i++)      //转换成字符
//                 {
//
//                 degc=results1[i];
//                 UCA0IE |= UCTXIE;
//                 int a,k;
//                 for(a=0;a<20;a++)
//                 {
//                      for(k=0;k<210;k++);
//                 }
//
//
//                 }

//   	     	 }
   	     	//Init_UART1();

   	    }
   BUTTON_S4=0;
   buttonsPressed=0;
   ADC12CTL0 &= ~ADC12SC;
   ADC12IE &= ~ADC12IE12;
   ADC12CTL0 &= ~ADC12ENC;

   SPILCD_Clear(WHITE);
}

void HRCaculate(void)
{
  //HRate=0;
  unsigned int temHR=0;    //所有rr间期的和
  unsigned int count=0;
  unsigned int count_spa=0;
  unsigned int count_new=0;  //rr 间期的个数
  //unsigned int len=(int)(50/4);  //12.5
  unsigned int space=(int)(150/4);    //37.5
 // unsigned int width=(int)(70/4);   //17.5
  unsigned int Rindex[15];
  unsigned int Rindex_spa[14];

  int ECGmax = 0;
  int ECGmin = 0;
  int ECGrange = 0;

  unsigned int i;

  ECGmax = results1[+1];
  ECGmin = results1[ECG_Faddr+1];
  for(i=0 ; i<Num_of_ECG - 2;i++)
  {
      if(ECGmax < results1[ECG_Faddr+i])
       {ECGmax = results1[ECG_Faddr+i];}
      if(ECGmin > results1[ECG_Faddr+i])
       {ECGmin = results1[ECG_Faddr+i];}
  }

  ECGrange =  ECGmin + 0.7*(ECGmax - ECGmin);

  for (i=0;i<Num_of_ECG;i++)
  {
      if(results1[ECG_Faddr+i] >= ECGrange)
      {
          results[i]=results1[ECG_Faddr+i];
      }
      else
      {
          results[i] = 0;
      }
  }
  for (i=0;i<Num_of_ECG-1;i++)
  {
      results[i] = results[i+1] - results[i];
      if(results[i]<0){results[i] = -1;}
      if(results[i]>0){results[i] = 1;}
  }

  for (i=0;i<Num_of_ECG-2;i++)
  {
      results[i] = results[i+1] - results[i];
      if(results[i]==-2)
      {
          Rindex[count] = i+1;
          count++;
          if(count>15)break;
          i=i+space;
      }
  }

  if(count==0||count==1)
  {
    HRate=HRate;
  }
  else
  {
  count_spa=count-1;
  for(i=0;i<count_spa;i++)
  {
    Rindex_spa[i]=Rindex[i+1]-Rindex[i];   //极值位置差分
  }

  if(count_spa>2){
  for(i=0;i<count_spa-2;i++)
  {
    if( (abs(Rindex_spa[i+1]-Rindex_spa[i])>10) && (abs(Rindex_spa[i+1]-Rindex_spa[i+2])>10) )
    {
      Rindex_spa[i+1]=0;
    }

    if((Rindex_spa[count_spa-2]!=0)&&(Rindex_spa[1]!=0))
    {
      if(abs(Rindex_spa[0]-Rindex_spa[1])>10)
      {
        Rindex_spa[0]=0;
      }
      if(abs(Rindex_spa[count_spa-1]-Rindex_spa[count_spa-2])>10)
      {
        Rindex_spa[count_spa-1]=0;
      }
    }

    if((Rindex_spa[count_spa-2]!=0)&&(Rindex_spa[1]==0))
    {
      if(abs(Rindex_spa[count_spa-1]-Rindex_spa[count_spa-2])>10)
      {
        Rindex_spa[count_spa-1]=0;
      }
      if(abs(Rindex_spa[0]-Rindex_spa[count_spa-2])>10)
      {
        Rindex_spa[0]=0;
      }
    }

    if((Rindex_spa[count_spa-2]==0)&&(Rindex_spa[1]!=0))
    {
      if(abs(Rindex_spa[count_spa-1]-Rindex_spa[1])>10)
      {
        Rindex_spa[count_spa-1]=0;
      }
      if(abs(Rindex_spa[0]-Rindex_spa[1])>10)
      {
        Rindex_spa[0]=0;
      }
    }

    if((Rindex_spa[count_spa-2]==0)&&(Rindex_spa[1]==0))
    {
      if(abs(Rindex_spa[count_spa-1]-Rindex_spa[0])>10)
      {
        Rindex_spa[count_spa-1]=0;
        Rindex_spa[0]=0;
      }
    }

  }
  }

  for (i=0;i<count_spa;i++)
  {

    if(Rindex_spa[i]!=0)
    {
      temHR=temHR+Rindex_spa[i];
      count_new=count_new+1;
    }
  }
  if(temHR==0)
  {
    HRate = HRate;
  }

  else
  {
    HRate=(int)((60.0*120.0*(float)(count_new)/(float)(temHR)));
  }
  }
}


void convertTwoDigBinToASCII(unsigned int bin, BYTE* str)
 {
	 int cir=0;
     str[0] = '0';

     for(cir=0;cir<4;cir++)
     {
    	 str[3-cir]=(bin%10)+'0';
    	 bin/=10;
     }
     str[4]='\0';
 }

void  ADC12_IO_Init(){
    TB0CCTL0=0x0080; //定时器B输出为翻转模式
        TB0CCR0=136 ;//采样率??
        TB0CTL=TBSSEL_1+MC_1+TBCLR; //ACLK+增模式+CLEAR
    ADC12CTL0 = ADC12ON+ADC12SHT0_15+ADC12REFON;      //采样保持定时器0分频因子为256

    ADC12CTL1= ADC12SSEL1+ADC12CONSEQ_2+ADC12DIV_5+ADC12SHP+ADC12SHS_2;  //内部时钟源ADC12OSC,分频系数4,单通道多次采样
                                                                          //  TB0触发，采样率120
    ADC12CTL1 &= ~(ADC12CSTARTADD3+\
                    ADC12CSTARTADD2+\
                    ADC12CSTARTADD1+\
                    ADC12CSTARTADD0);                        // 转换起始地址
    ADC12CTL1 |= ADC12CSTARTADD3+ADC12CSTARTADD2;
    ADC12CTL2=0x20+ADC12SR+ADC12PDIV;                        //预分频为4,采样率为~50 ksps
    ADC12MCTL0 = ADC12INCH_0;//p6.0为0通道
        ADC12MCTL1 = ADC12INCH_1+ADC12EOS;//p6.1为1通道
    ADC12MCTL12 = ADC12INCH_0;                              //输入通道选择A13
    ADC12IE = 0x00;
    ADC12IE = ADC12IE12;                                    //ADC12IE12中断使能
    P6SEL |= 0x01;                                 //P7.1
    ADC12CTL0 |= ADC12ENC;                      // 使能转换

}

#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not found!
#endif
{
        switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG )) {
        case SYSUNIV_NONE:
                __no_operation();
                break;
        case SYSUNIV_NMIIFG:
                __no_operation();
                break;
        case SYSUNIV_OFIFG:

                UCS_clearFaultFlag(UCS_BASE, UCS_XT2OFFG);
                UCS_clearFaultFlag(UCS_BASE, UCS_DCOFFG);
                SFR_clearInterrupt(SFR_BASE, SFR_OSCILLATOR_FAULT_INTERRUPT);

                break;
        case SYSUNIV_ACCVIFG:
                __no_operation();
                break;
        case SYSUNIV_BUSIFG:
                // If the CPU accesses USB memory while the USB module is
                // suspended, a "bus error" can occur.  This generates an NMI.  If
                // USB is automatically disconnecting in your software, set a
                // breakpoint here and see if execution hits it.  See the
                // Programmer's Guide for more information.
                SYSBERRIV = 0;  // Clear bus error flag
                USB_disable();  // Disable
        }
}
