
#include  "msp430g2553.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= BIT1 + BIT2;                      
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 8;                              // 1MHz 115200
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{

  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
  
}
