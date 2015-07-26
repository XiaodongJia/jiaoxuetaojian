/*
 * spo2h.h
 *
 *  Created on: 2014Äê9ÔÂ14ÈÕ
 *      Author: lenovo
 */

#ifndef SPO2H_H_
#define SPO2H_H_

extern volatile int num1;
extern volatile int num2;
extern volatile int tran_mode_SP;
extern volatile int iyz;
extern volatile int ryz;

extern void SPO2H(int transmit_mode);
extern void on_red(void);
extern volatile int ad_f;

#endif /* SPO2H_H_ */
