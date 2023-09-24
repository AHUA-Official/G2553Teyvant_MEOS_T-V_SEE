
#include <msp430g2553.h>
#include "HT1621.H"
#include "ADC10.h"



void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=8MHZ
}
void int_WDT(void)                 //���Ź�����
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}

void init_devices(void)
{
   port_init();                     //IO������
   int_Clock();                     //ʱ������
   int_WDT();                       //���Ź�����
}

/******************************************************** 
�������ƣ�main() 
��������: ������
********************************************************/
void main() 
{ 
  
  init_devices( );                       // ��ʼ������
  adc10_init( );
  Ht1621_Init( );                        //�ϵ��ʼ��LCD 
                                        //��ʱһ��ʱ�� 
  Ht1621WrAllData(0,Ht1621Tab,16);      //���1621�Ĵ������ݣ�������
  _BIS_SR(GIE);
  while(1)
  {
     display_adc10( );
  }
      
} 


