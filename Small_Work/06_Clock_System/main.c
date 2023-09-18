#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    /*����MCLKƵ��*/
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
	/*��ʼ��LED2����IO��P1.6Ϊ���*/
    P1DIR |= BIT6;
    /*��ʼ��LED2����IO��P1.6Ϊ�͵�ƽ��LED2��ʼ״̬Ϊ��*/
    P1OUT &= ~BIT6;
    while(1)
    {
    	P1OUT ^= BIT6;
    	__delay_cycles(500000);
    }
	return 0;
}
