#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// �رտ��Ź���ʱ��
	/*��ʼ��LED2����IO��P1.6Ϊ���*/
    P1DIR |= BIT6;
    /*��ʼ��LED2����IO��P1.6Ϊ�͵�ƽ��LED2��ʼ״̬Ϊ��*/
    P1OUT &= ~BIT6;
    /*��ʼ����������IO��P1.3Ϊ����*/
    P1DIR &= ~BIT3;
    /*ʹ��P1.3�ڵ���������*/
    P1REN |= BIT3;
    P1OUT |= BIT3;
    /*��P1.3���ж�*/
    P1IE |= BIT3;
    /*�趨Ϊ�½��ش���*/
    P1IES |= BIT3;
    /*����жϱ�־λ*/
    P1IFG &= ~BIT3;
    /*��ȫ���ж�*/
    __bis_SR_register(GIE);
    while(1)
    {

    }
	return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
	if(P1IFG & BIT3)//�ж��Ƿ���P1.3�����ж�
	{
		P1IFG &= ~BIT3;//�����־λ
		P1OUT ^= BIT6;
	}
}
