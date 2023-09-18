#include <msp430.h> 
#include <stdint.h>

volatile uint8_t rx_data;
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
	UCA0CTL0 = UCMSB|UCMST|UCSYNC;
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
 *
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
 *
 */
uint8_t SpiBusRead(void)
{
	/*等待收到数据*/
	while(!(IFG2 & UCA0RXIFG));
	/*返回收到的数据*/
	return UCA0RXBUF;
}
/*
 * main.c
 * P1.1   <------跳线帽连接------>   P1.2
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitSpiBus();
    SpiBusWrite(0xAA);
    rx_data = SpiBusRead();
    while(1)
    {
    	__delay_cycles(1000000);
    }
	return 0;
}
