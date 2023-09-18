#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// 关闭看门狗，如果不关闭，系统就会不断复位
    /*将P1.0设置为输出，P1DIR是P1口的方向寄存器*/
    P1DIR |= BIT0;
    /*将P1.0设置为高电平，点亮LED1*/
    P1OUT |= BIT0;
    while(1)
    {

    }
	return 0;
}
