
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

unsigned char DIG_CODE[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                            0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void HC595_Send(uchar dat0);
void HC595store(void);
void HC595shift(void);


void main(void)
{   
  
      WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer

      P2DIR = BIT0+BIT1+BIT2;
      P2OUT = BIT0+BIT1+BIT2;
      
     
          HC595_Send(~DIG_CODE[1]);
          HC595_Send(~DIG_CODE[2]);
          HC595_Send(~DIG_CODE[3]);
          HC595_Send(~DIG_CODE[4]);
          HC595_Send(~DIG_CODE[5]);
          HC595_Send(~DIG_CODE[6]);
          HC595_Send(~DIG_CODE[7]);
          HC595_Send(~DIG_CODE[8]);
          __delay_cycles(1000000);       
      
        while(1);
}


void HC595_Send(uchar dat0)
{
	uchar i,temp;
	HC595_Store_H;
	HC595_Shift_H;
	for(i=0;i<8;i++)
	{
                temp = dat0 & 0x80;
		if(temp != 0)  SDI_H;	//从di位到高位                
                else           SDI_L;
                HC595shift();
		dat0 <<= 1;               
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

