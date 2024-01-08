/*
 * TA_PWM.h
 *
 *  Created on: 2013-2-9
 *      Author: Administrator
 */

#ifndef TA_PWM_H_
#define TA_PWM_H_

extern char TA0_PWM_Init(char Clk,char Div,char Mode1,char Mode2);
extern char TA0_PWM_SetPeriod(unsigned int Period);
extern char TA0_PWM_SetPermill(char Channel,unsigned int Duty);

extern char TA1_PWM_Init(char Clk,char Div,char Mode1,char Mode2);
extern char TA1_PWM_SetPeriod(unsigned int Period);
extern char TA1_PWM_SetPermill(char Channel,unsigned int Duty);

#endif /* TA_PWM_H_ */
