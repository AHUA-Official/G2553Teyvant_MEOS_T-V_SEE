#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// �رտ��Ź���������رգ�ϵͳ�ͻ᲻�ϸ�λ
    /*��P1.0����Ϊ�����P1DIR��P1�ڵķ���Ĵ���*/
    P1DIR |= BIT0;
    /*��P1.0����Ϊ�ߵ�ƽ������LED1*/
    P1OUT |= BIT0;
    while(1)
    {

    }
	return 0;
}
