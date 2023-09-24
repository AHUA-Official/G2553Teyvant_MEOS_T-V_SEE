
#include  <msp430g2553.h>


void ms_delay(int z);

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
   
   P1DIR |= BIT0+BIT6;
   
   P1OUT |= BIT0+BIT6;
   
   while(1)
   {
       P1OUT ^= BIT0;
       ms_delay(500);
           
   }  
}

void ms_delay(int z)
{
 int x,y;
  for(x=z;x>0;x--)
     for(y=110;y>0;y--);
}
