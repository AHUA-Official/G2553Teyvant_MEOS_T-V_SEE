

/*----------------------------------------------------
����˵�����۲�Һ����ʾ
****************************************************/
#include "msp430g2553.h"

#include  "Cry1602.h"


uchar *s1 = "welcome!";
void int_Clock(void);  

void main( void )
{
    
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    int_Clock();
    LcdReset();
    
    while(1)
    {
     DispStr(4,0,s1);

    }
}
void int_Clock(void)               //ʱ�ӳ�ʼ��
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO��������=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO��������=8MHZ
}