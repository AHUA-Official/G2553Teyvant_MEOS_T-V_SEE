
#include "msp430g2553.h"
//#include "intrins.h"

#define uchar unsigned char 
#define uint  unsigned int

#define HC595_Shift_H  P2OUT |= BIT2    //移位时钟     ，595的11脚
#define HC595_Shift_L  P2OUT &= ~BIT2
#define HC595_Store_H  P2OUT |= BIT1    //串行转并行时钟，595的12脚
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
         if(setsat == 1)      HC595_Send(0x7fff,0x0001);    //显示一个点，左下角
         else if(setsat == 0) HC595_Send(0x0000,0xffff);    //显示一个点，右上角
         else if(setsat == 2) HC595_Send(0x00ff,0xff00);    //显示左半上屏
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
		if(temp != 0)  SDI_H;	//从di位到高位                
                else           SDI_L;
                HC595shift();
		dat0 >>= 1;               
	}
	for(i=0;i<16;i++)
	{
                temp = dat1 & 0x0001;
		if(temp != 0)  SDI_H;	//从di位到高位                
                else           SDI_L;
                HC595shift();
		dat1 >>= 1;               
	}
        _NOP();
        _NOP();
       HC595store(); 
}

/***************************************
 字节写入存储寄存器，并且送往输出端口，数据刷新输出
***************************************/
void HC595store(void) 
{
   HC595_Store_L;   //先将存储时钟置低                  
   HC595_Store_H;   //将存储时钟置高产生一个上升沿，就会把一个字节
                    //写入存储寄存器，即将74HC595 的数据刷新输出。
} 
/**************************************
           寄存器移位函数
**************************************/
void HC595shift(void) 
{
   HC595_Shift_L;  //要将时钟线置低                 
   HC595_Shift_H;  //把时钟线拉高，则是上升沿，就会把进行寄存器移位操作
}

//shizhong初始化
void clock_init( )
{
      DCOCTL = 0;
      BCSCTL1 = CALBC1_1MHZ;  //1MHZ
      DCOCTL = CALDCO_1MHZ;
      BCSCTL2 = DIVM_3;      //主时钟八分频
}

  


// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    uchar j = 0;
//    CCR0 += 50000;                            // Add Offset to CCR0
    j++;
    if(j==20)  //延时一秒
    {
      j = 0;
      setsat++;
    }
    if(setsat == 3)
      {
        setsat = 0;
      }
    
}


