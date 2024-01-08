#include <msp430.h> 
#include <stdint.h>

volatile uint8_t master_receive_data;
volatile uint8_t slave_receive_data;
/*
 * @fn:		void InitSystemClock(void)
 * @brief:	��ʼ��ϵͳʱ��
 * @para:	none
 * @return:	none
 * @comment:��ʼ��ϵͳʱ��
 */
void InitSystemClock(void)
{
	/*����DCOΪ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*����SMCLK��ʱ��ԴΪDCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK�ķ�Ƶϵ����Ϊ1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:		void InitSpiBusA0(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:ʹ��UCSI_A0ģ��,SPI��ģʽ
 *
 */
void InitSpiBusA0(void)
{
	/*��������*/
	P1SEL |= BIT1 | BIT2 | BIT4;
	P1SEL2 |= BIT1 | BIT2 | BIT4;
	/*SPI����,ʱ��ԴSMCLK*/
	UCA0CTL1 |= UCSWRST|UCSSEL_2;
	/*SPIģʽ,MSB����,Masterģʽ*/
	UCA0CTL0 = UCMSB|UCMST|UCSYNC;
	/*SPI����*/
	UCA0BR0 = 1;
	UCA0BR1 = 0;
	/*SPIʹ��*/
	UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void InitSpiBusB0(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:ʹ��UCSI_A0ģ��,SPI��ģʽ
 *
 */
void InitSpiBusB0(void)
{
	/*��������*/
	P1SEL |= BIT5 | BIT6 | BIT7;
	P1SEL2 |= BIT5 | BIT6 | BIT7;
	/*SPI����,ʱ��ԴSMCLK,�ӻ�ģʽ��������ʱ��*/
	//UCB0CTL1 |= UCSWRST|UCSSEL_2;
	/*SPIģʽ,MSB����,Slaveģʽ*/
	UCB0CTL0 = UCMSB|UCSYNC;
	/*SPI����,�ӻ�ģʽ������������*/
	//UCB0BR0 = 1;
	//UCB0BR1 = 0;
	/*SPIʹ��*/
	UCB0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void SpiBusMatserWrite(uint8_t data)
 * @brief:	ʹ��SPI����дһ���ֽ�
 * @para:	data:��д����
 * @return:	none
 * @comment:ʹ��UCSI_A0ģ��
 *
 */
void SpiBusMatserWrite(uint8_t data)
{
	/*����������д�뷢�ͼĴ���*/
	UCA0TXBUF = data;
	/*�ȴ����ݷ��ͳɹ�*/
	while(!(IFG2 & UCA0TXIFG));
}
/*
 * @fn:		uint8_t SpiBusMasterRead(void)
 * @brief:	ʹ��SPI���߶�һ���ֽ�
 * @para:	none
 * @return:	����������
 * @comment:ʹ��UCSI_A0ģ��
 *
 */
uint8_t SpiBusMasterRead(void)
{
	/*������ս��ձ�־*/
	IFG2 &= ~UCA0RXIFG;
	/*����Ч����д�뷢�ͼĴ���*/
	UCA0TXBUF = 0xFF;
	/*�ȴ��յ�����*/
	while(!(IFG2 & UCA0RXIFG));
	/*�����յ�������*/
	return UCA0RXBUF;
}
/*
 * @fn:		void SpiBusSlaveWrite(uint8_t data)
 * @brief:	ʹ��SPI����дһ���ֽ�
 * @para:	data:��д����
 * @return:	none
 * @comment:ʹ��UCSI_B0ģ��
 *
 */
void SpiBusSlaveWrite(uint8_t data)
{
	/*����������д�뷢�ͼĴ���*/
	UCB0TXBUF = data;
}
/*
 * @fn:		uint8_t SpiBusSlaveRead(void)
 * @brief:	ʹ��SPI���߶�һ���ֽ�
 * @para:	none
 * @return:	����������
 * @comment:ʹ��UCSI_B0ģ��
 *
 */
uint8_t SpiBusSlaveRead(void)
{
	/*�ȴ��յ�����*/
	while(!(IFG2 & UCB0RXIFG));
	/*�����յ�������*/
	return UCB0RXBUF;
}
/*
 * main.c
 * P1.4   <------����ñ����------>   P1.5
 * P1.1   <------�Ű�������------>   P1.6
 * P1.2   <------�Ű�������------>   P1.7
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitSpiBusA0();
    InitSpiBusB0();

    /*��������*/
    SpiBusMatserWrite(0xAA);
    slave_receive_data = SpiBusSlaveRead();
    /*�ӷ�����*/
    SpiBusSlaveWrite(0x55);
    master_receive_data = SpiBusMasterRead();

    while(1)
    {
    	__delay_cycles(1000000);
    }
	return 0;
}
