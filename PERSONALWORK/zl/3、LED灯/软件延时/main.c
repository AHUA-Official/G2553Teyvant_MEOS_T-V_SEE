/*����������򵥵���ʱ����*/
/*����Ĭ��Ϊʱ��Ƶ��Ϊ800KHz*/
#include <msp430x14x.h>

void delay(int z)
{
 int x,y;
  for(x=z;x>0;x--)
     for(y=110;y>0;y--);
}
/*��һ������ȫ��*/
/*
void main()
{

 WDTCTL=WDTPW+WDTHOLD; //�رտ��Ź�
//WDTPW=Watchdog timer password. Always read as 069h. 
//Must be written as 05Ah, ora PUC will be generated.
//Watchdog timer hold. This bit stops the watchdog timer. 
//Setting WDTHOLD= 1 when the WDT is not in use conserves power.
//0 Watchdog timer is not stopped,1 Watchdog timer is stopped

 P2DIR=0XFF;
 while(1)
 {
  //P2OUT=0X00;
 P2OUT=0X55; 
  delay(500);
 P2OUT=0Xaa;
delay(500);
 }
}*/

/*�ڶ�������һ������*/

/*
void main()
{
 WDTCTL=WDTPW+WDTHOLD;
 P2DIR|=BIT0;
  P2OUT&=0XFE;//P2OUT=0XFE;
  
  P2DIR|=BIT3;//���趨������Ϊ���
  P2OUT&=~BIT3;//���趨����߻��
 //עҪ��ĳһ��IO�������� �����������ݱ���ͬʱ��
  
 
}
*/
/*������������ȡ���õ���*/

void main()
{

 WDTCTL=WDTPW+WDTHOLD; 
 P2DIR|=BIT6;//���趨������Ϊ���
 while(1)
 { 
  P2OUT^=BIT6;//���趨����߻��
  delay(500);
 }
}

