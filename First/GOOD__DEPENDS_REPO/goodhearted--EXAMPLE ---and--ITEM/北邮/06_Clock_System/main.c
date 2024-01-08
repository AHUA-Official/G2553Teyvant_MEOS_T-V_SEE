#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    /*设置MCLK频率*/
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
	/*初始化LED2所在IO口P1.6为输出*/
    P1DIR |= BIT6;
    /*初始化LED2所在IO口P1.6为低电平，LED2初始状态为灭*/
    P1OUT &= ~BIT6;
    while(1)
    {
    	P1OUT ^= BIT6;
    	__delay_cycles(500000);
    }
	return 0;
}
