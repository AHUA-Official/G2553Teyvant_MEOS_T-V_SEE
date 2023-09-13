#include <msp430.h> 





void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0 | BIT6;                             // P1.0, P1.1output.                            // P1.0, P1.1pullup
  P1OUT |= BIT0;
  P1OUT &= ~BIT0;
  while (1)
  {
    P1OUT ^= BIT0 | BIT6;
    __delay_cycles(1000000);

  }
}

