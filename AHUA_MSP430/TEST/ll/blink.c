#include <msp430.h>

void delay(void);                           // Software delay

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  CACTL2 = P2CA4;                           // CA1/P1.1 = +comp
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TACTL = TASSEL_2 + ID_3 + MC_2;           // SMCLK/8, cont-mode
  __enable_interrupt();                     // enable interrupts

  while (1)                                 // Loop
  {
    CACTL1 = 0x00;                          // No reference voltage
    __bis_SR_register(LPM0_bits);           // Enter LPM0
    CACTL1 = CAREF0 + CAON;                 // 0.25*Vcc, Comp. on
    __bis_SR_register(LPM0_bits);           // Enter LPM0
    CACTL1 = CAREF1 + CAON;                 // 0.5*Vcc, Comp. on
    __bis_SR_register(LPM0_bits);           // Enter LPM0
    CACTL1 = CAREF1 + CAREF0 + CAON;        // 0.55V, Comp. on
    __bis_SR_register(LPM0_bits);           // Enter LPM0
  }
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM0_bits);   // Clear LPM0 bits from 0(SR)
}
