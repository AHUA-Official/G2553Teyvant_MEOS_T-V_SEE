/* Timer_A3.c
 *描述：由Grace自动生成，启用CCI1B的下降沿捕获功能
 *  Created on: 2013-4-3
 *      Author: Administrator */
#include "msp430G2553.h"
#include "Slope.h"

void Timer0_A3_init(void)
{
	//----捕获模式----
	TA0CTL = TASSEL_2+ID_0+MC_2;          	//连续计数开始
    TA0CCTL1 = CM_1 + CCIS_1 + SCS + CAP +OUTMOD_0 + CCIE;	//OUTMOD_0是Grace强行配送的，可不加
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR_HOOK(void)
{
    switch (__even_in_range(TA0IV, TA0IV_TAIFG))    // Efficient switch-implementation
    {
        case TA0IV_TACCR1:
            Slope_TA_CCI1B();            break;
        case TA0IV_TACCR2:            break;
        case TA0IV_TAIFG:            break;
        default:break;
    }
}




