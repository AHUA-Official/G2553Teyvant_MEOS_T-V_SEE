
#include  "msp430g2553.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

uchar *tishi = "��������";

void Delays(void);
void PutString(uchar *ptr);

void main(void)
{
  
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;                     //DCO = 1MHz
  
  P1SEL = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  _EINT();
  
     while(1)
    {   
     
      PutString(tishi);
      Delays();
    }

}

/*******************************************
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ������
����ֵ  ����
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG2&UCA0TXIFG));                // TX������У�
            UCA0TXBUF = *ptr++;                       // ��������
      }
      while (!(IFG2&UCA0TXIFG));
      UCA0TXBUF = '\n';
}

//  Echo back RXed character, confirm TX buffer is ready first
/*#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = 0x12;                    // TX -> RXed character
}*/


/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
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
