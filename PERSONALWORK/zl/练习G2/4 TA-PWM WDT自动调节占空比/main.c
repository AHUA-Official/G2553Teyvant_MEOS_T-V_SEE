
/*
 * Breath_Lamp.c
 * Ӳ��������P1.6(PWM����ڣ�����1���̹�LED��������ñ���ӣ���
 * ����������LED������ʱ�佥�������ֽ����䰵���˳ơ������ơ��������TA_PWM�⺯����
 *  Created on: 2013-4-8
 *  Author: Administrator
 */
#include "MSP430G2553.h"
#include "TA_PWM.h"
//#include "Breath_Lamp_Event.h"
//#include "Breath_Lamp_Global.h"

#define PWM_PERIOD 10

extern void WDT_Ontime();

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	//-----��ʼ��TA��ʱ��-----
	TA0_PWM_Init('A',1,'F',0);//ACLK, ����Ƶ��TA0.1 �����ǰPWM
        //TA1_PWM_Init('A',1,'F','F');
	TA0_PWM_SetPeriod(PWM_PERIOD);//�趨PWM������
        TA1_PWM_SetPeriod(PWM_PERIOD);
	//-----��ʼ�����Ź���ʱ��-----
	WDTCTL = WDT_ADLY_16;//WDT��Ϊ16ms��ʱ��ģʽ
	IE1 |= WDTIE;// ʹ�ܼĴ���IE1����Ӧ��WDT�ж�λ
	 
         _BIS_SR(LPM3_bits + GIE);//ʹ�����ж�
         //_enable_interrupts();								//ʹ�����ж�
	 //_bis_SR_register(LPM3_bits) ;
}
/******************************************************************************************************
 * ��       �ƣ�WDT_ISR()
 * ��       �ܣ���ӦWDT��ʱ�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����WDT��ʱ�ж϶�ռ�ж����������������һ���ж��ж��¼���Ҳ�����˹������־λ��
 *                  ���ԣ���WDT��ʱ�жϷ����Ӻ����У�ֱ�ӵ���WDT�¼��������Ϳ����ˡ�
 * ��       ������
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR		// Watch dog Timer interrupt service routine
__interrupt void WDT_ISR(void)
{
	WDT_Ontime();
}
