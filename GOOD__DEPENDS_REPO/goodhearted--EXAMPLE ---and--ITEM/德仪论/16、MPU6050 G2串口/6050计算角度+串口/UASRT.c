


#include "UASRT.h"


//���ڳ�ʼ��
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
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ������
����ֵ  ����
********************************************/
void PutString(char *ptr)
{
     while(*ptr != '\0')
      {
            while (!(IFG2&UCA0TXIFG));                // TX������У�
            UCA0TXBUF = *ptr++;                       // ��������
      }
      while (!(IFG2&UCA0TXIFG));
      UCA0TXBUF = '\n';
}

/*******************************************
�������ƣ�PutChar
��    �ܣ���PC������һ���ַ���Ӧ��ASCII��
��    ����zifu--���͵��ַ�
����ֵ  ����
********************************************/
void PutChar(unsigned char zifu)
{
      while (!(IFG2&UCA0TXIFG));

       UCA0TXBUF =  zifu; 
     
}


/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
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