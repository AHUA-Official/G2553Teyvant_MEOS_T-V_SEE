/*
 * softFIFO.c
 *
 *  Created on: 2013-3-12
 *      Author: wangJingbing
 */

#include "msp430g2553.h"
#include "softFIFO.h"
#define DISABLE_INT				_DINT()		// ���ж�
#define RESTORE_INT			_EINT()			// ���ж�

/***********************���º궨����FIFO�������й�******************************/
unsigned char FIFOBuff[FIFOBUFF_SIZE]={0}; 	//���建���������(FIFO)
static unsigned int 	FIFO_IndexW;          	//�������д��ָ��(ͷָ��)
static unsigned int 	FIFO_IndexR;          		//������ж�ȡָ��(βָ��
unsigned int 	FIFO_Count;           					//FIFO��δ��ȡ���ݵĸ���
/****************************************************************************
* ��    �ƣ�Write_FIFO()
* ��    �ܣ���һ��ֵѹ����Ӧ���̻������
* ��ڲ�����value:����
* ���ڲ�������
****************************************************************************/
void Write_FIFO(unsigned char value)
{
  if(FIFO_Count>=FIFOBUFF_SIZE) return;//����������������������ֵ
  DISABLE_INT;
  FIFO_Count++;                       //������������
  FIFOBuff[FIFO_IndexW] = value;   //�Ӷ���ͷ��׷���µ�����
  if (++FIFO_IndexW >=  FIFOBUFF_SIZE) //ѭ�����У��������ͷָ��Խ��
   {
	  FIFO_IndexW = 0;                  //����ͷָ��ص�������ʼλ��
   }
  RESTORE_INT;
}
/****************************************************************************
* ��    �ƣ�Read_FIFO()
* ��    �ܣ��ӻ�������ڶ�ȡһ��ֵ
* ��ڲ�������
* ���ڲ�����value������
* ˵    ��: ����һ�θú��������Զ�ɾ�����������һ�����ݡ�
****************************************************************************/
unsigned char Read_FIFO()
{
	unsigned char value;
  if(FIFO_Count==0)  return(0);     	//�������ݣ�����0
  //DISABLE_INT;								//���в��������в�������ʱ�ж�ִ��ScanIO����������ȫ�ֱ�����ȷ
  FIFO_Count--;                     			//��������������1
  value=FIFOBuff[FIFO_IndexR];    		//�ӻ�����β����ȡһ������
  if (++FIFO_IndexR >= FIFOBUFF_SIZE)	//ѭ�����У��������βָ��Խ��
     {
	  FIFO_IndexR = 0;             			//����βָ��ص�������ʼλ��
     }
  //RESTORE_INT;                     				//�ָ��ж�����
  return(value);
}



