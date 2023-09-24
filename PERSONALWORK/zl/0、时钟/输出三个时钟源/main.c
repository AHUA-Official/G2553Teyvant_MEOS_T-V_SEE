
/*#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
  DCOCTL = DCO0 + DCO1 + DCO2;              // Max DCO
  BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
  BCSCTL2 |= SELS;                          // SMCLK = XT2
  P5DIR |= 0x70;                            // P5.6,5,4 outputs
  P5SEL |= 0x70;                            // P5.6,5,5 options

  while(1)
  {
  }
}*/

#include  <msp430g2553.h>

void main(void)
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
  DCOCTL = 0;
  BCSCTL1 |= 0x8f;  //MHZ   Ê±ÖÓ²»ÄÜÌ«µÍ
  DCOCTL  |= 0xe0;
  
  P1DIR |= 0x13;                            // P1.0,1 and P1.4 outputs
  P1SEL |= 0x11;                            // P1.0,4 ACLK, SMCLK output

  while(1)
  {
    P1OUT |= 0x02;    	                    // P1.1 = 1
    P1OUT &= ~0x02;                         // P1.1 = 0
  }
}



