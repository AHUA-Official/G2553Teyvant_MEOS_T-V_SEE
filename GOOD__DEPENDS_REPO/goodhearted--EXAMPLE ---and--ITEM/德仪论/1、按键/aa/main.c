
#include  <msp430g2553.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0x01;                            // Set P1.0 to output direction
  P1OUT |= 0x00;
  while (1)                                 // Test P1.4
  {
     P1OUT |= 0x01;       // if P1.4 set, set P1.0
     P1OUT &= ~0x01;                    // else reset
  }
}
