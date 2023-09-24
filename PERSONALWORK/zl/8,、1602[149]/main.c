
/***************************************************
程序功能：动态显示文字“welcome”
----------------------------------------------------
测试说明：观察液晶显示
****************************************************/
#include  <msp430x14x.h>
#include  "Cry1602.h"
#include  "Cry1602.c"

uchar *s1 = "welcome!";

void main( void )
{
  
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;  
  
    
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    LcdReset();
    DispStr(4,1,s1);
//    LocateXY(0,9);                      //确定写入字符的显示位置
    while(1)
    {
      LPM0;
    }
}
