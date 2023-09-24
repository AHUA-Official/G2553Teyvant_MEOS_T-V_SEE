
#include <msp430g2553.h>


#define keyin    (P1IN & 0x0f)
#define uchar unsigned char
#define uint  unsigned int


uchar KeyVal = 0;   // �����ļ�ֵ

void delay(void);

/********************������********************/
void main( void )
{   
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    
    P1IES = 0x0f;               // P1.0~P1.3ѡ���½����ж�
    P1IE = 0x0f;                // ���ж�ʹ��
    P2DIR = (BIT0+BIT1+BIT2+BIT3);
    P2OUT = (BIT0+BIT1+BIT2+BIT3);
    
    _EINT();                    //��ȫ���жϿ���λ
    while(1)
    {
        LPM3;
        P2OUT = KeyVal;
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
/*******************************************
�������ƣ�PORT1_ISR 
��    �ܣ�P1�˿ڵ��жϷ�����
��    ������
����ֵ  ����
********************************************/
#pragma vector=PORT1_VECTOR
__interrupt void  PORT1_ISR(void)
{
    if(P1IFG & 0x0f)
    {
        switch(P1IFG)
        {
         case 0x01:
                    if(keyin == 0x0e)   //����ǵ�һ������������
                    {
                        delay();
                        if(keyin == 0x0e)
                        {
                            //while(keyin != 0x0f);       //�ȴ������ſ�
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x02:
                    if(keyin == 0x0d)   //����ǵڶ�������������
                    {
                        delay();
                        if(keyin == 0x0d)
                        {
                            //while(keyin != 0x0f);       //�ȴ������ſ�
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x04:
                    if(keyin == 0x0b)   //����ǵ���������������
                    {
                        delay();
                        if(keyin == 0x0b)
                        {
                            //while(keyin != 0x0f);       //�ȴ������ſ�
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x08:
                    if(keyin == 0x07)   //����ǵ��ĸ�����������
                    {
                        delay();
                        if(keyin == 0x07)
                        {
                            //while(keyin != 0x0f);       //�ȴ������ſ�
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         default:
                    //while(keyin != 0x0f);       //�ȴ������ſ�
                    //KeyVal = 0;
                    //LPM1_EXIT;
                    P1IFG = 0;
                    return;
        }
    }
}
