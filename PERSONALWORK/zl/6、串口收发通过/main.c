
#include "msp430G2553.h"
//#include "stdio.h"
void main(void)
{
WDTCTL = WDTPW + WDTHOLD;           //停止看门狗
UCA0CTL1 |= UCSWRST;                // USCI_A0 进入软件复位状态
UCA0CTL1 |= UCSSEL_2;                 //时钟源选择 SMCLK
BCSCTL1 = CALBC1_1MHZ;                //设置 DCO 频率为 1MHz
DCOCTL = CALDCO_1MHZ;
P1SEL = BIT1 + BIT2 ;                         // P1.1 = RXD, P1.2=TXD
P1SEL2 = BIT1 + BIT2 ;                       // P1.1 = RXD, P1.2=TXD
P1DIR |= BIT0;
UCA0BR0 = 0x68;                         //时钟源 1MHz 时波特率为9600
UCA0BR1 = 0x00;                        //时钟源 1MHz 时波特率为9600
UCA0MCTL = UCBRS0;                    //小数分频器
UCA0CTL1 &= ~UCSWRST;                 //初始化 USCI_A0 状态机
IE2 |= UCA0RXIE;                          //使能 USCI_A0 接收中断
//IE2 |= UCA0TXIE;     //外加  发送中断
UCA0IRRCTL=UCIRRXFE;
_EINT();                          //开总中断
while(1)
{
}
}

#pragma vector = USCIAB0RX_VECTOR         //接收中断
__interrupt void USCI0RX_ISR(void)
{
while ( !(IFG2&UCA0TXIFG) );                //确保发送缓冲区准备好
//( !(IFG2&UCA0RXIFG) );            //接收缓冲
P1OUT ^= BIT0;                            //接收指示灯状态改变
UCA0TXBUF = UCA0RXBUF;        //发送接收到的数据
}

