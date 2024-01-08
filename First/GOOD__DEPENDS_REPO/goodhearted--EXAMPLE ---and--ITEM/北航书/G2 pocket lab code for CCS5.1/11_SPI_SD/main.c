/*
 * main.c
 * ��ָ������д128�ֽ����ݣ��ٶ��ص�DATA[]����
 * ����ִֻ��һ�飬��CCS�в鿴����ֵ���˶��Ƿ���ȷ��дSD��
 */
#include"MSP430G2553.h"
#include"SD_SPI.h"
#include"USCI_SPI.h"
#define SD_SECTOR_ADDR  52334		//SD����������������ַ
#define SD_SECTOR_FIRST  30
#define SD_SECTOR_NUM  128
unsigned char DATA[128]={0};
void My_Write_Data();
void My_Read_Data();

 void main()
 {
	 unsigned char temp = 0;
	 WDTCTL = WDTPW + WDTHOLD;
	 //-----DCO��Ϊ12MHz----
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL = CALDCO_12MHZ;

	//-----�������ָʾ��-----
	P1DIR |= BIT0;
	P1OUT &=~BIT0;
	//-----��ʼ��Ӳ��SPI-----
	 USCI_A0_init();
	 // -----ֱ����ʼ���ɹ���һ��һ�ξͿɳ�ʼ���ɹ���-----
	do{
		temp = SD_Init();
	} while(temp == 0);
	//-----д��ָ��������128���ֽ�-----
	My_Write_Data();
	//-----ָ����������ȡ128���ֽ�-----
	My_Read_Data();
	 P1OUT |= BIT0;
	 _bis_SR_register(LPM3_bits);
 }
 /******************************************************************************************************
  * ��       �ƣ�My_Write_Data()
  * ��       �ܣ���SD����д128�ֽ���
  * ��ڲ�������
  * ���ڲ�������
  * ˵       �����˺���������ʾдSD��
  * ��       ������
  ******************************************************************************************************/
 void My_Write_Data()
 {
	 unsigned char i=0,temp=0;
	 unsigned char Temp[128]={0};
	 for (i=0;i<=127;i++)
	 {
		 Temp[i]=9;		// ����ʱ������ֵ
	 }
	do{ 						// һֱд��SD������ֱ���˴�д������ɹ���һ��һ�ξͿɳɹ���
		temp = SD_Write_Sector(SD_SECTOR_ADDR,Temp,SD_SECTOR_FIRST,SD_SECTOR_NUM);
	}while(temp ==0 );
}
 /******************************************************************************************************
  * ��       �ƣ�My_Write_Data()
  * ��       �ܣ���SD��ָ��λ���ж�128�ֽ���
  * ��ڲ�������
  * ���ڲ�������
  * ˵       �����˺���������ʾ��SD��
  * ��       ������
  ******************************************************************************************************/
void My_Read_Data()
 {
	 unsigned char i=0,temp=0;
	 for (i=0;i<127;i++)
	 {
		 DATA[i]=0;			//	�������������
	 }
	 do{							// һֱ��ȡSD������ֱ���˴ζ�ȡ�����ɹ���һ��һ�ξͿɳɹ���
		 temp = SD_Read_Sector(SD_SECTOR_ADDR ,DATA,SD_SECTOR_FIRST,SD_SECTOR_NUM);
	 }while(temp ==0);
 }
