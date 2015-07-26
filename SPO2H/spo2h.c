/*
 * spo2h.c
 *
 *  Created on: 2014年9月14日
 *      Author: lenovo
 */

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
#include "spo2h.h"
#include "ADC12.h"
#include "bt.h"
#include "key.h"
#include "heartrate.h"
#include "math.h"

unsigned int g = 0;
unsigned char   str1[3];
int i;
#define uint unsigned int
#define uchar unsigned char
#define max 480


 unsigned int r_locate[10];  //ird
 unsigned int r1_locate[10];// red
 unsigned int nr_locate[10];  //ired 负！峰值位置
 unsigned int nr1_locate[10];// red
 unsigned int rt,rb,it,ib;
 unsigned int red_ac = 0;
 unsigned int ired_ac = 0;
 unsigned int red_min = 0;
 unsigned int ired_min = 0;
 float Q = 70.0; //血氧值
 float pre_Q = 70; //上一次计算的血氧值
 float Q1 = 0.0;
 float Q2 = 0.0;
 int sao2 = 0;
 float red_dc = 0.0;
 float ired_dc = 0.0;
 float length_dc = 0.0;
 float length_idc = 0.0;
 volatile int iyz;
 volatile int ryz;
 volatile int num1=0;
 volatile int num2=0;
 volatile int tran_mode_SP;
 volatile int ad_f;


 int c,r;
 #define pinghua 5
 int red_flage = 0;  //red = 1 ;ired =1
 int MSB,LSB;
 int count=0;





 const unsigned char HRhan2[6][128]={
         {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x0F,0x00,0x80,0xFF,0x1F,0x00,0x80,0x03,0x38,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x00,0x01,0x30,0x00,0x08,0x01,0x30,0x3C,0x1C,0x01,0x30,0x1C,0x1C,0x00,0x30,0x0C,0x18,0x00,0x30,0x0E,0x38,0x00,0x30,0x06,0x30,0x00,0x30,0x06,0x60,0x00,0x30,0x02,0x40,0x00,0x30,0x02,0x80,0x00,0x30,0x02,0x00,0x00,0x30,0x00,
         0x00,0x00,0x30,0x00,0x00,0x60,0x38,0x00,0x00,0x60,0x20,0x00,0x00,0xE0,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xC0,0x01,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"心",4*/

         {0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0xFC,0xFF,0xFF,0x3F,0x18,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x88,0x01,0x08,0x20,0x08,0x39,0x18,0x30,0xFC,0x3F,0x0C,0x60,0x18,0x08,0x03,0xC0,0x31,0x84,0x00,0x00,0x02,0x43,0x00,0x00,0x80,0x01,0x00,0x00,0xC4,0x30,0x00,0x00,0xE3,0x3F,0x01,
         0x80,0x31,0x90,0x03,0xE0,0x38,0x0C,0x07,0x40,0x10,0x06,0x0C,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0xF8,0xFF,0xFF,0x1F,0x30,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"率",5*/

         {0x00,0x00,0x00,0x00,0x10,0x00,0x38,0x02,0x38,0x00,0x06,0x0D,0xF8,0x80,0x81,0x0C,0xE0,0xC1,0x40,0x0C,0x80,0x33,0x20,0x0C,0x00,0x1E,0x30,0x0C,0x00,0x0C,0x10,0x0C,0x00,0x1E,0x10,0x0C,0x00,0x16,0x18,0x3C,0x00,0x33,0x08,0x06,0x80,0x21,0x08,0x02,0x80,0x41,0x0C,0x02,0xC0,0x40,0x0C,0x03,0x40,0x80,0x0C,0x01,0x60,0x80,0x0C,0x01,0xE0,0xFF,0x8F,0x00,0x00,0x18,0x8C,0x0C,0x00,0x18,0x4C,0x0C,0x00,0x18,0x4C,0x1C,
         0x40,0x18,0x4C,0x18,0x20,0x18,0x2C,0x20,0x38,0x18,0x2C,0x00,0xF8,0xFF,0x2F,0x00,0x10,0x18,0x18,0x01,0x00,0x18,0x00,0x03,0x00,0x18,0x00,0x03,0x00,0x18,0x00,0x06,0x00,0x18,0x00,0x04,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"波",0*/

         {0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x60,0x04,0x20,0x00,0x18,0x02,0x10,0x00,0x0C,0x02,0x08,0x00,0x07,0x02,0x0C,0x80,0x01,0x02,0x04,0xC0,0x00,0x02,0x06,0x60,0x00,0x02,0x02,0x38,0x00,0x02,0x03,0x1C,0x00,0x02,0x03,0x08,0x20,0x02,0x01,0x00,0x18,0x82,0x01,0x00,0x04,0x82,0x01,0x00,0x03,0x82,0x01,0x80,0x01,0x82,0x01,0xC0,0x00,0x82,0x01,0x60,0xF0,0xFF,0x3F,0x78,0x60,0x82,0x01,0x30,0x00,0x82,0x01,
         0x00,0x10,0x82,0x01,0x00,0x0C,0x82,0x01,0x00,0x06,0x82,0x01,0x00,0x03,0x82,0x01,0x80,0x01,0x82,0x01,0xC0,0x00,0x82,0x01,0xE0,0xF0,0xFF,0x1F,0x70,0xE0,0x00,0x00,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"形",1*/

         {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0x1F,0x38,0x30,0x0C,0x00,0x10,0x30,0x0C,0x00,0x00,0x38,0x8C,0x00,0x00,0x36,0x8C,0x01,0x00,0x33,0x8C,0x01,0x80,0x31,0x8C,0x03,0xC0,0x30,0x0C,0x03,0xC0,0x30,0x0C,0x06,0x70,0x30,0x0C,0x0C,0x70,0x30,0x0C,0x08,0x20,0x38,0x0C,0x00,0x00,0x20,0x08,0x00,0x00,0x01,0x80,0x00,0x80,0xFF,0xFF,0x00,0x80,0x01,0x80,0x00,0x80,0x01,0x80,0x00,
         0x80,0x01,0x80,0x00,0x80,0x01,0x80,0x00,0x80,0xFF,0xFF,0x00,0x80,0x01,0x80,0x00,0x80,0x01,0x80,0x00,0x80,0x01,0x80,0x00,0x80,0x01,0x80,0x00,0x80,0xFF,0xFF,0x00,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"显",2*/

         {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x07,0x00,0x00,0x80,0x1F,0x00,0x00,0x80,0x73,0x20,0x10,0x80,0x01,0x10,0x38,0x80,0x01,0x18,0x38,0x80,0x01,0x0C,0x70,0x80,0x01,0x06,0x60,0x80,0x01,0x03,0xC0,0x80,0x81,0x03,0x80,0x81,0x81,0x01,0x00,0x83,0xC1,0x00,0x00,0x86,0xE1,0x00,0x00,0x88,0x71,0x00,0x00,0x80,0x41,0x00,0x00,0x80,0x01,0x00,0x00,0x80,0x01,0x00,0xFE,0xFF,0xFF,0x3F,0x3C,0x00,0x00,0x00,
         0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x03,0xE0,0x01,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"示",3*/


   };

const unsigned char spo2[5][64]={
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x13,0x18,0x1C,0x0C,0x18,0x06,0x10,0x06,0x20,0x06,0x20,0x06,0x00,0x0E,0x00,0x1C,0x00,0x78,0x00,0xF0,0x01,0xC0,0x07,0x00,0x1E,0x00,0x18,0x00,0x30,0x00,0x30,0x04,0x30,0x04,0x30,0x0C,0x18,0x1C,0x0C,0xE4,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"S",1*/

        {0x00,0x7E,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0xE0,0x19,0x18,0x1E,0x0C,0x1C,0x0C,0x18,0x06,0x18,0x06,0x18,0x06,0x18,0x06,0x18,0x06,0x18,0x06,0x18,0x04,0x18,0x0C,0x1C,0x18,0x1A,0xF0,0x79,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"p",0*/

        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x30,0x0C,0x18,0x18,0x08,0x10,0x0C,0x30,0x04,0x30,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x06,0x60,0x0C,0x30,0x0C,0x30,0x08,0x10,0x18,0x18,0x30,0x0C,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"O",3*/

        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3F,0xF8,0x3F,0x0C,0x20,0x04,0x10,0x04,0x08,0x04,0x04,0x00,0x02,0x00,0x01,0x80,0x00,0x40,0x00,0x20,0x00,0x10,0x00,0x18,0x00,0x0C,0x00,0x0C,0x30,0x0C,0x30,0x0C,0x20,0x0C,0x20,0x18,0x10,0x38,0x08,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"2",4*/

        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x10,0x36,0x18,0x63,0x08,0x63,0x0C,0x63,0x04,0x63,0x04,0x63,0x02,0x63,0x02,0x63,0x01,0x36,0x1D,0x9C,0x37,0x80,0x22,0x80,0x63,0x40,0x63,0x40,0x63,0x20,0x63,0x20,0x63,0x30,0x63,0x10,0x22,0x18,0x36,0x08,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"%",0*/
};
 /*io口定义*/
 //DA
 #define CS_OUT    P2OUT
 #define CS_DIR    P2DIR
 #define CS_R      BIT2
 #define  CS_I     BIT3
 //IO
 #define  IO_R_DIR P6DIR
 #define  IO_R_OUT P6OUT
#define   IO_I_DIR P2DIR
#define   IO_I_OUT P2OUT
 #define  IO_R     BIT2
 #define  IO_I     BIT5

 //void on_red(void);
 void on_ired(void);
 void int_adc(void);
 void caculate(void);
 void delay(void);
 void wave(int a);
 void delay_2(void);
 void int_dac(void);
 void save(void);
 void ad(void);
 void Init_UART1(void);
 void BT_Send(void);   //蓝牙发送
 void SpO2_lcd(void);
 void save(void);
// Global variables
WORD rxByteCount;                        // Momentarily stores the number of bytes received
BYTE buf_bcuartToUsb[BC_RXBUF_SIZE];     // Same size as the UART's rcv buffer
BYTE buf_usbToBcuart[128];               // This can be any size


void SPO2H(int transmit_mode)
{
	USE_LCD();
	SpO2_lcd();
    int_adc();

    Init_UART2();
    UCA0IE |= UCRXIE;

    //PMM_setVCore(PMM_BASE, PMM_CORE_LEVEL_2);
    initClocks(20000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    delay_2();
    delay_2();
//    USB_setup(TRUE,TRUE);
    BUTTON_S4=0;
    while(!(buttonsPressed & BUTTON_S4))
    {
        Init_UART2();
        UCA0IE |= UCRXIE;
        _EINT();
       // delay_2();
      //  delay_2();
        ad();
        if(BUTTON_S4==0){
        on_ired();
        wave(1);    //显示红光
      /*  for(i=0;i<480;i++){
            results[i+480]=results[i];
        }*/
        NONUSE_LCD();
        save();  //      前0-480为红外 1000-1480为
       // delay_2();
  //      delay_2();
        ad();
        if(BUTTON_S4==0){
        on_red();
        wave(0);    //显示红外
       unsigned int count=0;   //ired
       unsigned int count1=0;  //red
       unsigned int count_spa=0;
       unsigned int count1_spa=0;
       //调试时 使用

       int red_hr=0;
       int hr = 0; //心率
       unsigned int j = 0, j1 = 0;


       for(i = 9; i < max-10 ;i++)  //ired
       {
           if(results[i]>=results[i+1]&&results[i]>=results[i+2]&&results[i]>=results[i+3]&&results[i]>=results[i-1]&&results[i]>=results[i-2]&&results[i]>=results[i-3]&&j<4&&
                   results[i]>=results[i+4]&&results[i]>=results[i+5]&&results[i]>=results[i+6]&&results[i]>=results[i-4]&&results[i]>=results[i-5]&&results[i]>=results[i-6]&&
                   results[i]>=results[i+7]&&results[i]>=results[i+8]&&results[i]>=results[i+9]&&results[i]>=results[i-7]&&results[i]>=results[i-8]&&results[i]>=results[i-9])
           {
               r_locate[count] = i;
               count++;
               if(count>=10)break;
               i = i+40;
           }

       }

       for(i = 9+480; i < max-10+480 ;i++)  //red
       {
           if(results[i]>=results[i+1]&&results[i]>=results[i+2]&&results[i]>=results[i+3]&&results[i]>=results[i-1]&&results[i]>=results[i-2]&&results[i]>=results[i-3]&&j1<4&&
                   results[i]>=results[i+4]&&results[i]>=results[i+5]&&results[i]>=results[i+6]&&results[i]>=results[i-4]&&results[i]>=results[i-5]&&results[i]>=results[i-6]&&
                   results[i]>=results[i+7]&&results[i]>=results[i+8]&&results[i]>=results[i+9]&&results[i]>=results[i-7]&&results[i]>=results[i-8]&&results[i]>=results[i-9])
           {
               r1_locate[count1] = i;
               count1++;
               if(count1>=10)break;
               i=i+40;
           }
       }

       /*计算心率 其实最后一个峰值坐标减去第一个 再除以个数就可以了哦*/
       count_spa = count - 1;
       count1_spa = count1 - 1;

      hr = (r_locate[count_spa]-r_locate[0])/count_spa;
      hr=(1.0/(float)(hr*0.01))*60;

      red_hr = (r1_locate[count1_spa]-r1_locate[0])/count1_spa;
      red_hr=(1.0/(float)(red_hr*0.01))*60;  //


      /*直流算平均值*/
      red_dc = 0.0;
      ired_dc = 0.0;
      length_idc = (float)(r_locate[count_spa]-r_locate[count_spa -2]);    //ired
      length_dc = (float)(r1_locate[count1_spa] - r1_locate[count1_spa -2]);
      for(i = r1_locate[count1_spa -2] ; i <r1_locate[count1_spa];i++)    //最后的位置是采样到最后一个峰峰值的坐标
      {
          red_dc += (float)results1[i]/length_dc;
      }
      for(i = r_locate[count_spa -2] ; i <r_locate[count_spa];i++)    //最后的位置是采样到最后一个峰峰值的坐标
      {
          ired_dc += (float)results1[i]/length_idc;
      }

      /*最小值及交流峰峰值 最后2个周期才稳定 可以用最后2个周期算峰峰值*/
      /*同时 红光用前一个峰值减去谷值 红外用后一个峰值减去谷值 去基线漂移*/
       red_min = results[1+480];//(float)red[0];
       ired_min = results[1];//(float)results[0];  避免第一个数采得不准
       ired_ac = 0;
       red_ac = 0;

       for(i =( count_spa -2); i <count_spa ;i++)  //ired
       {
           for(j=r_locate[i]; j < r_locate[i+1] ;j++)
           {
               if(results[j]<ired_min){ired_min=results[j];}
           }
           ired_ac += results[r_locate[i+1]] - ired_min;
           nr_locate[i]=ired_min;
           ired_min = results[1]; //更新初值
       }
       ired_ac = ired_ac/2;

       for(i = (count1_spa -2); i <count1_spa ;i++)  //red
       {
           for(j=r1_locate[i]; j < r1_locate[i+1] ;j++)
           {
               if(results[j]<red_min){red_min=results[j];}
           }
           red_ac += results[r1_locate[i]] - red_min;
           nr1_locate[i] = red_min;
           red_min = results[481];
       }
       red_ac = red_ac/2;


       /*计算血氧含量*/
       Q1 = (float)(red_ac/red_dc);
       Q2 = (float)ired_ac/(float)ired_dc ;
       Q = Q1/Q2 ;
       sao2 = (-4.1768)*Q + 100.9352 + 0.5;

       if(abs(hr-red_hr)>=10)                  //两次采样的心率值相差不大的情况下才更新Q
       {
           Q = pre_Q;
       }

       /*显示*/
       USE_LCD();
       SPILCD_Clear_Lim(212,260,18,52,WHITE); //371 355
       SPILCD_Clear_Lim(348,396,18,52,WHITE);
       unsigned int temp1,temp10,temp100;
       long temp_final1;
       long temp_final2;

       temp1=hr%10;             //计算心率个位
       temp10=(hr%100-temp1)/10;    //计算心率十位
       temp100=(hr-temp10*10-temp1)/100;    //计算心率百位
       temp_final1=(long)(temp1+temp10*10+temp100*100);
       if(temp100>0)    DRAW_NUM(244,20,temp100,BLUE);  //显示心率百位
       DRAW_NUM(228,18,temp10,BLUE);         //显示心率十位
       DRAW_NUM(212,18,temp1,BLUE);       //显示心率个位

       sao2=99;
       temp1=sao2%10;                  //计算个位
       temp10=(sao2%100-temp1)/10;    //计算十位
       temp100=(sao2-temp10*10-temp1)/100;    //计算百位
       temp_final2=(long)(temp1+temp10*10+temp100*100);
       if(temp100>0)    DRAW_NUM(380,18,temp100,BLUE);  //显示心率百位
       DRAW_NUM(364,18,temp10,BLUE);  //显示十位
       DRAW_NUM(348,18,temp1,BLUE);    //显示个位
       NONUSE_LCD();
       i = 0;

       pre_Q = Q;   //记录这次显示的Q

  /*     char start[7]={'S','T','A','R','T','S','O'};
       long a;
       char b[5];
       char c[3];
       char d[3];
       ltoa(temp_final1,b);
       if(temp_final1>=0&&temp_final1<=9)
       {
       c[0]='0';
       c[1]='0';
       c[2]=b[0];
       }
       if(temp_final1>=10&&temp_final1<=99)
       {
       c[0]='0';
       c[1]=b[0];
       c[2]=b[1];
       }
       if(temp_final1>=100&&temp_final1<=999)
       {
       c[0]=b[0];
       c[1]=b[1];
       c[2]=b[2];
       }

       ltoa(temp_final2,b);
       if(temp_final2>=0&&temp_final2<=9)
       {
       d[0]='0';
       d[1]='0';
       d[2]=b[0];
       }
       if(temp_final2>=10&&temp_final2<=99)
       {
       d[0]='0';
       d[1]=b[0];
       d[2]=b[1];
       }
       if(temp_final2>=100&&temp_final2<=999)
       {
       d[0]=b[0];
       d[1]=b[1];
       d[2]=b[2];
       }

       char end[3]={'E','N','D'};
       j=0;
       for(j=0;j<960;j++)        //采样数据除以40转化成两位，便于传送
           {
           results[j]=results[j]/40;
           }*/
       int i;
       for(i=959;i>=0;i--)
       {
           results[i+10]=results[i]>>5;
       }
       results[0]='S';
       results[1]='T';
       results[2]='A';
       results[3]='R';
       results[4]='T';
       results[5]='S';
       results[6]='O';
       if(hr>=127){
       results[7]=0x7F;
       results[8]=(char)(hr-127);}
       else{
     	  results[7]=(char)(hr);
     	  results[8]=0;
       }
       results[9]=sao2;
       results[970]='A';
       results[971]='A';
       results[972]='A';
       results[973]='A';
       results[974]='A';
//       results[974]=0x7F;
       results[975]='E';
       results[976]='N';
       results[977]='D';
//       CRCresult=0;
//       for(i=9;i<969;i++)
//       {
//           results[i]=results[i]>>5;
//           CRCtmp = CRCresult%256;
//           CRCindex = CRCtmp^results[i];
//           CRCtmp = CRCresult/256;   //除以256
//           CRCresult = CRCtmp;
//           CRCresult = CRCresult^CRC_TA[CRCindex];
//       }
//       long a;
//       char b[5];
//       a=CRCresult;
//       ltoa(a,b);
//       results[969]=b[0];
//       results[970]=b[1];
//       results[971]=b[2];
//       results[972]=b[3];
//       results[973]=b[4];
//       results[974]='E';
//       results[975]='N';
//       results[976]='D';



       if(transmit_mode==1){
           bcUartInit();
           bcUartSend_char(results,978);//buf_bcuartToUsb
       }


       if(transmit_mode==2){
           hidSendDataInBackground(results,1956, HID0_INTFNUM,10000);
       }


        if(transmit_mode==3){
      	 int i;
      	 for(i=0;i<978;i++)
      	 {
               UCA0TXBUF = results[i];

               while(!(UCTXIFG==(UCTXIFG & UCA0IFG))&&((UCA0STAT & UCBUSY)==UCBUSY));

               int a,k;
               for(a=0;a<10;a++)
               {
                    for(k=0;k<80;k++);
               }
      	 }
       }

	   BUTTON_S4=0;
	   buttonsPressed=0;
	   NONUSE_LCD();
        }
        }
    }
    USE_LCD();
}

void save(void)
{
    /*save data*/
    int spo=0;//save计数
    for(spo = 0; spo < 480 ;spo++)
    {
        results[spo+480] = results[spo];
        results1[spo+480] = results1[spo];
    }
}

void SpO2_lcd(void)
{
    SPILCD_Clear(WHITE);
    //Show_ALLTHEME();
    DRAW_Other(250,274,2);
    DRAW_LINE(15,60,465,60,RED);
    DRAW_han(420,310,HRhan2[2]);
    DRAW_han(380,310,HRhan2[3]);
    DRAW_han(340,310,HRhan2[4]);
    DRAW_han(300,310,HRhan2[5]);

    //SpO2  %
    DRAW_Other(460,16,6); //s
    DRAW_Other(444,16,7); //p
    DRAW_Other(428,16,8); //o
    DRAW_Other(412,16,9); //2
    DRAW_Other(396,16,2); //:
   // DRAW_Other(348,16,10); //%
    DRAW_han(340,48,HRhan2[0]); //心
    DRAW_han(308,48,HRhan2[1]); //率
    DRAW_Other(260,16,2); //:

    NONUSE_LCD();
}


void int_adc(void){
    P6SEL |= BIT0 +   BIT1;                // p6.0 p6.1
    ADC12CTL0 |= ADC12ON+ADC12REF2_5V + ADC12REFON+ADC12SHT0_0;
    ADC12CTL1|= ADC12SHP + ADC12CONSEQ_3 + ADC12SHS_2 + ADC12SSEL_1;       // Repeat-sequence-of-channels | TIMER B |ACLK
    TB0CCTL0|=0x0080;                   //????????compare 计时 OUTMOD :100当定时器计数到TACCRX值时，输出翻转。输出信号的周期将是定时器的2倍
    TB0CCR0 = 80;
    TB0CCR1= 40;//比较模式下，用来设定计数终值；
    TB0CTL|=TBSSEL_1+MC_1+TBCLR;   //SMCLK |Up mode: Timer counts up to TB0CL0增模式记到CCR0 |
    //ADC12MCTL0 |= ADC12INCH_0;//+ADC12SREF_5;         //2 channels VR+ = VREF+ and VR- = AVSS A6
    //ADC12MCTL1 |= ADC12INCH_1+ADC12EOS;//+ADC12SREF_5;     // ！！！测两个通道的时候，最尾加 EOS才知道结束！
    ADC12MCTL0 |= ADC12INCH_0;//+ADC12SREF_5;         //2 channels VR+ = VREF+ and VR- = AVSS A6
    ADC12MCTL1 |= ADC12INCH_1+ADC12EOS;
    //ADC12IE |= 0x03;  ////ADC12MEM0 interrupt flag ADC12IFG0 | ADC12MEM1 interrupt flag ADC12IFG1
    //ADC12CTL0 |= ADC12ENC;                    // Enable conversions  ADC12CTL0 |= ENC;

}

void int_dac(void)
{
    /*初始化da*/
    CS_DIR |= CS_R + CS_I;
    CS_OUT |= CS_R + CS_I;
    IO_R_DIR |= IO_R;
    IO_I_DIR |= IO_I;

    /*UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCB0CTL0 |= UCMST+UCSYNC+UCMSB+UCCKPL;    // 3-pin, 8-bit SPI master                                           // Clock polarity high, MSB
    UCB0CTL1 |= UCSSEL_2+UCSWRST;                     // SMCLK
    UCB0BR0 = 0x20;                           // 2
    UCB0BR1 = 0X00;                              //
    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCB0IE &= ~UCRXIE;*/
}

void on_red(void)
{
    /*驱动红光*/


    IO_R_OUT |= IO_R;
    IO_I_OUT |= IO_I;
    red_flage = 1; //在显示波形的前驱动上红外光

    /*DAC*/

    MSB = 0x05;                          // Initialize data values
    LSB = 0x60;

    for(i=500;i>0;i--);
    CS_OUT &= ~CS_I;//P3.1为输出为0  ired
    for(i=500;i>0;i--);                // Wait for slave to initialize
    while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready? 空了表示传完了
    UCB0TXBUF = 0x00;
    while (!(UCB0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
    UCB0TXBUF = 0x00;                 // Send next value
    while (!(UCB0IFG&UCTXIFG));
    for(i=500;i>0;i--);
    CS_OUT |= CS_I;
    for(i=500;i>0;i--);

    CS_OUT |=  CS_I;
    for(i=500;i>0;i--); //p4.5 = 1  red // Now with SPI signals initialized,
    CS_OUT &= ~CS_R;                      // Wait for slave to initialize
    while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready? 空了表示传完了
    UCB0TXBUF = MSB;
    while (!(UCB0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
    UCB0TXBUF = LSB;                 // Send next value
    while (!(UCB0IFG&UCTXIFG));
    for(i=500;i>0;i--);
    CS_OUT |=  CS_R;

    IO_R_OUT &=~ IO_R;

}

void ad(void)
{
    num1 = 0;
    num2 = 0;

    /*AD采样*/
    ADC12IE = 0x00;
    ADC12IE = ADC12IE0;  ////ADC12MEM0 interrupt flag ADC12IFG0 | ADC12MEM1 interrupt flag ADC12IFG1
    ADC12CTL0 |= ADC12ENC+ADC12ON;                    // Enable conversions  ADC12CTL0 |= ENC;
    while(!BUTTON_S4)
    {
        _EINT();
       // ADC12CTL0 |= ADC12SC;   //!!完全不是靠开中断触发 时候靠设置的定时器来触发的！
        _NOP();
        if(num1>=max&&num2>=max)break;

    }
    /*关闭AD*/
    //_DINT();
    ADC12IE &= ~0x03;  ////ADC12MEM0 interrupt flag ADC12IFG0 | ADC12MEM1 interrupt flag ADC12IFG1
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 &= ~ADC12ON;
}





void on_ired()
{
    //试试先关闭红光 再打开红外

    /*DA*/
    IO_R_OUT |= IO_R;
    for(i=500;i>0;i--);
    CS_OUT &= ~CS_R;//P4.4为输出为0  red
    for(i=500;i>0;i--);                        // Wait for slave to initialize  数据开始传送的下降沿需要等待
    while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready? 空了表示传完了
    UCB0TXBUF = 0x00;
    while (!(UCB0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
    UCB0TXBUF = 0x00;                 // Send next value
    while (!(UCB0IFG&UCTXIFG));
    for(i=500;i>0;i--);
    CS_OUT |= CS_R;

    MSB = 0x05;                          // Initialize data values
    LSB = 0x60;
    for(i=500;i>0;i--);
    CS_OUT &= ~CS_I;//P3.1为输出为0  ired
    for(i=500;i>0;i--);                // Wait for slave to initialize
    while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready? 空了表示传完了
    UCB0TXBUF = MSB;
    while (!(UCB0IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
    UCB0TXBUF = LSB;                 // Send next value
    while (!(UCB0IFG&UCTXIFG));
    for(i=500;i>0;i--);
    CS_OUT |= CS_I;
    IO_I_OUT &=~  IO_I;//P2.0为输出为0
    //IO_R_OUT |= IO_R;//P2.1为输出为1

    //int_adc();
}


void wave(int a)
{
    /*均值滤波*/
   /* int savemid[pinghua];
    float c=0;
    int k1,k2;
    for(k1=2 ; k1<max-1 ;k1++)
    {
        for(k2=0 ; k2<pinghua ;k2++)
        {
           c += results[k1-2+k2];
        }
        c = c/pinghua;
        results[k1] = c;
        c = 0;     //每次循环后赋值为0
    }

    USE_LCD();
    SPILCD_Clear_Lim(0,479,61,270,WHITE); //清除原来的波形
    int i,j,j1;
    float k;
    //用采样得到的最大最小值 将波形固定在70-260

    if(a)          //red
    {
        rt = results[1];
        rb = results[1];
        for(i = 0; i < max-1 ;i++)
        {

            if(rt < results[i+1] )   //防止采样错误
             {rt = results[i+1];}
            if(rb > results[i+1] )
             {rb = results[i+1];}
        }
        ryz = rb + 0.75*(float)(rt-rb);
        k = (float)190/((float)rt-(float)rb);
      //  rb = rb*k;
        for(i=0;i<max-1;i++)
        {
            j=results[i]*k-rb*k+70;
            j1=results[i+1]*k-rb*k+70;
            DRAW_LINE(480-i,j,479-i,j1,BRRED);
        } */
    USE_LCD();
    SPILCD_Clear_Lim(0,479,61,270,WHITE); //清除原来的波形
    int i,j,j1;
    int savemid[pinghua];
     float c=0;
     int k1,k2;


    if(a)          //red
    {
        for(k1=2 ; k1<max-1 ;k1++)
        {
            for(k2=0 ; k2<pinghua ;k2++)
            {
               c += results[k1-2+k2];
            }
            c = c/pinghua;
            results[k1] = c;
            c = 0;     //每次循环后赋值为0
            j=results[k1]*0.046+70;
            j1=results[k1+1]*0.046+70;
            DRAW_LINE(480-k1,j,479-k1,j1,BRRED);
        }

    }

    else                   //ired
    {
      /*  it = results[1]; //TOP
        ib = results[1]; //BOTTOM
        for(i = 0; i < max-1 ;i++)
        {
            if(it < results[i+1] )   //防止采样错误
             {it = results[i+1];}
            if(ib > results[i+1] )
             {ib = results[i+1];}
        }
        iyz = ib + 0.75*(float)(it-ib);
        k = (float)190/((float)it-(float)ib);
      //  ib = ib*k;
        for(i=0;i<max-1;i++)
        {
            j=results[i]*k-ib*k+70;
            j1=results[i+1]*k-k*ib+70;
            DRAW_LINE(480-i,j,479-i,j1,GRAY);
        }*/

        for(k1=2 ; k1<max-1 ;k1++)
        {
            for(k2=0 ; k2<pinghua ;k2++)
            {
               c += results[k1-2+k2];
            }
            c = c/pinghua;
            results[k1] = c;
            c = 0;     //每次循环后赋值为0
            j=results[k1]*0.046+70;
            j1=results[k1+1]*0.046+70;
            DRAW_LINE(480-k1,j,479-k1,j1,BLACK);
        }
    }
    NONUSE_LCD();

}

void delay(void)
{
  TA1CTL&=~TAIFG;
  TA1CCR0=630; //0.02s
  TA1CTL=TASSEL_1+MC_1+TACLR;
  while((TA1CTL&TAIFG)==0);
    TA1CTL&=~TAIFG;
}

void delay_2(void)
{
    TA1CTL&=~TAIFG;
    TA1CCR0=0x0800;    //2S
    TA1CTL=TASSEL_1+MC_1+TACLR;    //ACLK 32768hz   增加模式
    while((TA1CTL&TAIFG)==0);
    TA1CTL&=~TAIFG;
}


