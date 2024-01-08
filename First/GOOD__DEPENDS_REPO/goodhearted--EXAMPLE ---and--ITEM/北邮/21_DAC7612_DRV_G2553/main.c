#include <msp430.h> 
#include <stdint.h>

#define		DAC_A_OUT		0
#define		DAC_B_OUT		1
#define		DAC_AB_OUT		2
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
 * @fn:		void InitSpiBus(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:使用UCSI_A0模块,SPI主模式
 *
 */
void InitSpiBus(void)
{
	/*引脚配置*/
	P1SEL |= BIT1 | BIT2 | BIT4;
	P1SEL2 |= BIT1 | BIT2 | BIT4;
	/*SPI禁能,时钟源SMCLK*/
	UCA0CTL1 |= UCSWRST|UCSSEL_2;
	/*SPI模式,MSB优先,Master模式*/
	UCA0CTL0 = UCMSB|UCMST|UCSYNC|UCCKPL;
	/*SPI速率*/
	UCA0BR0 = 1;
	UCA0BR1 = 0;
	/*SPI使能*/
	UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void SpiBusWrite(uint8_t data)
 * @brief:	使用SPI总线写一个字节
 * @para:	data:待写数据
 * @return:	none
 * @comment:使用UCSI_A0模块
 */
void SpiBusWrite(uint8_t data)
{
	/*待发送数据写入发送寄存器*/
	UCA0TXBUF = data;
	/*等待数据发送成功*/
	while(!(IFG2 & UCA0TXIFG));
}
/*
 * @fn:		uint8_t SpiBusRead(void)
 * @brief:	使用SPI总线读一个字节
 * @para:	none
 * @return:	读到的数据
 * @comment:使用UCSI_A0模块
 */
uint8_t SpiBusRead(void)
{
	/*等待收到数据*/
	while(!(IFG2 & UCA0RXIFG));
	/*返回收到的数据*/
	return UCA0RXBUF;
}
/*
 * @fn:		void InitDAC7612(void)
 * @brief:	初始化DAC7612的CS和LOADDACS
 * @para:	none
 * @return:	读到的数据
 * @comment:CS连接到P1.5，LOADDACS连接到P1.7
 */
void InitDAC7612(void)
{
	/*初始化P1.5和P1.7为输出*/
	P1DIR |= BIT5 + BIT7;
	/*初始化P1.5和P1.7输出高电平*/
	P1OUT |= BIT5 + BIT7;
}
/*
 * @fn:		void WriteDAC7612(uint16_t data,uint8_t chn)
 * @brief:	向DAC7612写入数据
 * @para:	data:0-4095
 * 			chn:通道，可选DAC_A_OUT、DAC_B_OUT、DAC_AB_OUT
 * @return:	none
 * @comment:CS连接到P1.5，LOADDACS连接到P1.7
 */
void WriteDAC7612(uint16_t data,uint8_t chn)
{
	uint16_t temp = 0;
	/*确定待写入数据*/
	temp = data & 0xFFF;
	switch(chn)//根据要输出的通道确定数据
	{
	case DAC_A_OUT:
		temp |= 0x2000;
		break;
	case DAC_B_OUT:
		temp |= 0x3000;
		break;
	case DAC_AB_OUT:
		temp &= ~0x2000;
		break;
	default:
		break;
	}
	/*CS为低*/
	P1OUT &= ~BIT5;
	/*写入数据*/
	SpiBusWrite((uint8_t)(temp >> 8));
	SpiBusWrite((uint8_t)(temp & 0x00FF));
	/*CS为高*/
	P1OUT |= BIT5;
	/*更新数据*/
	P1OUT &= ~BIT7;
	__delay_cycles(1000);
	P1OUT |= BIT7;
}
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitSpiBus();
    InitDAC7612();
    WriteDAC7612(2048,DAC_A_OUT);
    while(1)
    {
    	__delay_cycles(1000000);
    }
	return 0;
}
