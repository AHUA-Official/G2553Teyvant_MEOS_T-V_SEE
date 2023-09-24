/*
 * TouchIN.h
 *
 *  Created on: 2013-2-20
 *      Author: Administrator
 */

#ifndef TOUCHIN_H_
#define TOUCHIN_H_

extern void TouchIN_Dect()	;												//WDT中断事件
extern unsigned char TouchIN;											// 相当于PxIN寄存器作用，支持8个触摸按键
//extern unsigned int TouchIN;												// 改为此句代码可支持16个触摸按键



#endif /* TOUCHIN_H_ */
