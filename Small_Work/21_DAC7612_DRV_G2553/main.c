#include <msp430.h> 
#include <stdint.h>

#define		DAC_A_OUT		0
#define		DAC_B_OUT		1
#define		DAC_AB_OUT		2
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
	UCA0CTL0 = UCMSB|UCMST|UCSYNC|UCCKPL;
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
 */
uint8_t SpiBusRead(void)
{
	/*�ȴ��յ�����*/
	while(!(IFG2 & UCA0RXIFG));
	/*�����յ�������*/
	return UCA0RXBUF;
}
/*
 * @fn:		void InitDAC7612(void)
 * @brief:	��ʼ��DAC7612��CS��LOADDACS
 * @para:	none
 * @return:	����������
 * @comment:CS���ӵ�P1.5��LOADDACS���ӵ�P1.7
 */
void InitDAC7612(void)
{
	/*��ʼ��P1.5��P1.7Ϊ���*/
	P1DIR |= BIT5 + BIT7;
	/*��ʼ��P1.5��P1.7����ߵ�ƽ*/
	P1OUT |= BIT5 + BIT7;
}
/*
 * @fn:		void WriteDAC7612(uint16_t data,uint8_t chn)
 * @brief:	��DAC7612д������
 * @para:	data:0-4095
 * 			chn:ͨ������ѡDAC_A_OUT��DAC_B_OUT��DAC_AB_OUT
 * @return:	none
 * @comment:CS���ӵ�P1.5��LOADDACS���ӵ�P1.7
 */
void WriteDAC7612(uint16_t data,uint8_t chn)
{
	uint16_t temp = 0;
	/*ȷ����д������*/
	temp = data & 0xFFF;
	switch(chn)//����Ҫ�����ͨ��ȷ������
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
	/*CSΪ��*/
	P1OUT &= ~BIT5;
	/*д������*/
	SpiBusWrite((uint8_t)(temp >> 8));
	SpiBusWrite((uint8_t)(temp & 0x00FF));
	/*CSΪ��*/
	P1OUT |= BIT5;
	/*��������*/
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
