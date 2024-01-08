/****************************************************
�����ܣ�MCUͨ��������PC������4X4���̵ļ�ֵ
-----------------------------------------------------
ͨ�Ÿ�ʽ��N.8.1,  9600
------------------------------------------------------
����˵�����򿪴��ڵ������֣���ȷ����ͨ�Ÿ�ʽ������4X4
          ���̹۲���Ļ��ʾ�İ�����ֵ��
****************************************************/
#include  <msp430x14x.h>
#include  "keypad.h"
#include  "keypad.C"

//�����ⲿ����������
extern unsigned char key_Pressed;      
extern unsigned char key_val;         
extern unsigned char key_Flag; 

void PutString(uchar *ptr);
void PutChar(uchar zifu);
/*******************������*******************/
void main(void)
{
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    uchar *tishi = "This Key's ID is:";
    P6DIR |= BIT2;P6OUT |= BIT2;              //�رյ�ƽת��
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= UTXE0;                             // Enable USART0 TXD/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    
    Init_Keypad();                            //��ʼ�����̶˿�
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
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
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
�������ƣ�PutChar
��    �ܣ���PC������һ���ַ���Ӧ��ASCII��
��    ����zifu--���͵��ַ�
����ֵ  ����
********************************************/
void PutChar(uchar zifu)
{
      while (!(IFG1 & UTXIFG0));
      if(zifu > 9)            //���ͼ�ֵ1~16��Ӧ��ASCII��
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
      TXBUF0 = '\n';          //���ͻس��ַ�
}


