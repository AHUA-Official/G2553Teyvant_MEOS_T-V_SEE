


#include "UASRT.h"


//串口初始化
void UARST_int()
{
  P1SEL = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

}


/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：无
返回值  ：无
********************************************/
void PutString(char *ptr)
{
     while(*ptr != '\0')
      {
            while (!(IFG2&UCA0TXIFG));                // TX缓存空闲？
            UCA0TXBUF = *ptr++;                       // 发送数据
      }
      while (!(IFG2&UCA0TXIFG));
      UCA0TXBUF = '\n';
}

/*******************************************
函数名称：PutChar
功    能：向PC机发送一个字符对应的ASCII码
参    数：zifu--发送的字符
返回值  ：无
********************************************/
void PutChar(unsigned char zifu)
{
      while (!(IFG2&UCA0TXIFG));

       UCA0TXBUF =  zifu; 
     
}


/*******************************************
函数名称：Delays
功    能：延时一会
参    数：无
返回值  ：无
********************************************/
void Delays(void)
{
    unsigned char i=20;
    unsigned int j;

    while(i--)
    {
            j=2000;
            while(j--);
    }
}