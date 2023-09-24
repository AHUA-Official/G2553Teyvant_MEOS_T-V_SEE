/*
 * main.c
 */
#include "MSP430G2553.h"
#include "DAC8411.h"


void main(void) 
{
	
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1 = CALBC1_12MHZ;      /* Set DCO to12MHz */
    DCOCTL  = CALDCO_12MHZ;
    
    DAC8411_Init();
    write2DAC8411(0X7FFF);//基准电压一半
    while(1)
    {
      _BIS_SR(LPM0_bits) ;
    }
}
