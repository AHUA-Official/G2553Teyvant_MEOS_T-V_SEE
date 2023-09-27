#include <msp430g2553.h> 
#define uchar unsigned char
#define uint unsigned int
void main() 
{   
  WDTCTL=WDTPW+WDTHOLD;  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL =CALDCO_1MHZ;  
  P1SEL |=BIT1+BIT2;  
  P1SEL2 |=BIT1+BIT2; 
  UCA0CTL1 |= UCSWRST;
  UCA0CTL1 |=UCSSEL_2;  
  UCA0BR0=104;  
  UCA0BR1=0;    
  UCA0MCTL =UCBRS0; 
  UCA0CTL1 &= ~UCSWRST;
  IE2 |=UCA0RXIE;  
  __bis_SR_register(LPM0_bits+GIE);
} 
#pragma vector=USCIAB0RX_VECTOR 
__interrupt void uart()
{ 
  while(!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = UCA0RXBUF; 
}