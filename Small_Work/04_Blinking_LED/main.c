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
    while(1)
    {
    	P1OUT ^= BIT6;
    	/*此语句等效为P1OUT = P1OUT ^ BIT6
    	 * '^'为按位异或运算。
    	 * P1OUT 初始化为x0xxxxxx,BIT6为 01000000
    	 * 第一次执行此语句时：x0xxxxxx ^ 01000000 -> x1xxxxxx -> P1OUT,P1.6输出高电平,LED2点亮
    	 * 第二次执行此语句时：x1xxxxxx ^ 01000000 -> x0xxxxxx -> P1OUT,P1.6输出低电平,LED2熄灭
    	 * 如此循环，LED2闪烁
    	 *
    	 */
    	__delay_cycles(500000);
    	/*这条语句的执行等同于延时，参数为500000，执行时间大致为500ms，也就是这条语句需要500ms才能
    	 * 执行完毕。
    	 */
    }
	return 0;
}
