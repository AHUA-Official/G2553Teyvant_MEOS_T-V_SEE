#include <msp430.h> 
#include <stdint.h>

volatile uint8_t master_receive_data;
volatile uint8_t slave_receive_data;
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
 * @fn:		void InitSpiBusA0(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:使用UCSI_A0模块,SPI主模式
 *
 */
void InitSpiBusA0(void)
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
 * @fn:		void InitSpiBusB0(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:使用UCSI_A0模块,SPI主模式
 *
 */
void InitSpiBusB0(void)
{
	/*引脚配置*/
	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;
	/*SPI禁能,时钟源SMCLK,从机模式无需设置时钟*/
	//UCB0CTL1 |= UCSWRST|UCSSEL_2;
	/*SPI模式,MSB优先,Slave模式*/
	UCB0CTL0 = UCMSB|UCSYNC;
	/*SPI速率,从机模式无需设置速率*/
	//UCB0BR0 = 1;
	//UCB0BR1 = 0;
	/*SPI使能*/
	UCB0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void SpiBusMatserWrite(uint8_t data)
 * @brief:	使用SPI总线写一个字节
 * @para:	data:待写数据
 * @return:	none
 * @comment:使用UCSI_A0模块
 *
 */
void SpiBusMatserWrite(uint8_t data)
{
	/*待发送数据写入发送寄存器*/
	UCA0TXBUF = data;
	/*等待数据发送成功*/
	while(!(IFG2 & UCA0TXIFG));
}
/*
 * @fn:		uint8_t SpiBusMasterRead(void)
 * @brief:	使用SPI总线读一个字节
 * @para:	none
 * @return:	读到的数据
 * @comment:使用UCSI_A0模块
 *
 */
uint8_t SpiBusMasterRead(void)
{
	/*首先清空接收标志*/
	IFG2 &= ~UCA0RXIFG;
	/*将无效数据写入发送寄存器*/
	UCA0TXBUF = 0xFF;
	/*等待收到数据*/
	while(!(IFG2 & UCA0RXIFG));
	/*返回收到的数据*/
	return UCA0RXBUF;
}
/*
 * @fn:		void SpiBusSlaveWrite(uint8_t data)
 * @brief:	使用SPI总线写一个字节
 * @para:	data:待写数据
 * @return:	none
 * @comment:使用UCSI_B0模块
 *
 */
void SpiBusSlaveWrite(uint8_t data)
{
	/*待发送数据写入发送寄存器*/
	UCB0TXBUF = data;
}
/*
 * @fn:		uint8_t SpiBusSlaveRead(void)
 * @brief:	使用SPI总线读一个字节
 * @para:	none
 * @return:	读到的数据
 * @comment:使用UCSI_B0模块
 *
 */
uint8_t SpiBusSlaveRead(void)
{
	/*等待收到数据*/
	while(!(IFG2 & UCB0RXIFG));
	/*返回收到的数据*/
	return UCB0RXBUF;
}
/*
 * main.c
 * P1.4   <------跳线帽连接------>   P1.5
 * P1.1   <------杜邦线连接------>   P1.6
 * P1.2   <------杜邦线连接------>   P1.7
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitSpiBusA0();
    InitSpiBusB0();

    /*主发从收*/
    SpiBusMatserWrite(0xAA);
    slave_receive_data = SpiBusSlaveRead();
    /*从发主收*/
    SpiBusSlaveWrite(0x55);
    master_receive_data = SpiBusMasterRead();

    while(1)
    {
    	__delay_cycles(1000000);
    }
	return 0;
}
