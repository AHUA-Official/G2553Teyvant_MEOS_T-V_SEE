


#include  <msp430x14x.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1SEL |= 0x0E;                            // P1.1 - P1.4 option select
  P1DIR |= 0x0F;                            // P1.0 - P1.4 outputs
  P2DIR |= 0x0F; 
  CCR0  = 5000;
  CCTL0 = OUTMOD_4 + CCIE;                  // CCR0 toggle, interrupt enabled
  CCR1  = 10000; 
  CCTL1 = OUTMOD_4 + CCIE;                  // CCR1 toggle, interrupt enabled
  CCR2  = 50000;
  CCTL2 = OUTMOD_4 + CCIE;                  // CCR2 toggle, interrupt enabled
  TACTL = TASSEL_2 + MC_2 + ID_3 + TAIE;          // ACLK, Contmode, int enabled
                                                   //8·ÖÆµ
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
  CCR0 += 1000;                              // Add Offset to CCR0
  P2OUT ^= 0x02;
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
  switch( TAIV )
  {
  case  2: CCR1 += 5000;                    // Add Offset to CCR1
           P2OUT ^= 0x04;
           break;
  case  4: CCR2 += 50000;                   // Add Offset to CCR2
           P2OUT ^= 0x08;
           break;
  case 10: P2OUT ^= 0x01;                   // Timer_A3 overflow
           break;
 }
}

//  As coded with TACLK ~800kHz DCO, toggle rates are:
//  P1.1= CCR0 = 800kHz/(2*200) ~2000Hz
//  P1.2= CCR1 = 800kHz/(2*1000) ~400Hz
//  P1.3= CCR2 = 800kHz/(2*10000) ~40Hz
//  P1.0= overflow = 800kHz/(2*65536) ~6Hz
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.1/TA0|--> CCR0
//           |         P1.2/TA1|--> CCR1
//           |         P1.3/TA2|--> CCR2
//           |             P1.0|--> Overflow/software
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with IAR Embedded Workbench Version: 3.21A
//******************************************************************************