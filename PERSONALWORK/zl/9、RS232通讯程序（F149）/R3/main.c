/******************************************************
�����ܣ�PCͨ�����ڵ��Ծ�����MCU�������ݣ�MCU������1602
          Һ������ʾ
-------------------------------------------------------
ͨ�Ÿ�ʽ��N.8.1,  9600
------------------------------------------------------
����˵�����򿪴��ڵ������֣���ȷ����ͨ�Ÿ�ʽ�����PC����
          ��ѧϰ�巢�����ݣ��۲�Һ������ʾ���ַ���
******************************************************/
#include  <msp430.h>
#include  "cry1602.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

void InitUART(void);
void PutString(uchar *ptr);
/***************������************/
void main( void )
{
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    uchar pX=0,pY=0;
    uchar *tishi = "Send data to MCU, and they will be displayed on Cry1602!";
    
    WDTCTL = WDTPW + WDTHOLD;     //�ع�
    P6DIR |= BIT2;P6OUT |= BIT2;  //�رյ�ƽת��
    
    InitUART();                   //��ʼ��UART
    LcdReset();                   //��ʼ��LCD
    
    PutString(tishi);
    while(1)
    {
        if(IFG1 & URXIFG0)      //����յ��ַ�
        {
            Disp1Char(pX++,pY,U0RXBUF);
            if(pX == 16)
            {
                pX  = 0;
                pY ^= 1;
            }
        }
    }
    
}
/*******************************************
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
����ֵ  ����
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // TX������У�
            TXBUF0 = *ptr++;                       // ��������
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
�������ƣ�InitUART
��    �ܣ���ʼ��UART�˿�
��    ������
����ֵ  ����
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
