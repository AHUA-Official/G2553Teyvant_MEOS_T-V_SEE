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
    while(1)
    {
    	P1OUT ^= BIT6;
    	/*������ЧΪP1OUT = P1OUT ^ BIT6
    	 * '^'Ϊ��λ������㡣
    	 * P1OUT ��ʼ��Ϊx0xxxxxx,BIT6Ϊ 01000000
    	 * ��һ��ִ�д����ʱ��x0xxxxxx ^ 01000000 -> x1xxxxxx -> P1OUT,P1.6����ߵ�ƽ,LED2����
    	 * �ڶ���ִ�д����ʱ��x1xxxxxx ^ 01000000 -> x0xxxxxx -> P1OUT,P1.6����͵�ƽ,LED2Ϩ��
    	 * ���ѭ����LED2��˸
    	 *
    	 */
    	__delay_cycles(500000);
    	/*��������ִ�е�ͬ����ʱ������Ϊ500000��ִ��ʱ�����Ϊ500ms��Ҳ�������������Ҫ500ms����
    	 * ִ����ϡ�
    	 */
    }
	return 0;
}
