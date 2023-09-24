
#include "msp430g2253.h"



void main(void)
{
      WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
     
     P1DIR = 0XFF;
 
     
     while(1)
     {
     
           P1OUT = 0XFF;
           P1OUT = 0X00;   
     }
}