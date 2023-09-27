#include <msp430.h> 
#include "stdint.h"
uint8_t combuff[20] = {0};//����Ϊ20��������������ڱ��洮������
uint8_t iscomend = 0;//��������������ж��Ƿ�һ�������������Ѿ��ɹ�����
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

    //�����ж�����
    IE2 |= UCA0RXIE;
    //��ս����жϱ�־
    IFG2 &= ~UCA0RXIFG;
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
 * @fn:		void Execute(uint8_t *combuff)
 * @brief:	��������ִ�к���
 * @para:	combuff:ָ�򴮿����������ָ��
 * @return:	none
 * @comment:ִ�д�������
 */
void Execute(uint8_t *combuff)
{
	const uint8_t charbuff[5][10] = {"������","�׽�¡��","�೤��","LED1 On!","LED Off!"};//����Ԥ������
	if(combuff[0] == charbuff[0][0] && combuff[1] == charbuff[0][1])//������
	{
		UARTSendString("�����գ�!(��o��)",16);//
	}
	else if(combuff[0] == charbuff[1][0] && combuff[1] == charbuff[1][1])//�׽�¡��
	{
		UARTSendString("�����������( ��-�� )",21);
	}
	else if(combuff[0] == charbuff[2][0] && combuff[1] == charbuff[2][1])//�೤��
	{
		UARTSendString("��֧�飡!�r(�s-�t)�q",20);
	}
	else if(combuff[0] == charbuff[3][0] && combuff[6] == charbuff[3][6])//LED1 On!
	{
		UARTSendString("Yes!My Lord!(^^�g",17);
		P1OUT |= BIT0;
	}
	else if(combuff[0] == charbuff[4][0] && combuff[6] == charbuff[4][6])//LED1 Off!
	{
		UARTSendString("Yes!My Highness!(^^�g",21);
		P1OUT &= ~BIT0;
	}
	else
	{
		UARTSendString("What Are You Fucking Talking!(�p����)͹",41);
	}
}
/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    InitSystemClock();//��ʼ��ϵͳʱ��
    InitUART();//��ʼ������
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    __bis_SR_register(GIE);//��ȫ���ж�
    while(1)
    {
    	if(iscomend)
    	{
    		iscomend = 0;//�����־λ����ֹ�ظ�ִ��
    		Execute(combuff);//ִ������
    	}
    }
	return 0;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void UART_Receive_ISR(void)
{
	static uint8_t cnt = 0;
	if(IFG2 & UCA0RXIFG)//����Ƿ���USCI_A0�Ľ����жϣ�USCI_A0��USIC_B0�Ľ����жϹ���ͬһ����
	{
		IFG2 &= ~UCA0RXIFG;//�����־λ
		combuff[cnt++] = UCA0RXBUF;//��������
		cnt %= 20;//��ֹcnt����20�����»��������
		if(combuff[cnt - 1] == '\n')//�����⵽���������(�Ի�����Ϊ�������)
		{
			cnt = 0;//��λ������
			iscomend = 1;//����������
		}
	}
}
