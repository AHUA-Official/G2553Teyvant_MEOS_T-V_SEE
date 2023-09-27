#include <msp430.h> 
#include <stdint.h>

volatile uint8_t rx_data;
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
 * @fn:		void InitSpiBus(void)
 * @brief:	initialize spi module
 * @para:	none
 * @return:	none
 * @comment:ʹ��UCSI_A0ģ��,SPI��ģʽ
 *
 */
void InitSpiBus(void)
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
 * @fn:		void SpiBusWrite(uint8_t data)
 * @brief:	ʹ��SPI����дһ���ֽ�
 * @para:	data:��д����
 * @return:	none
 * @comment:ʹ��UCSI_A0ģ��
 *
 */
void SpiBusWrite(uint8_t data)
{
	/*����������д�뷢�ͼĴ���*/
	UCA0TXBUF = data;
	/*�ȴ����ݷ��ͳɹ�*/
	while(!(IFG2 & UCA0TXIFG));
}
/*
 * @fn:		uint8_t SpiBusRead(void)
 * @brief:	ʹ��SPI���߶�һ���ֽ�
 * @para:	none
 * @return:	����������
 * @comment:ʹ��UCSI_A0ģ��
 *
 */
uint8_t SpiBusRead(void)
{
	/*�ȴ��յ�����*/
	while(!(IFG2 & UCA0RXIFG));
	/*�����յ�������*/
	return UCA0RXBUF;
}
/*
 * main.c
 * P1.1   <------����ñ����------>   P1.2
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
