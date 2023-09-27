#include  <msp430x14x.h>

void main(void)
{
  /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
  
  WDTCTL = WDTPW + WDTHOLD;                 // �ع�
  //P6DIR |= BIT2;P6OUT |= BIT2;              //�رյ�ƽת��
  P2DIR = 0xff;                             // P2�˿�����Ϊ���
  P2OUT = 0xff;                             // �ر�����LED
  P2SEL |= BIT2 + BIT3 + BIT4;                     // P2.3��P2.4�����ڲ�ģ��
  CCTL0 = OUTMOD_7;
  CCR0 = 4096-1;                            // PWM����Ϊ1S
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = 3072;                              // CCR1 PWM duty cycle
  CCTL2 = OUTMOD_7;                         // CCR2 reset/set
  CCR2 = 1024;                              // CCR2 PWM duty cycle
  TACTL = TASSEL_1 + ID_3 + MC_1;           // ACLK/8, up mode

  _BIS_SR(LPM3_bits);                       // Enter LPM3
}

/*zhongduan
#include  <msp430x14x.h>

void main(void)
{  
    
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
//    P3DIR = 0XFF;P3OUT = 0XFF;
//    P4DIR = 0XFF;P4OUT = 0XFF;
//    P5DIR = 0XFF;P5OUT = 0XFF;
//    P6DIR = 0XFF;P6OUT = 0XFF;
  
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    
//    P6DIR |= BIT2;P6OUT |= BIT2;    //�رյ�ƽת��
    CCTL0 = CCIE;                   //ʹ��CCR0�ж�
    TA0CCTL0;                    //�趨����0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //��ʱ��A��ʱ��Դѡ��ACLK��������ģʽ
    P1DIR = 0xff;                   //����P2�ڷ���Ϊ���
    P1OUT = 0xff;
 
    _EINT();                        //ʹ��ȫ���ж�
    LPM3;                           //CPU����LPM3ģʽ
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{  
    P1OUT ^= 0xff;                  //P2�����ȡ��
}*/