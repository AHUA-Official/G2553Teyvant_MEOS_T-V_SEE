#include <msp430.h> 
#include "stdint.h"
#include <stdio.h>
uint32_t timestamp = 0;//时间戳
uint16_t capvalue_1 = 0;//第一次捕捉值
uint16_t capvalue_2 = 0;//第二次捕捉值
uint32_t timestamp_1 = 0;//第一次时间戳
uint32_t timestamp_2 = 0;//第二次时间戳
uint32_t totaltime = 0;
float freq = 0;
/*
 * @fn:		void InitSystemClock(void)
 * @brief:	初始化系统时钟
 * @para:	none
 * @return:	none
 * @comment:初始化系统时钟
 */
void InitSystemClock(void)
{
	/*配置DCO为1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*配置SMCLK的时钟源为DCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK的分频系数置为1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:		void InitUART(void)
 * @brief:	初始化串口，包括设置波特率，数据位，校验位等
 * @para:	none
 * @return:	none
 * @comment:初始化串口
 */
void InitUART(void)
{
    /*复位USCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*选择USCI_Ax为UART模式*/
    UCA0CTL0 &= ~UCSYNC;

    /*配置UART时钟源为SMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*配置波特率为9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*使能端口复用*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*清除复位位，使能UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	通过串口发送字符串
 * @para:	pbuff:指向要发送字符串的指针
 * 			num:要发送的字符个数
 * @return:	none
 * @comment:通过串口发送字符串
 */
void UARTSendString(uint8_t *pbuff,uint8_t num)
{
	uint8_t cnt = 0;
	for(cnt = 0;cnt < num;cnt ++)
	{
		while(UCA0STAT & UCBUSY);
		UCA0TXBUF = *(pbuff + cnt);
	}
}
/*
 * @fn:		void UARTPrint(uint8_t *pbuff)
 * @brief:	通过串口发送字符串
 * @para:	pbuff:指向要发送字符串的指针
 * @return:	none
 * @comment:通过串口发送字符串,遇到'\0'停止发送
 */
void UARTPrint(uint8_t *pbuff)
{
	uint8_t cnt = 0;
	while(*(pbuff + cnt) != '\0')
	{
		while(UCA0STAT & UCBUSY);
		UCA0TXBUF = *(pbuff + cnt);
		cnt ++;
	}
}
/*
 * @fn:		void PrintFreq(float freq)
 * @brief:	通过串口发送频率
 * @para:	freq:频率
 * @return:	none
 * @comment:通过串口发送频率
 */
void PrintFreq(float freq)
{
	uint32_t temp = (uint32_t)(freq * 1000);
	uint8_t charbuff[] = {0,0,0,0,0,0,0,0,0};//最大999999.999Hz
	int8_t cnt = 0;
	for(cnt = 8;cnt >= 0;cnt --)
	{
		charbuff[cnt] = (uint8_t)(temp % 10) + '0';
		temp /= 10;
	}
	UARTSendString("频率为：",8);
	UARTSendString(charbuff,6);
	UARTSendString(".",1);
	UARTSendString(charbuff + 6,3);
	UARTSendString("Hz",2);
}
/*
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitUART();

    /*设置时钟源为SMCLK*/
    TA1CTL |= TASSEL_2;
    /*设置工作模式为Up Mode*/
    TA1CTL |= MC_1;
    /*设置定时间隔*/
    TA1CCR0 = 49999;//50ms
    /*开启TAIFG中断*/
    TA1CTL |= TAIE;

    /*TA1,CCR2用于捕捉功能*/
    TA1CCTL2 |= CAP;
    /*上升沿捕捉*/
    TA1CCTL2 |= CM0;
    /*P2.5作为捕捉输入(CCI2B)*/
    TA1CCTL2 |= CCIS0;
    P2SEL |= BIT5;
    /*允许捕捉比较中断*/
    TA1CCTL2 |= CCIE;

    /*设置时钟源为SMCLK*/
    TA0CTL |= TASSEL1;
    /*设置工作模式为Up&Down*/
    TA0CTL |= MC0|MC1;
    /*设置TA0CCR0为0x00FF*/
    TA0CCR0 = 0x0AAA;
    /*设置TA0CCR1为0x00FF*/
    TA0CCR1 = 0x0555;//占空比(TACCR0 - TACCR1) / TACCR0,频率=SMCLK/(TACCR0)/2
    /*设置为比较模式*/
    TA0CCTL0 &= ~CAP;
    TA0CCTL1 &= ~CAP;
    /*设置比较输出模式*/
    TA0CCTL1 |= OUTMOD_6;
    /*设置IO复用*/
    P1SEL |= BIT6;
    P1DIR |= BIT6;

    /*打开全局中断*/
    __bis_SR_register(GIE);
    while(1)
    {
    	__delay_cycles(500000);
    	freq = (float)(1000000.0) / totaltime;
    	PrintFreq(freq);
    }
	return 0;
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Time_Tick(void)
{
	static uint8_t cnt = 0;
	__bis_SR_register(GIE);//允许中断嵌套
	switch(TA1IV)
	{
	case 0x02://捕捉比较中断1
		break;
	case 0x04://捕捉比较中断2
		if(cnt == 0)
		{
			capvalue_1 = TA1CCR2;//保存第一次捕捉值
			timestamp_1 = timestamp;//保存第一次时间戳
			cnt ++;
		}
		else
		{
			capvalue_2 = TA1CCR2;//保存第二次捕捉值
			timestamp_2 = timestamp;//保存第二次时间戳
			cnt = 0;
			totaltime = (timestamp_2 - timestamp_1) * 50000 + capvalue_2 - capvalue_1;//计算总时间
		}
		break;
	case 0x0A://溢出中断
		timestamp ++;
		break;
	default:
		break;
	}
}
