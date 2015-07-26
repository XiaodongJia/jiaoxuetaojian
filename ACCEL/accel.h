/*
 * accel.h
 *
 *  Created on: 2014Äê9ÔÂ14ÈÕ
 *      Author: lenovo
 */

#ifndef ACCEL_H_
#define ACCEL_H_

extern unsigned int interupt_flag;
extern void ACCELMENU(int transmit_mode);
extern void ACCELNAME(int transmit_mode);
extern void ACCEL(int transmit_mode);
extern void ACCEL2(int transmit_mode);
extern void WALK(int transmit_mode);
extern void WALK2(int transmit_mode);
//extern void WALK(void);
extern volatile int accel_select;

#endif /* ACCEL_H_ */
