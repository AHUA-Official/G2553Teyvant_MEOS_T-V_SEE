/*
 * TouchPad_LED
 * 硬件描述：P1.0接了1个红光LED（用跳线帽连接），P2.0和P2.5引出两个触摸板
 * 功能描述：按下P2.0灯亮，按下P2.5灯灭。
 *  Created on: 2013-4-8
 *  Author: Administrator
 */
#include "msp430g2553.h"
#include "TouchIN.h"				/*触摸按键检测库函数*/

void WDT_Ontime(void);

//-----对硬件相关代码进行宏定义处理-----
#define LED_ON 	P1OUT |= BIT0 		/*宏定义LED所在IO*/
#define LED_OFF 	P1OUT &= ~BIT0 	/*宏定义LED所在IO*/

void main(void) {
	WDTCTL = WDTPW+WDTHOLD;	//关狗
	//-----初始化GPIO-----
	P1DIR |= BIT0; 							//LED所连IO口P1.0设为输出
	P1OUT &= ~BIT0;
	//-----初始化WDT定时中断为16ms-----
	WDTCTL = WDT_ADLY_16;        // “超级”宏定义
    IE1 |= WDTIE;								//使能WDT中断
	 _enable_interrupts();         		// 等同_EINT，使能总中断
	 _bis_SR_register(LPM3_bits);	//等同LPM3
	//while(1);
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
/******************************************************************************************************
 * 名       称：WDT_Ontime()
 * 功       能：WDT定时中断事件处理函数，即当WDT定时中断发生后后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void WDT_Ontime(void)
{
	//-----首先必须定时扫描触摸按键检测函数-----
	TouchIN_Dect();
	if(TouchIN & BIT0) LED_ON ;
	if(TouchIN & BIT1)	 LED_OFF;
	}
