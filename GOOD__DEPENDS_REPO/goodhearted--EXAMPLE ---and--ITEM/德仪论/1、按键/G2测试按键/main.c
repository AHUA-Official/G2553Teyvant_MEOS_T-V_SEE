
#include <msp430g2553.h>

#define keyin (P1IN & 0x08)
#define uchar unsigned char
#define uint  unsigned int

void delay(void);


void main( void )
{
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  
  //P1DIR &= ~BIT3;               //
  
  P1REN |= BIT3;
  P1OUT |= BIT3;  
  
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  
  
  while(1)
  {
    
      if(keyin != 0x08)       //如果有键被按下
        {
            delay();            //延时消抖
            if(keyin != 0x08)   //再次检测按键状态
            {
                while(keyin != 0x08);   //等待按键被放开
                P1OUT ^= BIT0;                             
            }
        } 
    
    
  }
}


/*******************************************
函数名称：delay
功    能：用于消抖的延时
参    数：无
返回值  ：无
********************************************/
void delay(void)
{
    uint tmp;
    
    for(tmp = 12000;tmp > 0;tmp--);
}
