/*���棺����G2 LaunchPad�ϵ�LED2ʹ����P1.6��I2C���ţ������������漰��I2C��ʵ�鶼�����P1.6���߰γ�,���������̣�
 * main.c
 */#include <msp430.h>
#include "LCD_128.h"
#include "HT1621.h"
#include "TCA6416A.h"

long temp;
long IntDeg;
void ADC10_ISR(void);
void ADC10_init(void);
void LCD_Init();
void LCD_Display();

void main()
{
	WDTCTL=WDTPW+WDTHOLD;
	ADC10_init();
	LCD_Init();
    while(1)
    {
      ADC10CTL0 |= ENC + ADC10SC;            // Sampling and conversion start
 //     _bis_SR_register(CPUOFF + GIE);       	 // LPM0 with interrupts enabled
      __delay_cycles(1000);
      //-----ADCת������жϻ���CPU���ִ�����´���-----
      temp = ADC10MEM;							//��ȡAD����ֵ
      IntDeg= temp*4225/1024 - 2777;		//ת��Ϊ���϶ȣ���10������
 //  IntDeg=-123;									//�������Ի�ø��£�ֱ�Ӹ���ֵ�Բ���LCD��ʾ
      LCD_Display();										//����LCD��ʾ����
    }
}
/******************************************************************************************************
 * ��       �ƣ�LCD_Init()
 * ��       �ܣ���ʼ��LCD��ʾ��ص�Ӳ������Ԥ��̶��������ʾ����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����Ԥ����ʾ���ݰ������϶�oC���Լ�С���㡣
 * ��       ������
 ******************************************************************************************************/
void LCD_Init()
{
    TCA6416A_Init();
    HT1621_init();
	//-----��ʾ�̶������LCD��-----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	LCD_DisplaySeg(_LCD_DOT2);		//�¶�С����
	//-----�������조o��-----
	LCD_DisplayDigit(9,5);
	LCD_ClearSeg(_LCD_5D);
	LCD_ClearSeg(_LCD_5C);
	//-----�������조C��-----
	LCD_DisplayDigit(0,6);
	LCD_ClearSeg(_LCD_6B);
	LCD_ClearSeg(_LCD_6C);
}
/******************************************************************************************************
 * ��       �ƣ�LCD_Displaly()
 * ��       �ܣ����¶�ֵ��ʾ����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ���������Ը��¶ȵĴ���������ֵȼ�����
 * ��       ������
 ******************************************************************************************************/
void LCD_Display()
{
	 if( IntDeg>=0)	  LCD_ClearSeg(_LCD_1G);		//���¶ȣ����������
	      else
	      {
	    	  IntDeg=-IntDeg;						//���¶ȣ���������ֵ����
	    	  LCD_DisplaySeg(_LCD_1G);		//���¶ȣ���Ӹ���
	      }
	      //-----���3λ��ʾ����-----
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,2);
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,3);
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
	      //-----���3λ����ʾ����-----
	      LCD_DisplayDigit(IntDeg/100,2);
	      LCD_DisplayDigit((IntDeg%100)/10,3);
	      LCD_DisplayDigit((IntDeg%100)%10,4);
	      //-----���»��棬������ʾ-----
	      HT1621_Reflash(LCD_Buffer);
}
/******************************************************************************************************
 * ��       �ƣ�ADC10_init(void)
 * ��       �ܣ���ʼ��ADC10�ɼ��ڲ��¶ȴ������������ֶ�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ֱ�Ӵ�CCS��Example����ֲ����
 * ��       ������
 ******************************************************************************************************/
void ADC10_init(void)
{
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10;
      __delay_cycles(30000);
    ADC10CTL0 |= ENC;
}
/******************************************************************************************************
 * ��       �ƣ�ADC10_ISR_HOOK
 * ��       �ܣ�ADCת����ɺ���CPU
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ֱ�Ӵ�CCS��Example����ֲ����
 * ��       ������
 ******************************************************************************************************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	_bic_SR_register_on_exit(LPM4_bits);
}
