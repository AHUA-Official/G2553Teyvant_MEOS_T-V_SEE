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
    while(1)
    {
    	if(P1IN & BIT3)//���P1.3��Ϊ�ߵ�ƽ��֤������û������
    	{
    		P1OUT &= ~BIT6;//Ϩ��LED2
    	}
    	else//���P1.3Ϊ�͵�ƽ������������
    	{
    		P1OUT |= BIT6;//����LED2
    	}
    }
	return 0;
}
