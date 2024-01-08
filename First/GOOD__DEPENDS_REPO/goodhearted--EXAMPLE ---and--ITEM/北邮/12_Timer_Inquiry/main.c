#include <msp430.h> 
#include "stdint.h"
/*
 * main.c
 */
int main(void)
{
	uint8_t cnt = 0;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	/*����DCOƵ��Ϊ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*����P1.6Ϊ���*/
    P1DIR |= BIT6;

    /*����ʱ��ԴΪSMCLK*/
    TA1CTL |= TASSEL_2;
    /*���ù���ģʽΪUp Mode*/
    TA1CTL |= MC_1;//Up Modeģʽ�£�16bit Timer������ TAR���0������TACCR0��Ȼ��ص�0���ظ��������
    /*���ö�ʱ���*/
    TA1CCR0 = 49999;//1MHz �� 50000������1/1MHz * 50000 20Hz 0.05s
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
