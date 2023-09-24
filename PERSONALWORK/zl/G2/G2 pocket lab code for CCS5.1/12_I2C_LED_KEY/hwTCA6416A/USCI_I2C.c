/*
 * USCI_I2C.c
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */
#include"MSP430G2553.h"

#define TX_STATE 						0					/*I2C����״̬*/
#define RX_STATE						1					/*I2C����״̬*/
//-----��SMCLK��Ƶ����I2Cͨ�Ų�����-----
#define I2C_BAUDRATE_DIV 	14				/*I2C�����ʿ���*/
#define SLAVE_ADDR 				0x20				/*�ӻ�TCA6416A�ĵ�ַ*/
static unsigned char  TxByteCnt=0;				//ʣ�෢������
static unsigned char  RxByteCnt=0;				//ʣ���������
static unsigned char *pTxData;                     // ������TX ���ݵ�ָ��
static unsigned char *pRxData;                     // Rx���մ�����ݵ�ָ��
unsigned char I2C_State =  0;						//�շ�״ָ̬ʾ����
/******************************************************************************************************
 * ��       �ƣ�USCI_I2C_Init()
 * ��       �ܣ���ʼ��USCI_B0ģ��ΪI2Cģʽ
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����I2C��Ϊ3��������״̬���ݲ�ʹ��Tx��Rx�ж�
 * ��       ������
 ******************************************************************************************************/
void USCI_I2C_Init()
{
	 _disable_interrupts();
	  P1SEL |= BIT6 + BIT7;                      						// GPIO ����ΪUSCI_B0����
	  P1SEL2|= BIT6 + BIT7;                    						 // GPIO ����ΪUSCI_B0����
	  UCB0CTL1 |= UCSWRST;                      					 // �����λ״̬
	  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // ͬ��ͨ��I2C����״̬
	  UCB0CTL1 = UCSSEL_2 + UCSWRST;            		 // ʹ��SMCLK�������λ״̬
	  UCB0BR0 =I2C_BAUDRATE_DIV ;                          // ���˷�Ƶϵ����ʵ�ʲ����ʻ���SMCLK�й�
	  UCB0BR1 = 0;														//��һ����ķ�Ƶһ�㲻����
	  UCB0I2CSA = SLAVE_ADDR;                    			// I2C�ӻ���ַ�����ں궨�����޸�
	  UCB0CTL1 &= ~UCSWRST;                     				// ����I2C
	  _enable_interrupts();
}
/******************************************************************************************************
 * ��       �ƣ�USCI_I2C_Tx_Init()
 * ��       �ܣ���ʹ��I2C��Tx�ж�
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����I2Cͨ��ֻ�ܰ�˫����ֻʹ��һ���жϣ��ɿ�
 * ��       ������
 ******************************************************************************************************/
void USCI_I2C_Tx_Init()
{
	  _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // ȷ�����߿���
	  IE2 &= ~UCB0RXIE;							//�ر�Rx�ж�
	  I2C_State=TX_STATE;
	  IE2 |= UCB0TXIE;                          	    //����Tx�ж�
	  _enable_interrupts();						// �����ж�
}
/******************************************************************************************************
 * ��       �ƣ�USCI_I2C_Rx_Init()
 * ��       �ܣ���ʹ��I2C��Rx�ж�
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����I2Cͨ��ֻ�ܰ�˫����ֻʹ��һ���жϣ��ɿ�
 * ��       ������
 ******************************************************************************************************/
void USCI_I2C_Rx_Init()
{
	 _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // ȷ�����߿���
	  IE2 &= ~UCB0TXIE;							//�ر�Rx�ж�
	  I2C_State=RX_STATE;
	  IE2 |= UCB0RXIE;                          	    //����Tx�ж�
	  _enable_interrupts();						// �����ж�
}
/******************************************************************************************************
 * ��       �ƣ�PreSet_Tx_Frame()
 * ��       �ܣ�������������ݵġ����С�
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ֻ�в�BUSY��STOP�Ѹ�λ������²�������������С�
 * ��       ������
 ******************************************************************************************************/
unsigned char PreSet_Tx_Frame(unsigned char *p_Tx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
 	pTxData = (unsigned char *)p_Tx;     	// ��������ָ��
    TxByteCnt = num;                  					// ����ʣ�෢�����ݸ���
    UCB0CTL1 |= UCTR + UCTXSTT;            // I2C Txλ, ���start condition
    _bis_SR_register(CPUOFF+GIE);       		// ��LPM0ģʽ�������ж�
    return(1);
}
/******************************************************************************************************
 * ��       �ƣ�PreSet_Rx_Frame()
 * ��       �ܣ�������������ݵġ����С�
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ֻ�в�BUSY��STOP�Ѹ�λ������²�������������С�
 * ��       ������
 ******************************************************************************************************/
unsigned char PreSet_Rx_Frame(unsigned char *p_Rx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
	pRxData = (unsigned char *)p_Rx;     	// ��������ָ��
	 RxByteCnt= num;                  				// ����ʣ��������ݸ���
	 UCB0CTL1 &= ~UCTR;
     UCB0CTL1 |= UCTXSTT;                    		// I2C Rxλ, ���start condition
    _bis_SR_register(CPUOFF+GIE);       	    //  ��LPM0ģʽ�������ж�
    return(1);
 }
void I2C_TxFrame_ISR(void);
void I2C_RxFrame_ISR(void);
/******************************************************************************************************
 * ��       �ƣ�USCIAB0TX_ISR()
 * ��       �ܣ���ӦI2C���շ��жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����I2C��Tx��Rx�����ж����
 * ��       ������
 ******************************************************************************************************/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	 _disable_interrupts();							//��ͬ_DINT
  if(I2C_State==TX_STATE)							//�ж�����״̬���Ƿ�״̬
    I2C_TxFrame_ISR();                        			// �¼�������֡
  else
	I2C_RxFrame_ISR();                     			// �¼�������֡
  	//-------Ԥ������ѭ���л���CPU��;-------
  	  if(RxByteCnt == 0 || TxByteCnt == 0)	//���û�д����ͻ����������
  	    __bic_SR_register_on_exit(CPUOFF);   // Exit LPM0
  	 _enable_interrupts();							//��ͬ_ENIT
}
/******************************************************************************************************
 * ��       �ƣ�I2C_TxFrame_ISR()
 * ��       �ܣ�I2C��Tx�¼������������ͻ��������е�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ��������FIFO��������ָ������ѭ��
 * ��       ������
 ******************************************************************************************************/
void I2C_TxFrame_ISR(void)
{
   if (TxByteCnt)                            			// ��������Ƿ���
  {
    UCB0TXBUF = *pTxData;                   // װ�����������
    pTxData++;											//����ָ����λ
    TxByteCnt--;                            			// ���������ݸ����ݼ�
  }
  else														//���ݷ������
  {
    UCB0CTL1 |= UCTXSTP;                   	// ��ֹͣλ
    IFG2 &= ~UCB0TXIFG;                     	// �˹������־λ������û��дBuffer��MCU�����Զ������
  }
}
/******************************************************************************************************
 * ��       �ƣ�I2C_RxFrame_ISR()
 * ��       �ܣ�I2C��Rx�¼�����������ȡUCB0RXBUFд��ָ������������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������ƶ�FIFO��������ָ������ѭ��
 * ��       ������
 ******************************************************************************************************/
void I2C_RxFrame_ISR(void)
{
	  if (RxByteCnt == 1)                       			// ֻʣ1�ֽ�û����ʱ��ʵ���Ѿ���RxBuff���ˣ�
	      UCB0CTL1 |= UCTXSTP;                  		// �������ֹͣλ
	  RxByteCnt--;                              				// �������ֽ����ݼ�
	  *pRxData = UCB0RXBUF;                  		// �洢�ѽ��յ�����
	  *pRxData++;												//����ָ����λ
}
