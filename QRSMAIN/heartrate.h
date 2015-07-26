/*
 * heartrate.h
 *
 *  Created on: 2014��9��13��
 *      Author: ty
 */

#ifndef HEARTRATE_H_
#define HEARTRATE_H_

typedef unsigned char BYTE;

extern volatile int index11;      //ԭʼ���������±�
extern volatile int d11;
extern volatile int point_color;
extern volatile int results[1500];	// ���ԭʼ��������
extern volatile int results1[1517];            //���ת��֮���LCD��ʾ����
//extern volatile int tran_mode;

extern volatile int menu_select;
extern volatile int HRate;
extern volatile int index_ECG;

extern volatile unsigned int CRCresult;
extern volatile unsigned char CRCtmp,CRCindex;
extern volatile const unsigned int CRC_TA[256];

void  ADC12_IO_Init();
void HRCaculate();

extern void HR(int transmit_mode);
extern void convertTwoDigBinToASCII(unsigned int bin, BYTE* str);
//extern void TRAN(void);

#endif /* HEARTRATE_H_ */
