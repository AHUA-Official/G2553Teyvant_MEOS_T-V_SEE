/*****************************************************
程序功能：MCU不停向PC机发送数据,在屏幕上显示0~127对应
          的ASCII字符
------------------------------------------------------
通信格式：N.8.1, 2400
------------------------------------------------------
测试说明：打开串口调试精灵，正确设置通信格式，观察屏幕
******************************************************/

#include  <msp430x14x.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

void Delays(void);
void PutString(uchar *ptr);

/********************主函数**********************/
void main(void)
{
    uchar *tishi = " MCU sends 0~127 to PC and the\
                    \n screen will display their corresponding\
                    \n ASCII code as follows:";
    uchar value = 0;
  
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    WDTCTL = WDTPW + WDTHOLD;                 // 关狗
    P6DIR |= BIT2;P6OUT |= BIT2;              // 关电平转换
    P3SEL |= 0x30;                            // P3.4,5选择为UART收发端口
    ME1 |= UTXE0 + URXE0;                     // 使能USART0收发
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x0D;                             // 32k/2400 - 13.65
    UBR10 = 0x00;                             //
    UMCTL0 = 0x6B;                            // Modulation
    UCTL0 &= ~SWRST;                          // 初始化UART0状态机
    IE1 |= URXIE0;                            // 使能接收中断
    _EINT();
    PutString(tishi);
    while(1)
    {     
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = value++;
        value &= 0x7f;                        // 保证value的数值小于128
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = '\n';
        Delays();
    }
}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：无
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));                // TX缓存空闲？
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
函数名称：Delays
功    能：延时一会
参    数：无
返回值  ：无
********************************************/
void Delays(void)
{
    uchar i=20;
    uint j;

    while(i--)
    {
            j=2000;
            while(j--);
    }
}
