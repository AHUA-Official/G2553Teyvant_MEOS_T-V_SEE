#include <msp430.h> 
#include "stdint.h"

uint32_t currenttime = 40500;//��������ʱ��ı�������ֵ����11:15:00
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
 * @fn:		void PrintTime(uint32_t time)
 * @brief:	ͨ�����ڷ��͵�ǰʱ��
 * @para:	time:��ǰʱ��
 * @return:	none
 * @comment:ͨ�����ڷ��͵�ǰʱ��
 */
void PrintTime(uint32_t time)
{
	uint8_t charbuff[] = {0,0,':',0,0,':',0,0};
	charbuff[7] = (uint8_t)((time % 60) % 10) + '0';//�õ���ǰ���λ
	charbuff[6] = (uint8_t)((time % 60) / 10) + '0';//�õ���ǰ��ʮλ
	charbuff[4] = (uint8_t)((time % 3600) / 60 % 10) + '0';//�õ���ǰ�ָ�λ
	charbuff[3] = (uint8_t)((time % 3600) / 60 / 10) + '0';//�õ���ǰ��ʮλ
	charbuff[1] = (uint8_t)((time / 3600) % 10) + '0';//�õ���ǰʱ��λ
	charbuff[0] = (uint8_t)(time / 3600 / 10) + '0';//�õ���ǰʱʮλ
	UARTSendString("��ǰʱ�䣺",10);
	UARTSendString(charbuff,8);
}
/*
 * main.c
 */
uint8_t flag = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitUART();
    /*����P1.6Ϊ���*/
    P1DIR |= BIT6;

    /*����ʱ��ԴΪSMCLK*/
    TA1CTL |= TASSEL_2;
    /*���ù���ģʽΪUp Mode*/
    TA1CTL |= MC_1;
    /*���ö�ʱ���*/
    TA1CCR0 = 49999;// 50ms 1MHz 1/1MHz 1ns 50ms / 1ns = 50000 50000 - 1 = 49999

    /*����TAIFG�ж�*/
    TA1CTL |= TAIE;
    /*��ȫ���ж�*/
    __bis_SR_register(GIE);
    while(1)
    {
    	if(flag == 1)
    	{
    		flag = 0;
    		P1OUT ^= BIT6;
    		PrintTime(currenttime);
    	}
    }
	return 0;
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Time_Tick(void)
{
	static uint8_t cnt = 0;
	switch(TA1IV)
	{
	case 0x02:
		break;
	case 0x04:
		break;
	case 0x0A:
		cnt ++;
		if(cnt == 20)
		{
			cnt = 0;//���������
			flag = 1;//1s ����
			currenttime ++;//ʱ���1
			currenttime %= 86400;//һ��24Сʱ����ֹ����
		}
		break;
	default:
		break;
	}
}
