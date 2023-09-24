/*****************************************************
�����ܣ�MCU��ͣ��PC����������,����Ļ����ʾ0~127��Ӧ
          ��ASCII�ַ�
------------------------------------------------------
ͨ�Ÿ�ʽ��N.8.1, 2400
------------------------------------------------------
����˵�����򿪴��ڵ��Ծ��飬��ȷ����ͨ�Ÿ�ʽ���۲���Ļ
******************************************************/

#include  <msp430x14x.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

void Delays(void);
void PutString(uchar *ptr);

/********************������**********************/
void main(void)
{
    uchar *tishi = " MCU sends 0~127 to PC and the\
                    \n screen will display their corresponding\
                    \n ASCII code as follows:";
    uchar value = 0;
  
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    WDTCTL = WDTPW + WDTHOLD;                 // �ع�
    P6DIR |= BIT2;P6OUT |= BIT2;              // �ص�ƽת��
    P3SEL |= 0x30;                            // P3.4,5ѡ��ΪUART�շ��˿�
    ME1 |= UTXE0 + URXE0;                     // ʹ��USART0�շ�
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x0D;                             // 32k/2400 - 13.65
    UBR10 = 0x00;                             //
    UMCTL0 = 0x6B;                            // Modulation
    UCTL0 &= ~SWRST;                          // ��ʼ��UART0״̬��
    IE1 |= URXIE0;                            // ʹ�ܽ����ж�
    _EINT();
    PutString(tishi);
    while(1)
    {     
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = value++;
        value &= 0x7f;                        // ��֤value����ֵС��128
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = '\n';
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
            while (!(IFG1 & UTXIFG0));                // TX������У�
            TXBUF0 = *ptr++;                       // ��������
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
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
