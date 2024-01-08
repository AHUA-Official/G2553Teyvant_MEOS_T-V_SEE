/*
 * UART_Event.c
 * ���ܣ�ȫ�洦������λ��ͨ�ŵ��������
 *  Created on: 2013-3-18
 *      Author: Administrator
 */
#include "MSP430G2553.h"
#include "UARt_Global.h"
#include "UART_FIFO.h"
#include "UART_Event.h"
//-----����Ӳ���йصĴ���궨�崦��-----
#define LED1_ON P1DIR|=BIT0; P1OUT|=BIT0
#define LED1_OFF P1DIR|=BIT0; P1OUT&=~BIT0
#define LED2_ON P1DIR|=BIT6; P1OUT|=BIT6
#define LED2_OFF P1DIR|=BIT6; P1OUT&=~BIT6
//-----Ԥ����ROM�е���ʾ����-----
const unsigned char Out_DELETE[]= "\x8 \x8";  /* VT100 backspace and clear */
const unsigned char String1[]="����:LED1_ON LED1_OFF LED2_ON LED2_OFF\r\n";
const unsigned char String2[]="Please input Command:\r\n";
const  unsigned char String3[]="Are you crazy?\r\n";
const  unsigned char String4[]="I was born for these!\r\n";
const  unsigned char String5[]="I have got it!\r\n";
const  unsigned char String6[]="It is easy for me!\r\n";
const  unsigned char String7[]="As your wish!\r\n";

void Command_match();  // �ַ�ƥ�������
/******************************************************************************************************
 * ��       �ƣ�UART_OnTx()
 * ��       �ܣ�UART��Tx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����Tx_FIFO�������ݾͽ������Ƶ�Tx Buffer�Ĵ�����ȥ
 * ��       ������
 ******************************************************************************************************/
void UART_OnTx(void)
{
	unsigned char Temp;
	if(Tx_FIFO_DataNum>0)
	{
		Tx_FIFO_ReadChar(&Temp);	//����FIFO�⺯��
		UCA0TXBUF= Temp;
	}
}
/******************************************************************************************************
 * ��       �ƣ�UART_OnRx()
 * ��       �ܣ�UART��Rx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����Խ��յ������ݣ�����Դ����д���
 * ��       ������
 ******************************************************************************************************/
void UART_OnRx(void)
{
	unsigned char Temp = 0;
	Temp=UCA0RXBUF;			// Ԥ����Tx Buffer����
	//-----���ȱ��������Ļ-----
	if(Temp == 0x0d)				//����ǻس�
	{
		Tx_FIFO_WriteChar('\r');
		Tx_FIFO_WriteChar('\n');
	}
	else if(Temp==0x08 || Temp==0x7f)	//������˸�
	{
		UART_SendString(Out_DELETE);		// �����˸��
	}
	else										//�����������ʾ����
		Tx_FIFO_WriteChar(Temp);		//��������
	//-----�س���ʼ����֡ʶ��-----
	if(Temp == 0x0d)				// ����ǻس������������������˶ϡ���
	{
		if(Rx_FIFO_DataNum > 0)	//FIFO�������ݣ�����������ж�
		{
			Command_match();		//�ж�������ʲô
			Rx_FIFO_Clear();				//���FIFO
		}
		else{									//���ɶ���ݶ�û�У������˸��س���
			UART_SendString(String1);  	//��ʾ������ʾ��
			UART_SendString(String2);	//��ʾ������ʾ��
		}
	}
	//-----�˸����Ҫɾ��FIFO��һ������-----
	else if(Temp==0x08 || Temp==0x7f)	//������˸��������Ҫɾ��һ��
	{
		if( Rx_FIFO_DataNum>0)			//�����ݲ���Ҫɾ��û�����ݵ�Ȼ����ɾ
		{
			_disable_interrupts();			//����FIFOʱ������ж�
			Rx_FIFO_DataNum--;				//�������ݼ�1
			if(Rx_FIFO_IndexW >0)			//��ֹ���
				Rx_FIFO_IndexW--;				//дָ���˸�
			 _enable_interrupts();
		}
	}
	//-----�Ȳ��ǻس�Ҳ�����˸��Ǿ���������������-----
	else
	{
		Rx_FIFO_WriteChar(Temp); 			//����дFIFO
	}
}
/******************************************************************************************************
 * ��       �ƣ�UART_SendString()
 * ��       �ܣ���UART����һ���ַ���
 * ��ڲ�����*Ptr���ַ����׵�ַ
 * ���ڲ�������
 * ˵       �����ַ�������ܳ�������Tx_FIFO���ȣ���ᷢ������CPU
 * ��       ������
 ******************************************************************************************************/
void UART_SendString(const unsigned char *Ptr) //����λ�������ַ���
{
	while(*Ptr)
	{
		Tx_FIFO_WriteChar(*Ptr++);
	}
}
/******************************************************************************************************
 * ��       �ƣ�Command_match()
 * ��       �ܣ��Խ��յ����������ݽ���ƥ�䣬����ƥ��������LED�����Դ�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������4��Ԥ��Լ���������֣�LED1_ON,LED1_OFF,LED2_ON,LED2_OFF
 * ��       ������
 ******************************************************************************************************/
void Command_match()  // �ַ�ƥ������
{
	unsigned char Command_Num=0;
	//-----���4�֣�LED1_ON,LED1_OFF,LED2_ON,LED2_OFF-----
	if((Rx_FIFO[0]=='L')&&(Rx_FIFO[1]=='E')&&(Rx_FIFO[2]=='D')
			&&(Rx_FIFO[4]=='_')&&(Rx_FIFO[5]=='O'))//��ƥ�乲����ĸLED?_O??
			{
					if((Rx_FIFO[3]=='1')&&(Rx_FIFO[6]=='N'))
							Command_Num=1;		//ƥ����������LED1_ON
					if((Rx_FIFO[3]=='1')&&(Rx_FIFO[6]=='F')&&(Rx_FIFO[7]=='F'))
							Command_Num=2;		//ƥ����������LED1_OFF
					if((Rx_FIFO[3]=='2')&&(Rx_FIFO[6]=='N'))
							Command_Num=3;		//ƥ����������LED2_ON
					if((Rx_FIFO[3]=='2')&&(Rx_FIFO[6]=='F')&&(Rx_FIFO[7]=='F'))
							Command_Num=4;		//ƥ����������LED2_OFF
			}
	switch(Command_Num)
	{
	case 0: UART_SendString(String3);break;		//ûƥ�����κ�������ʹ�����ʾ��
	 //-----ִ��LED����������������������ʾ��-----
	case 1: LED1_ON;UART_SendString(String4);break;
	case 2: LED1_OFF;UART_SendString(String5);break;
	case 3: LED2_ON;UART_SendString(String6);break;
	case 4: LED2_OFF;UART_SendString(String7);break;
	default:break;
	}
}


