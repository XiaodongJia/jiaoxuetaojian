/*
 * bt.h
 *
 *  Created on: 2014Äê9ÔÂ14ÈÕ
 *      Author: lenovo
 */

#ifndef BT_H_
#define BT_H_

extern char degc;
extern unsigned int accel_end;
extern unsigned int Next;
extern unsigned char RxBuf[20];
extern unsigned char RxBuf1[20];
extern unsigned int endflag;
extern unsigned int RxPoint;
extern unsigned int RxPoint1;
void btSend_char(char * buf, unsigned int len);
extern void Init_UART1(void);
extern void Init_UART2(void);
extern void Init_UART3(void);

#endif /* BT_H_ */
