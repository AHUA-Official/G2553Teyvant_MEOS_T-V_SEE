/******************************************************
程序功能：PC通过串口调试精灵向MCU发送数据，MCU将其在1602
          液晶上显示
-------------------------------------------------------
通信格式：N.8.1,  9600
------------------------------------------------------
测试说明：打开串口调试助手，正确设置通信格式，向从PC机上
          向学习板发送数据，观察液晶上显示的字符。
******************************************************/
#include  <msp430.h>
#include  "cry1602.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

void InitUART(void);
void PutString(uchar *ptr);
/***************主函数************/
void main( void )
{
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    uchar pX=0,pY=0;
    uchar *tishi = "Send data to MCU, and they will be displayed on Cry1602!";
    
    WDTCTL = WDTPW + WDTHOLD;     //关狗
    P6DIR |= BIT2;P6OUT |= BIT2;  //关闭电平转换
    
    InitUART();                   //初始化UART
    LcdReset();                   //初始化LCD
    
    PutString(tishi);
    while(1)
    {
        if(IFG1 & URXIFG0)      //如果收到字符
        {
            Disp1Char(pX++,pY,U0RXBUF);
            if(pX == 16)
            {
                pX  = 0;
                pY ^= 1;
            }
        }
    }
    
}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // TX缓存空闲？
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
