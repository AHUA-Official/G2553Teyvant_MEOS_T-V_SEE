
#include <msp430x14x.h>

#define keyin    (P1IN & 0x0f)
#define uchar unsigned char
#define uint  unsigned int

void delay(void);


void main( void )
{
  
  uchar temp;
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  //P1DIR = ~(BIT0+BIT1+BIT2+BIT3);               //����P1.0~P.3Ϊ����״̬��P.7Ϊ���
  P2DIR = (BIT0+BIT1+BIT2+BIT3);
  P2OUT = (BIT0+BIT1+BIT2+BIT3);
  
  
  while(1)
  {
    
      if(keyin != 0x0f)       //����м�������
        {
            delay();            //��ʱ����
            if(keyin != 0x0f)   //�ٴμ�ⰴ��״̬
            {
                temp=keyin;
                while(keyin != 0x0f);   //�ȴ��������ſ�
          /*    switch(temp)    //ת����ֵ    
                {
                case 0x0e:
                            keyval = 1;break;
                case 0x0d:
                            keyval = 2;break;
                case 0x0b:
                            keyval = 3;break;
                case 0x07:
                            keyval = 4;break;
                default:
                            keyval = 0;break;
                }*/
                P2OUT = temp;
                
                
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
