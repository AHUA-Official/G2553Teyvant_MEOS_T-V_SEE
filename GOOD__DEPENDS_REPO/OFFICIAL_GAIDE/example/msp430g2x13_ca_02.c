//******************************************************************************
//   MSP430G2x13/G2x53 Demo - Comp_A, Detect Threshold, Set P1.0 if P1.1 > 0.25*Vcc
//
//   Description: Use Comparator_A to detect a voltage threshold.
//   Using an external potentiometer, an unknown voltage is applied to P1.1.
//   Comparator_A compares the unknown voltage to an internal reference
//   voltage, in this example 0.25*VCC. If the unknown voltage is higher
//   than 0.25*VCC, P1.0 is set, if not, P1.0 is reset.
//   ACLK = n/a, MCLK = SMCLK = default DCO
//
//          MSP430G2x13/G2x53
//       -----------------
//   /|\ |            XIN|-
//    |  |               |
//    ---|RST        XOUT|-
//    |  |               |
//    R<-|P1.1/CA1   P1.0|-->LED
//    |  |               |
//    ---|VSS
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include  <msp430g2553.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  while (1)                                 // Test P1.4
  {
     P1OUT |= 0x01;       // if P1.4 set, set P1.0
    P1OUT &= ~0x01;                    // else reset
  }
}

