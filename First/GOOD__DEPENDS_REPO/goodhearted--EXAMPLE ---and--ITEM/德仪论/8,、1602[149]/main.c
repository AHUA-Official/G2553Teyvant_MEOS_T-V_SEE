
/***************************************************
�����ܣ���̬��ʾ���֡�welcome��
----------------------------------------------------
����˵�����۲�Һ����ʾ
****************************************************/
#include  <msp430x14x.h>
#include  "Cry1602.h"
#include  "Cry1602.c"

uchar *s1 = "welcome!";

void main( void )
{
  
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;  
  
    
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    LcdReset();
    DispStr(4,1,s1);
//    LocateXY(0,9);                      //ȷ��д���ַ�����ʾλ��
    while(1)
    {
      LPM0;
    }
}
