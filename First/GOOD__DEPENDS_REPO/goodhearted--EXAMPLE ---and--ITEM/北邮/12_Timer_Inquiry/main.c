#include <msp430.h> 
#include "stdint.h"
/*
 * main.c
 */
int main(void)
{
	uint8_t cnt = 0;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	/*配置DCO频率为1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*设置P1.6为输出*/
    P1DIR |= BIT6;

    /*设置时钟源为SMCLK*/
    TA1CTL |= TASSEL_2;
    /*设置工作模式为Up Mode*/
    TA1CTL |= MC_1;//Up Mode模式下，16bit Timer计数器 TAR会从0递增到TACCR0，然后回到0，重复这个过程
    /*设置定时间隔*/
    TA1CCR0 = 49999;//1MHz 计 50000个数，1/1MHz * 50000 20Hz 0.05s
    while(1)
    {
    	if(TA1CTL & TAIFG)
    	{
    		cnt ++;
    		TA1CTL &= ~TAIFG;
    		if(cnt == 20)//0.05s * 20 = 1s
    		{
    			P1OUT ^= BIT6;
    			cnt = 0;
    		}
    	}
    }
	return 0;
}
