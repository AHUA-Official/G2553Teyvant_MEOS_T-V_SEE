
#include <msp430g2553.h>

#define keyin (P1IN & 0x08)
#define uchar unsigned char
#define uint  unsigned int

void delay(void);


void main( void )
{
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  
  //P1DIR &= ~BIT3;               //
  
  P1REN |= BIT3;
  P1OUT |= BIT3;  
  
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  
  
  while(1)
  {
    
      if(keyin != 0x08)       //����м�������
        {
            delay();            //��ʱ����
            if(keyin != 0x08)   //�ٴμ�ⰴ��״̬
            {
                while(keyin != 0x08);   //�ȴ��������ſ�
                P1OUT ^= BIT0;                             
            }
        } 
    
    
  }
}


/*******************************************
�������ƣ�delay
��    �ܣ�������������ʱ
��    ������
����ֵ  ����
********************************************/
void delay(void)
{
    uint tmp;
    
    for(tmp = 12000;tmp > 0;tmp--);
}
