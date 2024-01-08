#include <msp430.h> 
#include "stdint.h"
#include <stdio.h>
uint32_t timestamp = 0;//ʱ���
uint16_t capvalue_1 = 0;//��һ�β�׽ֵ
uint16_t capvalue_2 = 0;//�ڶ��β�׽ֵ
uint32_t timestamp_1 = 0;//��һ��ʱ���
uint32_t timestamp_2 = 0;//�ڶ���ʱ���
uint32_t totaltime = 0;
float freq = 0;
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
 * @fn:		void InitUART(void)
 * @brief:	��ʼ�����ڣ��������ò����ʣ�����λ��У��λ��
 * @para:	none
 * @return:	none
 * @comment:��ʼ������
 */
void InitUART(void)
{
    /*��λUSCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*ѡ��USCI_AxΪUARTģʽ*/
    UCA0CTL0 &= ~UCSYNC;

    /*����UARTʱ��ԴΪSMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*���ò�����Ϊ9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*ʹ�ܶ˿ڸ���*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*�����λλ��ʹ��UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	ͨ�����ڷ����ַ���
 * @para:	pbuff:ָ��Ҫ�����ַ�����ָ��
 * 			num:Ҫ���͵��ַ�����
 * @return:	none
 * @comment:ͨ�����ڷ����ַ���
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
 * @brief:	ͨ�����ڷ����ַ���
 * @para:	pbuff:ָ��Ҫ�����ַ�����ָ��
 * @return:	none
 * @comment:ͨ�����ڷ����ַ���,����'\0'ֹͣ����
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
 * @brief:	ͨ�����ڷ���Ƶ��
 * @para:	freq:Ƶ��
 * @return:	none
 * @comment:ͨ�����ڷ���Ƶ��
 */
void PrintFreq(float freq)
{
	uint32_t temp = (uint32_t)(freq * 1000);
	uint8_t charbuff[] = {0,0,0,0,0,0,0,0,0};//���999999.999Hz
	int8_t cnt = 0;
	for(cnt = 8;cnt >= 0;cnt --)
	{
		charbuff[cnt] = (uint8_t)(temp % 10) + '0';
		temp /= 10;
	}
	UARTSendString("Ƶ��Ϊ��",8);
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

    /*����ʱ��ԴΪSMCLK*/
    TA1CTL |= TASSEL_2;
    /*���ù���ģʽΪUp Mode*/
    TA1CTL |= MC_1;
    /*���ö�ʱ���*/
    TA1CCR0 = 49999;//50ms
    /*����TAIFG�ж�*/
    TA1CTL |= TAIE;

    /*TA1,CCR2���ڲ�׽����*/
    TA1CCTL2 |= CAP;
    /*�����ز�׽*/
    TA1CCTL2 |= CM0;
    /*P2.5��Ϊ��׽����(CCI2B)*/
    TA1CCTL2 |= CCIS0;
    P2SEL |= BIT5;
    /*����׽�Ƚ��ж�*/
    TA1CCTL2 |= CCIE;

    /*����ʱ��ԴΪSMCLK*/
    TA0CTL |= TASSEL1;
    /*���ù���ģʽΪUp&Down*/
    TA0CTL |= MC0|MC1;
    /*����TA0CCR0Ϊ0x00FF*/
    TA0CCR0 = 0x0AAA;
    /*����TA0CCR1Ϊ0x00FF*/
    TA0CCR1 = 0x0555;//ռ�ձ�(TACCR0 - TACCR1) / TACCR0,Ƶ��=SMCLK/(TACCR0)/2
    /*����Ϊ�Ƚ�ģʽ*/
    TA0CCTL0 &= ~CAP;
    TA0CCTL1 &= ~CAP;
    /*���ñȽ����ģʽ*/
    TA0CCTL1 |= OUTMOD_6;
    /*����IO����*/
    P1SEL |= BIT6;
    P1DIR |= BIT6;

    /*��ȫ���ж�*/
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
	__bis_SR_register(GIE);//�����ж�Ƕ��
	switch(TA1IV)
	{
	case 0x02://��׽�Ƚ��ж�1
		break;
	case 0x04://��׽�Ƚ��ж�2
		if(cnt == 0)
		{
			capvalue_1 = TA1CCR2;//�����һ�β�׽ֵ
			timestamp_1 = timestamp;//�����һ��ʱ���
			cnt ++;
		}
		else
		{
			capvalue_2 = TA1CCR2;//����ڶ��β�׽ֵ
			timestamp_2 = timestamp;//����ڶ���ʱ���
			cnt = 0;
			totaltime = (timestamp_2 - timestamp_1) * 50000 + capvalue_2 - capvalue_1;//������ʱ��
		}
		break;
	case 0x0A://����ж�
		timestamp ++;
		break;
	default:
		break;
	}
}
