/*
 * main.c
 */
#include "msp430g2553.h"
#include"SD_HardWare.h"
#include "SD_SPI.h"
#include"SPI.h"
#include "DAC8411.h"
#include "softFIFO.h"
//#define  WAV_SECTOR    532615			// TF���и���������������׵�ַ   532615��������
//#define  WAV_SECTOR    2642695		// TF���и���������������׵�ַ   2642695�����к�����һ��TF
#define  WAV_SECTOR    619	// TF���и���������������׵�ַ   672�����к���128M��TF
#define SYSCLK 12000000						//ϵͳʱ��
#define SAMPLE_FREQ 11025				//��Ƶ�ļ��Ĳ�����
#define VOL_REDUCE   1  					//ȡ2��N�η�������������
void Time0_A_Init();
 char SD_Read_InFIFO(unsigned long sector);
 void TA0_OnTime();
 void main()
 {
    WDTCTL = WDTPW + WDTHOLD;
 	BCSCTL1 = CALBC1_12MHZ;                   // Set range
 	DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation
 	__delay_cycles(100000);
 	//-----��ʼ��SPI-----
 	 SPI_init();
  	//-----��ʼ��SD-----
 	 SD_Init();
 	DAC8411_Init();
 	// -----��ʼ�������ʶ�ʱ��-----
 	Time0_A_Init();
 	while(1)
 	{
 		 SD_Read_InFIFO(WAV_SECTOR);			// ��ȡָ������
 	}
 }
 /******************************************************************************************************
  * ��       �ƣ�SD_Read_InFIFO(unsigned long sector)
  * ��       �ܣ���ͣ�Ķ�ȡSD������
  * ��ڲ�����ͷ������ַ
  * ���ڲ�������
  * ˵       ������SD���⺯����ͬ���ǣ���������ȡ����������FIFO��д��
  * ��       ������
  ******************************************************************************************************/
 char SD_Read_InFIFO(unsigned long sector)
 {
 	unsigned char CMD[6]={0};
 	static unsigned  long sec=1750;			// Ԥ��"����"��ȡƫ�ƣ��������ಿ��
    unsigned char retry = 0;
    unsigned char temp = 0;
    unsigned int i=0;
    sector = (sector+sec )<<9;					     //ת���߼���ַ
    //=====���²�����SD���⺯��һ��=====
    CMD[0]=0x51;	   									//SD_CMD17
    //-----ת��32λ������ַ-----
    CMD[1]= ((sector & 0xff000000) >>24);
    CMD[2]= ((sector & 0x00ff0000) >>16);
    CMD[3]= ((sector & 0x0000ff00) >>8);
    CMD[4]= sector & 0x000000ff;
    CMD[5]=0xFF;										    //SD_SPI_CRC
 	sec ++;				//��������
    //-----������д��SD�����100��ֱ��Ӧ��-----
       do
    {
       temp = Write_Command_SD(CMD);
       retry++;
       if(retry == 100)
       {
         return(0);
       }
    } while(temp!=0);
      while (SD_Read_Byte() != 0xfe) ;	//Ӧ����ȷ
    //=====���ϲ��ֺ�SD���⺯��һ�£����²��ָ�Ϊд���ݵ�FIFO=====
    for(i=0;i<512;i++)
    {
 	   while(FIFO_Count>= FIFOBUFF_SIZE)
 		   _NOP();															// FIFO �������ȴ�FIFO�е�����ȡ��
 	   Write_FIFO(SD_Read_Byte());							//������д��FIFO
    }
    //-----����ѭ������У��λ-----
    SD_Read_Byte();													//CRC - Byte
    SD_Read_Byte();													//CRC - Byte

 	SD_CS_High();														//CSƬѡ����
    return(1);
 }
//==============����Ϊ�������ͺ�̨��������Ϊǰ̨���򲿷�================
/******************************************************************************************************
 * ��       �ƣ�Time0_A_Init()
 * ��       �ܣ���ʼ��TA0
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������ǳ�ʼ�����IO��״̬
 * ��       ������
 ******************************************************************************************************/
void Time0_A_Init()
{
  TA0CCTL0 = CCIE;                         	// ����Ƚ�/����ģ��0���ж�
  TA0CCR0 = SYSCLK/SAMPLE_FREQ;		//�趨TA��ʱ����
  TA0CTL = TASSEL_2 + MC_1;           //TA0��Ϊ������ģʽ��ʱ��=SMCLK
  _enable_interrupts();                       //��ȫ�����жϣ�
}
/******************************************************************************************************
 * ��       �ƣ�Timer0_A0
 * ��       �ܣ�TACCR0�жϷ�����
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �������ж�Դ��ռ�ж�������ڣ������ж�
 * ��       ������
 ******************************************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	TA0CCTL0 &= ~CCIE;                       // �رձȽ�/����ģ��0���ж�
	TA0_OnTime();
	TA0CCTL0 = CCIE;                         // ����Ƚ�/����ģ��0���ж�
}
/******************************************************************************************************
 * ��       �ƣ� TA0_OnTime()
 * ��       �ܣ�TA0�Ķ�ʱ�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������
 * ��       ������
 ******************************************************************************************************/
void TA0_OnTime()
{
	unsigned int temp =0,tempL=0,tempH=0;
 	if(FIFO_Count != 0)
 	{
 		tempL =  Read_FIFO();
 		tempH =  Read_FIFO();
 		temp = ((tempH<<8) &0xff00)|(tempL &0x00ff);
		temp = temp+0x8000;	      //�з��������޷�����
 		write2DAC8411(temp/VOL_REDUCE);
 	}
}


