/***********************************************************************
 *
 *		设备		：	MSP430f5529
 * 		名称 		:  	ADC采样
 *		功能		:  	ADC12模块程序
 *
 *														2013/7/28
 *												  		   Sunchy
 *
 ***********************************************************************/

#include "ADC12.h"
#include "TFT_Screen.h"
#include "heartrate.h"
#include "spo2h.h"
#include "bloodpreasure.h"
#include "accel.h"


//=============中断向量====================
/**************ADC12中断*****************/

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{


	switch(__even_in_range(ADC12IV,36))
	{
	case  0: break;                           	// Vector  0:  No interrupt
  	case  2: break;                           	// Vector  2:  ADC overflow
  	case  4: break;                           	// Vector  4:  ADC timing overflow
    case  6:                                   // Vector  6:  ADC12IFG0
        if(num1<=480||num2<=480)
            {
            results[num1] =ADC12MEM0;
            num1++;
           // if((ADC12IV && 8)==8){
                results1[num2] = ADC12MEM1;
                num2++;}
           // }

        break;

    case  8:
     /*  if(num1<=480||num2<=480&&ad_f==1)
        {
            results1[num2] = ADC12MEM1;
            num2++;
            if((ADC12IV && 6)==6){
              results[num1] = ADC12MEM0;
              num1++;}// Move results, IFG is cleared// Vector  8:  ADC12IFG1
        }*/
      //  if(ad_f!=1)
       // {
            if (section==0){
              x=ADC12MEM1;   //处于充气阶段
            }
            else{
                results1[n]=ADC12MEM0;
                results[n]=ADC12MEM1;
                      n++;

                  flag=flag%2;  //曲线的密集程度
                      if(flag==0){
                          xaxis=xaxis%480;
                          if(xaxis==0){
                              SPILCD_Clear_Lim(0,479,61,270,WHITE); //清除原来的波形
                              //drawbpletter();
                              //DRAW_LINE(0,60,480,60,RED);
                          }
                          DRAW_LINE(480-xaxis,mm,480-xaxis+1,ADC12MEM0/20+60,BLACK);
                          DRAW_LINE(480-xaxis,xx,480-xaxis+1,ADC12MEM1/18+40,GREEN);
                          xaxis++;
                          xx=ADC12MEM1/18+40;
                          mm=ADC12MEM0/20+60;
                      }
                      flag++;
            }
//            __bic_SR_register_on_exit(LPM0_bits);   // 退出低功耗
    //    }
        break;

	case  10:                                  // Vector  10:  ADC12IFG2
		interupt_flag=1;
 		ADC12IE &=~ ADC12IE2;
 	    ADC12CTL0 &= ~ADC12SC;//启动采样
 	    ADC12CTL0 &= ~ADC12ENC+ADC12ON;		// Move results, IFG is cleared

  	case 12: break;                           	// Vector 12:  ADC12IFG3
  	case 14: break;                           	// Vector 14:  ADC12IFG4
  	case 16: break;                           	// Vector 16:  ADC12IFG5
 	case 18: break;                           	// Vector 18:  ADC12IFG6
 	case 20: break;                           	// Vector 20:  ADC12IFG7
 	case 22: break;                           	// Vector 22:  ADC12IFG8
 	case 24: break;                           	// Vector 24:  ADC12IFG9
 	case 26:
 		interupt_flag=1;
 		ADC12IE &=~ ADC12IE10;
 	    ADC12CTL0 &= ~ADC12SC;//启动采样
 	    ADC12CTL0 &= ~ADC12ENC+ADC12ON;
 		break;                           	// Vector 26:  ADC12IFG10
 	case 28: break;                           	// Vector 28:  ADC12IFG11
 	case 30:
 	            results1[index11] = ADC12MEM12;          	// Move results
 		  		//转换采样数据在LCD上显示
 		  		if(index11>=9&&index11<=488)
 		  		{
 		  		    SPILCD_Clear_Lim(488-index11,488-index11,61,270,WHITE);
 		  			DRAW_LINE(489-index11,results1[index11-1]/20+50,488-index11,results1[index11]/20+50, BLACK);}
 		  		if(index11>=506)
 		  		{
 		  		 SPILCD_Clear_Lim(985-index11,985-index11,61,270,WHITE);  //958-980
 		  		 DRAW_LINE(986-index11,results1[index11-1]/20+50,985-index11,results1[index11]/20+50, BLACK); //959-981
 		  		}
 		  		index11++;
                //  __bic_SR_register_on_exit(LPM0_bits);   // 退出低功耗
 	  		if (index11 == 489)
 		  		{
 	  		     index_ECG = 0x33;
 	  		     SPILCD_Clear_Lim(479,479,61,270,WHITE);
 	  		    /* for(index11=7;index11<=486;index11++){
 	  		         results1[index11]=results1[index11]>>4;
 	  		     }*/
 	  		    // HRCaculate();
 	  		     index11=506;
 		  		 __bic_SR_register_on_exit(LPM0_bits);   // 退出低功耗
 		  		}
 	  		if(index11 == 986)  //959
 	  		{
 	             index_ECG = 0x22;
 	             SPILCD_Clear_Lim(479,479,61,270,WHITE);
 	            /*for(index11=502;index11<982;index11++){
 	               results1[index11]=results1[index11]>>4;
 	              }*/
 	             index11 = 9;
 	             __bic_SR_register_on_exit(LPM3_bits);   // 退出低功耗
 	  		}
 		break;                           	  	// Vector 30:  ADC12IFG12


 	case 32: break;                           	// Vector 32:  ADC12IFG13
 	case 34: break;                           	// Vector 34:  ADC12IFG14
 	case 36: break;							  		// Vector 36:  ADC12IFG15
 	default: break;
  	}
}
