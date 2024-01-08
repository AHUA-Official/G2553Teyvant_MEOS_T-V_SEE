/****************************************************
程序功能：MCU通过串口向PC机发送4X4键盘的键值
-----------------------------------------------------
通信格式：N.8.1,  9600
------------------------------------------------------
测试说明：打开串口调试助手，正确设置通信格式，按动4X4
          键盘观察屏幕显示的按键键值。
****************************************************/
#include  <msp430x14x.h>
#include  "keypad.h"
#include  "keypad.C"

//引用外部变量的声明
extern unsigned char key_Pressed;      
extern unsigned char key_val;         
extern unsigned char key_Flag; 

void PutString(uchar *ptr);
void PutChar(uchar zifu);
/*******************主函数*******************/
void main(void)
{
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    uchar *tishi = "This Key's ID is:";
    P6DIR |= BIT2;P6OUT |= BIT2;              //关闭电平转换
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= UTXE0;                             // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    
    Init_Keypad();                            //初始化键盘端口
    while(1)
    {
        Key_Event();
    
        if(key_Flag == 1)
        {
            key_Flag = 0;
            PutString(tishi);
            PutChar(key_val);
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
            while (!(IFG1 & UTXIFG0));                // TX缓存空闲？
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
函数名称：PutChar
功    能：向PC机发送一个字符对应的ASCII码
参    数：zifu--发送的字符
返回值  ：无
********************************************/
void PutChar(uchar zifu)
{
      while (!(IFG1 & UTXIFG0));
      if(zifu > 9)            //发送键值1~16对应的ASCII码
      {
          TXBUF0 = 0x30 + zifu/10; 
          while (!(IFG1 & UTXIFG0));
          TXBUF0 = 0x30 + zifu%10;
      }
      else
      {
          TXBUF0 = 0x30 + zifu; 
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';          //发送回车字符
}


