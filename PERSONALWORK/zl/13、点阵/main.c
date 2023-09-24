
#include "msp430g2553.h"
//#include "intrins.h"

#define uchar unsigned char 
#define uint  unsigned int

#define HC595_Shift_H  P2OUT |= BIT2    //��λʱ��     ��595��11��
#define HC595_Shift_L  P2OUT &= ~BIT2
#define HC595_Store_H  P2OUT |= BIT1    //����ת����ʱ�ӣ�595��12��
#define HC595_Store_L  P2OUT &= ~BIT1
#define SDI_H          P2OUT |= BIT0
#define SDI_L          P2OUT &= ~BIT0

uchar setsat = 0;

void HC595_Send(uint dat0,uint dat1);
void HC595store(void);
void HC595shift(void);
void clock_init(void);


void main(void)
{
      WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
      clock_init( );
      
      TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
      TA0CCR0 = 5000;
      TACTL = TASSEL_2 + MC_1;                  // SMCLK, contmode  
      
      P2DIR = BIT0+BIT1+BIT2;
      P2OUT = BIT0+BIT1+BIT2;
      _EINT();
    
      while(1)
      {
         if(setsat == 1)      HC595_Send(0x7fff,0x0001);    //��ʾһ���㣬���½�
         else if(setsat == 0) HC595_Send(0x0000,0xffff);    //��ʾһ���㣬���Ͻ�
         else if(setsat == 2) HC595_Send(0x00ff,0xff00);    //��ʾ�������
      }
}


void HC595_Send(uint dat0,uint dat1)
{
	uchar i,temp;
	HC595_Store_H;
	HC595_Shift_H;
	for(i=0;i<16;i++)
	{
                temp = dat0 & 0x0001;
		if(temp != 0)  SDI_H;	//��diλ����λ                
                else           SDI_L;
                HC595shift();
		dat0 >>= 1;               
	}
	for(i=0;i<16;i++)
	{
                temp = dat1 & 0x0001;
		if(temp != 0)  SDI_H;	//��diλ����λ                
                else           SDI_L;
                HC595shift();
		dat1 >>= 1;               
	}
        _NOP();
        _NOP();
       HC595store(); 
}

/***************************************
 �ֽ�д��洢�Ĵ�����������������˿ڣ�����ˢ�����
***************************************/
void HC595store(void) 
{
   HC595_Store_L;   //�Ƚ��洢ʱ���õ�                  
   HC595_Store_H;   //���洢ʱ���ø߲���һ�������أ��ͻ��һ���ֽ�
                    //д��洢�Ĵ���������74HC595 ������ˢ�������
} 
/**************************************
           �Ĵ�����λ����
**************************************/
void HC595shift(void) 
{
   HC595_Shift_L;  //Ҫ��ʱ�����õ�                 
   HC595_Shift_H;  //��ʱ�������ߣ����������أ��ͻ�ѽ��мĴ�����λ����
}

//shizhong��ʼ��
void clock_init( )
{
      DCOCTL = 0;
      BCSCTL1 = CALBC1_1MHZ;  //1MHZ
      DCOCTL = CALDCO_1MHZ;
      BCSCTL2 = DIVM_3;      //��ʱ�Ӱ˷�Ƶ
}

  


// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    uchar j = 0;
//    CCR0 += 50000;                            // Add Offset to CCR0
    j++;
    if(j==20)  //��ʱһ��
    {
      j = 0;
      setsat++;
    }
    if(setsat == 3)
      {
        setsat = 0;
      }
    
}


