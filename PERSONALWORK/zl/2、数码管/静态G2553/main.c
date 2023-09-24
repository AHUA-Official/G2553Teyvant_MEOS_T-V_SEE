
#include "msp430g2553.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

//ÊýÂë¹Ü7Î»¶ÎÂë£º0--f
uchar seg7[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                      0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void ms_delay( );

void main( void )
{
  
  uchar i;
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P2DIR = 0xff;
  P2OUT = 0xff;
  
  while(1)
  {
      for(i=0;i<16;i++)
      {
         P2OUT = ~seg7[i];
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
   
}