
#include <msp430g2553.h>
#define uint   unsigned int 
#define uchar  unsigned char

//������74HC138����ĺ궨��
#define LSA_H     P2OUT |= BIT0
#define LSA_L     P2OUT &= ~ BIT0
#define LSB_H     P2OUT |= BIT1
#define LSB_L     P2OUT &= ~ BIT1
#define LSC_H     P2OUT |= BIT2
#define LSC_L     P2OUT &= ~ BIT2

uchar const LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                        0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71};//��������ܶ�ѡ����(0-F��
uchar DisplayData[8];

void DigDisplay( );


void main(void)
{
        uchar i;
        WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
        P1DIR |= 0XFF;
        
        P2DIR |= BIT0 + BIT1 +BIT2;
	
	while(1)
	{
          for(i=0;i<8;i++)
	    {
	       DisplayData[i]=LedData[i];	
	    }
	    DigDisplay();
	}	
    
}


/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ʹ���������ʾ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void DigDisplay( )
{
	uchar i;
	uint  j;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA_L;LSB_L;LSC_L; break;//��ʾ��0λ
			case(1):
				LSA_H;LSB_L;LSC_L; break;//��ʾ��1λ
			case(2):
				LSA_L;LSB_H;LSC_L; break;//��ʾ��2λ
			case(3):
				LSA_H;LSB_H;LSC_L; break;//��ʾ��3λ
			case(4):
				LSA_L;LSB_L;LSC_H; break;//��ʾ��4λ
			case(5):
				LSA_H;LSB_L;LSC_H; break;//��ʾ��5λ
			case(6):
				LSA_L;LSB_H;LSC_H; break;//��ʾ��6λ
			case(7):
				LSA_H;LSB_H;LSC_H; break;//��ʾ��7λ	
		}
		P1OUT = DisplayData[i];//���Ͷ���
		j=10;						 //ɨ����ʱ���趨
		while(j--);	
		P1OUT = 0x00;//����
	}
}
