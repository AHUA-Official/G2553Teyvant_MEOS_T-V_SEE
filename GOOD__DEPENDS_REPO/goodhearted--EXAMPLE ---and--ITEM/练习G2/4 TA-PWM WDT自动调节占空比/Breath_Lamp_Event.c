/*
 *  �������¼���������
*/
#include "msp430g2553.h"    /*��Ƭ���Ĵ���ͷ�ļ�*/
#include "Breath_Lamp_Event.h"
//#include "Breath_Lamp_Global.h"	/*ϵͳȫ�ֱ���*/
#include "TA_PWM.h"
/******************************************************************************************************
 * ��       �ƣ�WDT_Ontime()
 * ��       �ܣ�WDT��ʱ�ж��¼�������������WDT��ʱ�жϷ��������һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�����������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void WDT_Ontime()
{
	static int Bright_Delta=0;
	TA0_PWM_SetPermill(1,Bright);	//����PWMռ�ձ�
        //TA1_PWM_SetPermill(2,Bright);
        //TA1_PWM_SetPermill(1,Bright);
	if(Bright>=400)	//ռ�ձ����40%�������������Ӿ��仯������
		Bright_Delta=-3;
	if(Bright<=10)	//����
		Bright_Delta=3;
	Bright=Bright+Bright_Delta;
	_NOP();	//���ڲ���ϵ���Գ�����
}
