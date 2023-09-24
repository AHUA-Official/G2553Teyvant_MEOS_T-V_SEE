/*
 * SPI.c
Ӳ��SPI����������
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#include"MSP430G2553.h"
#include"SPI.h"


//*********************************������Ӳ��SPI�ײ���������**********************************
//-----Ӳ��SPI�ܽź궨��-----
#define SPI_SIMO        	BIT2		//1.2
#define SPI_SOMI        	BIT1		//1.1
#define SPI_CLK         	BIT4		//1.4
#define SPI_CS           	BIT4		//P2.4
//-----Ӳ��SPI���ƶ˿ں궨��-----
#define SPI_SEL2         	P1SEL2
#define SPI_SEL         		P1SEL
#define SPI_DIR         	P1DIR
#define SPI_OUT         	P1OUT
#define SPI_REN         	P1REN
//-----ʹ�ܶ�CS�˿ں궨��-----
#define SPI_CS_SEL2       P2SEL2
#define SPI_CS_SEL       	 P2SEL
#define SPI_CS_OUT       P2OUT
#define SPI_CS_DIR       	 P2DIR


/****************************************************************************
* ��    �ƣ�SPI_init()
* ��    �ܣ���Ӳ��SPI���г�ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ��������ʹ�ú���Ķ�д�������ڳ���ʼ�����ȵ��øó�ʼ������
* ʹ�÷�����SPI_init();
****************************************************************************/
void SPI_init(void)
{
    //-----�ܽų�ʼ��Ϊ SPI ����-----
    SPI_SEL |= SPI_CLK + SPI_SOMI + SPI_SIMO;
    SPI_SEL2 |= SPI_CLK + SPI_SOMI + SPI_SIMO;
    SPI_DIR |= SPI_CLK + SPI_SIMO;
    //-----SD ��SPIģʽ�£���Ҫ��SOMI����������-----
    SPI_REN |= SPI_SOMI;
    SPI_OUT |= SPI_SOMI;
    //-----ʹ��CS�ܽ�Ϊ�������-----
    SPI_CS_SEL   &= ~SPI_CS;
    SPI_CS_SEL2 &= ~SPI_CS;
    SPI_CS_OUT |= SPI_CS;
    SPI_CS_DIR  |= SPI_CS;
    //-----��λUCA0-----
    UCA0CTL1 |= UCSWRST;
    //-----3-pin, 8-bit SPI ����ģʽ- ������----
    UCA0CTL0 = UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC;
    //-----ʱ��ѡ��SMCLK��MSB first-----
    UCA0CTL1 = UCSWRST + UCSSEL_2;
    //-----f_UCxCLK = 12MHz/50 = 240kHz-----
    UCA0BR0 = 50;
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    //-----����UCA0-----
    UCA0CTL1 &= ~UCSWRST;
    //-----����жϱ�־λ-----
    IFG2 &= ~(UCA0RXIFG+UCA0TXIFG );
    __bis_SR_register(GIE);
}
/****************************************************************************
* ��       �ƣ�SPI_CS_High()
* ��       �ܣ�3��Ӳ��SPIģʽ������ʹ��CS�ܽ�Ϊ�ߵ�ƽ
* ��ڲ�������
* ���ڲ�������
* ˵       �����˴���CS�ܽſ��Ը���Ӳ������Ҫ������ָ���ܽ���CS���ɡ�
* ʹ�÷�����SPI_CS_High();
****************************************************************************/
void SPI_CS_High(void)
{
    SPI_CS_OUT |= SPI_CS;
}
/****************************************************************************
* ��       �ƣ�SPI_HighSpeed()
* ��       �ܣ�����SPIΪ����
* ��ڲ�������
* ���ڲ�������
* ˵       ������ЩSPI�豸�ɹ����ڸ���SPI״̬
* ʹ�÷�������
****************************************************************************/
void SPI_HighSpeed()
{
	UCA0CTL1 |= UCSWRST;
	UCA0BR0 = 2;                                // f_UCxCLK = 12MHz/2 = 6MHz
	UCA0BR1 = 0;
	UCA0MCTL = 0;
	UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* ��       �ƣ�SPI_LowSpeed()
* ��       �ܣ�����SPIΪ����
* ��ڲ�������
* ���ڲ�������
* ˵       ������ЩSPI�豸��Ҫ�����ڵ���SPI״̬
* ʹ�÷�������
****************************************************************************/
void SPI_LowSpeed()
{
    UCA0CTL1 |= UCSWRST;
    UCA0BR0 =50;                          		// f_UCxCLK = 12MHz/50 = 240KHz
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* ��    �ƣ�SPI_CS_Low()
* ��    �ܣ�3��Ӳ��SPIģʽ������ʹ��CS�ܽ�Ϊ�͵�ƽ
* ��ڲ�������
* ���ڲ�������
* ˵    ���� �˴���CS�ܽſ��Ը���Ӳ������Ҫ������ָ���ܽ���CS���ɡ�
* ʹ�÷�����SPI_CS_SLow();
****************************************************************************/
void SPI_CS_Low(void)
{
    SPI_CS_OUT &= ~SPI_CS;
}

/***************************************************************************
* ��    �ƣ�SPI_RxFrame()
* ��    �ܣ�3��Ӳ��SPIģʽ�£�����ָ����Ŀ���ֽ�
* ��ڲ�����pBuffer��ָ���Ž������ݵ�����
* 					size��Ҫ���յ��ֽ���
* ���ڲ�����0����ǰӲ��SPI��æ��
* 					1����ǰ�����ѷ�����ϣ�
* ˵    ����ʹ�øú������Խ���ָ��������һ֡����
* ʹ�÷�����SPI_RxFrame(CMD,6);// ����6���ֽڣ������η���CMD��
 ******************************************************************************/
unsigned char SPI_RxFrame(unsigned char  *pBuffer, unsigned int size)
{
    unsigned int gie = __get_SR_register() & GIE;              //���浱ǰ ȫ�� GIE ״̬
//    __disable_interrupt();                                 						//��ֹ�ж�
    IFG2 &= ~UCA0RXIFG;                                   				//��������жϱ�־λ
    while (size--){
        while (!(IFG2 & UCA0TXIFG)) ;                    			 	//�ȴ��������
        UCA0TXBUF = 0xff;                                  					//���Ϳ��ֽ�
        while (!(IFG2 & UCA0RXIFG)) ;                     				//�ȴ��������
        *pBuffer++ = UCA0RXBUF;
    }
//    __bis_SR_register(gie);                               						 //��ԭȫ���ж���Ϣ
    return 1;
}
/***************************************************************************//**
* ��    �ƣ�SPI_TxFrame()
* ��    �ܣ�3��Ӳ��SPIģʽ�£�����ָ����Ŀ���ֽڻ���
* ��ڲ�����pBuffer��ָ������͵������ַ
* 					size�������͵��ֽ���
* ���ڲ�����0����ǰӲ��SPI��æ��
* 					1����ǰ�����ѷ�����ϣ�
* ˵    ����ʹ�øú������Է���ָ��������һ֡����
* ʹ�÷�����SPI_TxFrame(CMD,6);	// ��CMD��ȡ��������6���ֽ�
 ******************************************************************************/
unsigned char SPI_TxFrame(unsigned char  *pBuffer, unsigned int  size)
{
	unsigned int  gie = __get_SR_register() & GIE;              //���浱ǰ ȫ�� GIE ״̬
//	_disable_interrupts();                                		 				 //��ֹ�ж�
    while (size--){
        while (!(IFG2 & UCA0TXIFG)) ;                   					 //�ȴ��������
        UCA0TXBUF = *pBuffer++;                       					 //д���д�ֽ�
    }
    while (UCA0STAT & UCBUSY) ;                     					 //�ȴ��շ����

    UCA0RXBUF;                                            	   					 //�ն�RXBUF��������һ��д����Ϣ
 //   _bis_SR_register(gie);                                					 //��ԭȫ���ж���Ϣ
    return 1;
}
