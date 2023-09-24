/*本程序用最简单的延时函数*/
/*开机默认为时钟频率为800KHz*/
#include <msp430x14x.h>

void delay(int z)
{
 int x,y;
  for(x=z;x>0;x--)
     for(y=110;y>0;y--);
}
/*第一个程序全亮*/
/*
void main()
{

 WDTCTL=WDTPW+WDTHOLD; //关闭看门狗
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

/*第二个程序一个灯亮*/

/*
void main()
{
 WDTCTL=WDTPW+WDTHOLD;
 P2DIR|=BIT0;
  P2OUT&=0XFE;//P2OUT=0XFE;
  
  P2DIR|=BIT3;//先设定第三脚为输出
  P2OUT&=~BIT3;//再设定输出高或低
 //注要对某一个IO单独操作 输出方向和数据必须同时有
  
 
}
*/
/*第三个程序用取反让灯闪*/

void main()
{

 WDTCTL=WDTPW+WDTHOLD; 
 P2DIR|=BIT6;//先设定第三脚为输出
 while(1)
 { 
  P2OUT^=BIT6;//再设定输出高或低
  delay(500);
 }
}

