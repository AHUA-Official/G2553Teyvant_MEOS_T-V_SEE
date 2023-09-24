
#include <msp430g2553.h>
#define uint   unsigned int 
#define uchar  unsigned char

//以下是74HC138所需的宏定义
#define LSA_H     P2OUT |= BIT0
#define LSA_L     P2OUT &= ~ BIT0
#define LSB_H     P2OUT |= BIT1
#define LSB_L     P2OUT &= ~ BIT1
#define LSC_H     P2OUT |= BIT2
#define LSC_L     P2OUT &= ~ BIT2

uchar const LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                        0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71};//共阴数码管段选数组(0-F）
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
* 函 数 名         : DigDisplay
* 函数功能		   : 使用数码管显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void DigDisplay( )
{
	uchar i;
	uint  j;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA_L;LSB_L;LSC_L; break;//显示第0位
			case(1):
				LSA_H;LSB_L;LSC_L; break;//显示第1位
			case(2):
				LSA_L;LSB_H;LSC_L; break;//显示第2位
			case(3):
				LSA_H;LSB_H;LSC_L; break;//显示第3位
			case(4):
				LSA_L;LSB_L;LSC_H; break;//显示第4位
			case(5):
				LSA_H;LSB_L;LSC_H; break;//显示第5位
			case(6):
				LSA_L;LSB_H;LSC_H; break;//显示第6位
			case(7):
				LSA_H;LSB_H;LSC_H; break;//显示第7位	
		}
		P1OUT = DisplayData[i];//发送段码
		j=10;						 //扫描间隔时间设定
		while(j--);	
		P1OUT = 0x00;//消隐
	}
}
