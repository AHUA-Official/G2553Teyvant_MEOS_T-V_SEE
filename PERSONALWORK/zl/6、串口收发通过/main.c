
#include "msp430G2553.h"
//#include "stdio.h"
void main(void)
{
WDTCTL = WDTPW + WDTHOLD;           //ֹͣ���Ź�
UCA0CTL1 |= UCSWRST;                // USCI_A0 ���������λ״̬
UCA0CTL1 |= UCSSEL_2;                 //ʱ��Դѡ�� SMCLK
BCSCTL1 = CALBC1_1MHZ;                //���� DCO Ƶ��Ϊ 1MHz
DCOCTL = CALDCO_1MHZ;
P1SEL = BIT1 + BIT2 ;                         // P1.1 = RXD, P1.2=TXD
P1SEL2 = BIT1 + BIT2 ;                       // P1.1 = RXD, P1.2=TXD
P1DIR |= BIT0;
UCA0BR0 = 0x68;                         //ʱ��Դ 1MHz ʱ������Ϊ9600
UCA0BR1 = 0x00;                        //ʱ��Դ 1MHz ʱ������Ϊ9600
UCA0MCTL = UCBRS0;                    //С����Ƶ��
UCA0CTL1 &= ~UCSWRST;                 //��ʼ�� USCI_A0 ״̬��
IE2 |= UCA0RXIE;                          //ʹ�� USCI_A0 �����ж�
//IE2 |= UCA0TXIE;     //���  �����ж�
UCA0IRRCTL=UCIRRXFE;
_EINT();                          //�����ж�
while(1)
{
}
}

#pragma vector = USCIAB0RX_VECTOR         //�����ж�
__interrupt void USCI0RX_ISR(void)
{
while ( !(IFG2&UCA0TXIFG) );                //ȷ�����ͻ�����׼����
//( !(IFG2&UCA0RXIFG) );            //���ջ���
P1OUT ^= BIT0;                            //����ָʾ��״̬�ı�
UCA0TXBUF = UCA0RXBUF;        //���ͽ��յ�������
}

