/*
 * key.h
 *
 *  Created on: 2014-7-28
 *      Author: admin
 */

#ifndef KEY_H_
#define KEY_H_

extern void scankey(void);
extern void init_key(void);

extern volatile int BUTTON_S1;
extern volatile int BUTTON_S2;
extern volatile int BUTTON_S3;
extern volatile int BUTTON_S4;
extern volatile int buttonsPressed;

#endif /* KEY_H_ */
