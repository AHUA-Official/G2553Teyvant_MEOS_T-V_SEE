#include  <msp430x44x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  while(1)
  {
     P1OUT |= 0x01;
  }
  
}
