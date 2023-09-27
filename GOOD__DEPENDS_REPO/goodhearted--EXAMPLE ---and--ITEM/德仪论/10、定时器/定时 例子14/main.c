
#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x02;                            // P1.1 output
  P1SEL |= 0x02;                            // P1.1 option select
  CCTL0 = OUTMOD_4;                         // CCR0 toggle mode
  CCR0 = 5;
  TACTL = TASSEL_1 + MC_3;                  // ACLK, up-downmode

  _BIS_SR(LPM3_bits);                       // Enter LPM3
}
