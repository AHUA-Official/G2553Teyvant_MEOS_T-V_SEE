//软件延时
/*#include "msp430x14x.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

//数码管7位段码：0--f
uchar seg7[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                      0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void ms_delay( );

void main( void )
{
  
  uchar i;
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P3DIR = 0xff;
  P3OUT = 0xff;
  
  while(1)
  {
      for(i=0;i<16;i++)
      {
         P3OUT = ~seg7[i];
         ms_delay( );
         ms_delay( );
         ms_delay( );
         ms_delay( );
      }
  }  
}


//yanshihanshu

void ms_delay( )
{
    uint m;
    
    for(m=0;m<48000;m++)
      ;
   
}*/


//定时器延时
#include "msp430x14x.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

//数码管7位段码：0--f
uchar seg7[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                      0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar i;

void main(void)
{
   i = 0;
   WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
   P2DIR = 0xff;
   P2OUT = 0xff;
   CCTL0 = CCIE;
   CCR0 = 4096;
   TACTL = TASSEL_1 + ID_3 + MC_1;                  // ACLK, upmode
   
   _BIS_SR(LPM3_bits + GIE);                 // Enter LPM0 w/ interrupt
  
   
   
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
    
    i++;
    if(i == 16)
      i = 0;
      P2OUT = ~seg7[i];    
}