/******************************************************
程序功能：接收来自PC机的字符，然后重新发送给PC机
-------------------------------------------------------
通信格式：N.8.1,  9600
         无校验，8个数据位，1个停止位，波特率9600
------------------------------------------------------
测试说明：打开串口调试助手，正确设置通信格式，向学习板
          发送一个字符，观察是否收到回发的字符，以及是否
          正确
*******************************************************/
#include  <msp430x14x.h>

/********************主函数********************/
void main(void)
{
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    WDTCTL = WDTPW + WDTHOLD;                 // 关闭看门狗
    P6DIR |= BIT2;P6OUT |= BIT2;              //关闭电平转换
    
    P3SEL |= 0x30;                            // 选择P3.4和P3.5做UART通信端口
    ME1 |= UTXE0 + URXE0;                     // 使能USART0的发送和接受
    UCTL0 |= CHAR;                            // 选择8位字符
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 波特率9600
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // 初始化UART状态机
    IE1 |= URXIE0;                            // 使能USART0的接收中断

    while(1)
    {
        _EINT();                               //打开全局中断
        LPM1;                                  //进入LPM1模式
        while (!(IFG1 & UTXIFG0));             //等待以前的字符发送完毕
        TXBUF0 = RXBUF0;                       //将收到的字符发送出去 
    }
}
/*******************************************
函数名称：UART0_RXISR
功    能：UART0的接收中断服务函数，在这里唤醒
          CPU，使它退出低功耗模式
参    数：无
返回值  ：无
********************************************/
#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
  LPM1_EXIT;                 //退出低功耗模式
}
