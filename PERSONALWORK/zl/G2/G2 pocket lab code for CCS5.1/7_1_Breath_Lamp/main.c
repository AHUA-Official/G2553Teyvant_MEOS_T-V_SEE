/*
 * Breath_Lamp.c
 * 硬件描述：P1.6(PWM输出口）接了1个绿光LED（用跳线帽连接）。
 * 功能描述：LED亮度随时间渐渐变量又渐渐变暗，人称“呼吸灯”。需调用TA_PWM库函数。
 *  Created on: 2013-4-8
 *  Author: Administrator
 */
#include "MSP430G2553.h"
#include "TA_PWM.h"
#include "Breath_Lamp_Event.h"
#include "Breath_Lamp_Global.h"

#define PWM_PERIOD 100

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	//-----初始化TA定时器-----
	TA0_PWM_Init('A',1,'F',0);							//ACLK, 不分频，TA0.1 输出超前PWM
	TA0_PWM_SetPeriod(PWM_PERIOD);		//设定PWM的周期
	//-----初始化看门狗定时器-----
	WDTCTL = WDT_ADLY_16;  						//WDT设为16ms定时器模式
	 IE1 |= WDTIE;                              				// 使能寄存器IE1中相应的WDT中断位
	 _enable_interrupts();								//使能总中断
	 _bis_SR_register(LPM3_bits) ;
}
/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：响应WDT定时中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：WDT定时中断独占中断向量，所以无需进一步判断中断事件，也无需人工清除标志位。
 *                  所以，在WDT定时中断服务子函数中，直接调用WDT事件处理函数就可以了。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR		// Watch dog Timer interrupt service routine
__interrupt void WDT_ISR(void)
{
	WDT_Ontime();
}

