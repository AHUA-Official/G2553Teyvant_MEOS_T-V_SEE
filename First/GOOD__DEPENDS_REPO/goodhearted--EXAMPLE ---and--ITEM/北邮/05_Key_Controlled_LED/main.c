#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// 关闭看门狗定时器
	/*初始化LED2所在IO口P1.6为输出*/
    P1DIR |= BIT6;
    /*初始化LED2所在IO口P1.6为低电平，LED2初始状态为灭*/
    P1OUT &= ~BIT6;
    /*初始化按键所在IO口P1.3为输入*/
    P1DIR &= ~BIT3;
    /*使能P1.3口的上拉电阻*/
    P1REN |= BIT3;
    P1OUT |= BIT3;
    while(1)
    {
    	if(P1IN & BIT3)//如果P1.3口为高电平，证明按键没被按下
    	{
    		P1OUT &= ~BIT6;//熄灭LED2
    	}
    	else//如果P1.3为低电平，按键被按下
    	{
    		P1OUT |= BIT6;//点亮LED2
    	}
    }
	return 0;
}
