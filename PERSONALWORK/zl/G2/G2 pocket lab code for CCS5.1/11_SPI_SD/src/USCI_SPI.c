/*
 * USCI_SPI.c
 *
 *  Created on: 2013-4-3
 *      Author: Administrator
 */
#include"MSP430G2553.h"
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

//-----���巢��/���ջ���-----
unsigned char  *SPI_Tx_Buffer;
unsigned char  *SPI_Rx_Buffer;
//-----���������/���յ��ֽ���-----
unsigned char  SPI_Tx_Size=0;
unsigned char  SPI_Rx_Size=0;
//-----���巢��/����ģʽ��־-----
unsigned char SPI_Rx_Or_Tx =0;			// 0:������	1��������   2���շ�
/****************************************************************************
* ��    �ƣ�USCI_A0_init()
* ��    �ܣ���Ӳ��SPI���г�ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ��������ʹ�ú���Ķ�д�������ڳ���ʼ�����ȵ��øó�ʼ������
* ʹ�÷�����USCI_A0_init();
****************************************************************************/
void USCI_A0_init(void)
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
    //-----3-pin, 8-bit SPI ����ģʽ-----
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
/****************************************************************************
* ��    �ƣ�SPI_Interrupt_Sel()
* ��    �ܣ��������ͻ�����ж�
* ��ڲ�����onOff = 0 :�رշ����жϣ��򿪽����жϣ�
* 					onOff = 1 :�رս����жϣ��򿪷����жϣ�
* ���ڲ�������
* ˵    ���� ʹ�ô˺���������ѡ��ǰ�ն�ģʽ�����ں���������ж�
* ʹ�÷�����SPI_Interrupt_Sel(0);		// �رշ����жϣ��򿪽����жϣ�
* 					SPI_Interrupt_Sel(1);		// �رս����жϣ��򿪷����жϣ�
****************************************************************************/
void SPI_Interrupt_Sel(unsigned char onOff)
{
	if(onOff == 0)						// ֻ�������ж�
	{
		  IE2 &=~UCA0TXIE ;
		  IE2 |= UCA0RXIE ;
	}
	else	if(onOff==1)				// ֻ���������ж�
	{
		  IE2 &=~UCA0RXIE ;
		  IE2 |= UCA0TXIE ;
	}
	else										//�շ�ȫ��
	{
		 IE2 |= UCA0RXIE ;
		 IE2 |= UCA0TXIE ;
	}
}
/****************************************************************************
* ��    �ƣ�SPI_RecieveFrame()
* ��    �ܣ�3��Ӳ��SPIģʽ�£�����ָ����Ŀ���ֽ�
* ��ڲ�����pBuffer��ָ���Ž������ݵ�����
* 					size��Ҫ���յ��ֽ���
* ���ڲ�����0����ǰӲ��SPI��æ��
* 					1����ǰ�����ѷ�����ϣ�
* ˵    ����ʹ�øú������Խ���ָ��������һ֡����
* ʹ�÷�����SPI_RecieveFrame(CMD,6);// ����6���ֽڣ������η���CMD��
****************************************************************************/
unsigned char SPI_RecieveFrame(unsigned char  *pBuffer, unsigned int size)
{
	if(size==0)									return (1);
	if(UCA0STAT & UCBUSY)			return	(0);			// �ж�Ӳ��SPI��æ������0
	 _disable_interrupts();											// �ر����ж�
    SPI_Rx_Or_Tx = 0;													// ��������ģʽ
    SPI_Rx_Buffer = pBuffer;										// �����ͻ���ָ������͵������ַ
    SPI_Rx_Size = size-1;												// �����͵����ݸ���
    SPI_Interrupt_Sel(SPI_Rx_Or_Tx);							// SPI�жϿ���ѡ��
    _enable_interrupts();                                				// �����ж�
    UCA0TXBUF = 0xff;													// �ڽ���ģʽ�£�ҲҪ�ȷ���һ�ο��ֽڣ��Ա��ṩͨ��ʱ�ӡ�
    _bis_SR_register(LPM0_bits);									// ����͹���ģʽ0
	return (1);
}
/****************************************************************************
* ��    �ƣ�SPI_SendFrame()
* ��    �ܣ�3��Ӳ��SPIģʽ�£�����ָ����Ŀ���ֽڻ���
* ��ڲ�����pBuffer��ָ������͵������ַ
* 					size�������͵��ֽ���
* ���ڲ�����0����ǰӲ��SPI��æ��
* 					1����ǰ�����ѷ�����ϣ�
* ˵    ����ʹ�øú������Է���ָ��������һ֡����
* ʹ�÷�����SPI_SendFrame(CMD,6);	// ��CMD��ȡ��������6���ֽ�
****************************************************************************/
unsigned char SPI_SendFrame(unsigned char  *pBuffer, unsigned int  size)
{
	if(size==0)									return (1);
	if(UCA0STAT & UCBUSY)			return	(0);			// �ж�Ӳ��SPI��æ������0
    _disable_interrupts();											// �ر����ж�
    SPI_Rx_Or_Tx = 1;													// ��������ģʽ
    SPI_Tx_Buffer = pBuffer;										// �����ͻ���ָ������͵������ַ
    SPI_Tx_Size = size-1;												// �����͵����ݸ���
    SPI_Interrupt_Sel(SPI_Rx_Or_Tx);							// SPI�жϿ���ѡ��
    _enable_interrupts();	                               				// �����ж�
    UCA0TXBUF = *SPI_Tx_Buffer;								// �ȷ��͵�һ���ֽ��˹�������һ��"����"�ж�
	_bis_SR_register(LPM0_bits);									// ����͹���ģʽ0
	return (1);
}
//-----��ǰ�����¼�������-----
static void SPI_TxISR();
static void SPI_RxISR();
/******************************************************************************************************
 * ��       �ƣ�USCI0TX_ISR_HOOK()
 * ��       �ܣ���ӦTx�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ��������������ѭ��CPU�Ĵ���
 * ��       ������
 ******************************************************************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR_HOOK(void)
{
	//-----�����ж��¼����溯��-----
	SPI_TxISR();
	//-----�жϴ˴β����Ƿ���ɣ�������˳��͹���-----
	 if(SPI_Tx_Size==0)
	_bic_SR_register_on_exit(LPM0_bits);
}
/******************************************************************************************************
 * ��       �ƣ�USCI0RX_ISR_HOOK()
 * ��       �ܣ���ӦRx�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ��������������ѭ��CPU�Ĵ���
 * ��       ������
 ******************************************************************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
	//-----�����ж��¼����溯��-----
	 SPI_RxISR();
	//-----�жϴ˴β����Ƿ���ɣ�������˳��͹���-----
	 if(SPI_Rx_Size==0)
	_bic_SR_register_on_exit(LPM0_bits);
}
/******************************************************************************************************
 * ��       �ƣ�UART_OnRx()
 * ��       �ܣ�UART��Rx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����Խ��յ������ݣ�����Դ����д���
 * ��       ������
 ******************************************************************************************************/
static void SPI_RxISR()
{

	*SPI_Rx_Buffer = UCA0RXBUF;								//  ��ȡ���ջ��棬ͬʱ�����������UCA0RXIFG���жϱ�־λ
	if(SPI_Rx_Size!=0)
	{
		SPI_Rx_Size-- ;														// �����͵����ݼ�1
		SPI_Rx_Buffer++;												// ����ָ������һ�ֽ�ƫ��
		UCA0TXBUF = 0xff;												// ����Ϊ���ṩCLK��UCA0TXIFG��־λͬʱ�������
	}
    IFG2 &= ~UCA0TXIFG;                                   			// ��������жϱ�־λ

}
/******************************************************************************************************
 * ��       �ƣ�UART_OnTx()
 * ��       �ܣ�UART��Tx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����Խ��յ������ݣ�����Դ����д���
 * ��       ������
 ******************************************************************************************************/
static void SPI_TxISR()
{
    UCA0RXBUF;                                            				// ������ȫ˫��ͨ�ţ��շ�ͬʱ�����ԣ��˴���UCA0RXBUF�ղ��������������UCA0RXIFG���жϱ�־λ
	if(SPI_Tx_Size!=0)
	{
		SPI_Tx_Size-- ;														// �����͵����ݼ�1
		SPI_Tx_Buffer++;											// ����ָ������һ�ֽ�ƫ��
		UCA0TXBUF = *SPI_Tx_Buffer;							// ���뷢�ͻ��棬ͬʱ�����������UCA0TXIFG���жϱ�־λ
	}
	else
		IFG2 &= ~UCA0TXIFG;                                   		// ���һ�Σ����ڲ���UCA0TXBUF���в�������Ҫ��Ϊ��������жϱ�־λ
}
