/* Timer_A3.c
 *��������Grace�Զ����ɣ�����CCI1B���½��ز�����
 *  Created on: 2013-4-3
 *      Author: Administrator */
#include "msp430G2553.h"
#include "Slope.h"

void Timer0_A3_init(void)
{
	//----����ģʽ----
	TA0CTL = TASSEL_2+ID_0+MC_2;          	//����������ʼ
    TA0CCTL1 = CM_1 + CCIS_1 + SCS + CAP +OUTMOD_0 + CCIE;	//OUTMOD_0��Graceǿ�����͵ģ��ɲ���
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




