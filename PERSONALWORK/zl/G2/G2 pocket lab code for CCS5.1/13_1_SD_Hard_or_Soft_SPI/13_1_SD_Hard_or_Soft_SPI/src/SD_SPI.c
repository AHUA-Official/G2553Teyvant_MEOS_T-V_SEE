/*
 * SD_SPI.c
 *˵�����ú�����Ϊͨ��SD����д����⺯����
 * 			 �û�ֻ��Ҫ���ġ�SD_HardWare.c���е������ײ���룬������ֲ���κδ������ϡ�
 *  Created on: 2013-3-4
 *      Author: Administrator
 */
#include"SD_HardWare.h"
#include "SD_SPI.h"
#include"SPI.h"

//-----SD ����ص�����궨��-----
#define SD_WRITE_DELAY				100				// �ȷ�74�����ϵ�ʱ��
#define SD_EMPTY_CLK				0xFF
#define SD_EMPTY_DATA				0xFF
#define SD_SPI_CRC						0xFF				//SPIģʽ��CRC��Ч������
#define SD_CMD0_CRC					0x95				//��λ����CMD0��CRCУ����
#define SD_CMD0 						0+0x40		//��λ����
#define SD_CMD1 						1+0x40		//SPIģʽ����
#define SD_CMD17 						17+0x40		//����������
#define SD_CMD24						24+0x40		//д��������
#define SD_CMD41						41+0x40		//��ʼ����������
#define SD_CMD55						55+0x40		//��ʼ����������
#define SD_CMD59                        59+0x40

//-----SD�����Ӧ��-----
#define SD_COMMAND_ACK		0x00   			//����Ӧ��
#define SD_RESET_ACK					0x01				//��λӦ��
#define SD_DATA_ACK					0xFE				//����Ӧ��
#define SD_WRITE_ACK					0x05				//д����Ӧ��
//----ϵͳ��ʱ����----
//#define SD_TIMEOUT					100
//#define TIMEOUT							200
//#define SD_TIMEOUT					255
//#define TIMEOUT							255
#define SD_TIMEOUT					1000
#define TIMEOUT							1000
/************************************************************************************************************
* ��    �ƣ�Write_Command_SD()
* ��    �ܣ�SD����SPIģʽ�£���SD����д��6���ֽڵ�����
* ��ڲ�����CMD��ָ���������ֽ����������
* ���ڲ�����SD ��Ӧ��ֵ
* ˵    ����  ��SD����һ��д�������ֽڵ�����
* ʹ�÷�����Write_Command_SD(CMD);
************************************************************************************************************/
unsigned char Write_Command_SD(unsigned char *CMD)
{
   unsigned char tmp=0;
   unsigned int i = 0;
   SD_CS_High();
   //-----�ȷ�8����clock-----
   SD_Write_Byte(0xFF);
   SD_CS_Low();
   //-----д��6���ֽڵ�������֡-----
   SD_Write_Frame(CMD,6);
   //-----�յ�һ�Σ������Ե�һ������ֵ-----
   SD_Read_Byte();
   i = SD_TIMEOUT;
   do
   {
      tmp = SD_Read_Byte();
      i--;
   } while((tmp==0xff)&&i);
   return(tmp);
}
/************************************************************************************************************
* ��    �ƣ�SD_Write_Sector
* ��    �ܣ�SD����SPIģʽ�£���SD���У���ָ����λ��д��ָ�����������ݡ�
* ��ڲ�����Addr�����������ĵ�ַ
* 					*Ptr��ָ���д���ݵ����ݻ���
* 					FirstNum��д����������׸��ֽ�ƫ�Ƶ�ַ
* 					Num���˴β���Ҫд������ݸ���				(FirstNum+Num<512)
* ���ڲ�����0��д��ʧ��
* 					1��д��ɹ�
* ʹ�÷�����SD_Write_Sector(83241,buffer,0,100); 	 //������83241��д��100���ֽ�
************************************************************************************************************/
unsigned char SD_Write_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num)
{
	unsigned char temp=0;
	unsigned int EndNum = 0;
	unsigned int i=0;
	unsigned char CMD[6]={0};
	//-----��������-----
	CMD[0]=SD_CMD24;
	//-----��32λ��ַ���------
	Addr = Addr <<9;													//sector = sector*512   �������ַת��Ϊ�߼���ַ��(ע��һ����Ҫ�˴���ת��)
	CMD[1]=((Addr&0xFF000000)>>24);
	CMD[2]=((Addr&0x00FF0000)>>16);
	CMD[3]=((Addr&0x0000FF00)>>8);
	CMD[4]=Addr&0x000000FF;
	//-----CRCУ�飬SPIģʽ����Ч��������-----
	CMD[5]=SD_SPI_CRC;
	//----�ȴ�SD��Ӧ��----
	i=TIMEOUT;															//��ʱ����ֵ
	do{
		temp = Write_Command_SD(CMD);
		 i--;
	}while((temp != SD_COMMAND_ACK)&&i);		//��ʱ�ж�
	if (i==0) 	return(0);													//��ʱ������ʧ�ܴ���
	//----�շ����ɴ�CLK----
	for(i=0;i<SD_WRITE_DELAY;i++)
	{
		SD_Write_Byte(SD_EMPTY_CLK);
	}
	//SD_Write_Frame(pTemp,SD_WRITE_DELAY);
	//-----��������ͷ�ֽ�-----
	SD_Write_Byte(SD_DATA_ACK);
//	//-----����512�ֽ����ݣ���������----
	EndNum=FirstNum+Num;

	for(i=0;i<FirstNum;i++)
	{
		//-----д������-----
		SD_Write_Byte(SD_EMPTY_DATA	);
	}
	SD_Write_Frame(Ptr,Num);				//д��Ч����
	for(i=0;i<512-EndNum;i++)
	{
		//-----д������-----
		SD_Write_Byte(SD_EMPTY_DATA	);
	}
	//-----���������ֽڵ�У��λ-----
	SD_Write_Byte(0xff);
	SD_Write_Byte(0xff);

	//-----�ж�SD��Ӧ���Ƿ�Ϊxxx00101b-------
	temp=SD_Read_Byte();
	temp =temp &0x01F;
	if(temp != SD_WRITE_ACK)
	{
		SD_CS_High();
		return(0);
	}
	//-------һֱ��CLK��ֱ����BUSY------
	i=TIMEOUT;														//��ʱ����ֵ
	do{
			 i--;
	}while(SD_Read_Byte() != 0xff &&i);					//��ʱ�ж�
	if (i==0)
	{
		SD_CS_High();
		return(0);															//��ʱ������ʧ�ܴ���
	}
	//---�ɹ����д���ݣ���ƬѡCS����------
	SD_CS_High();
	return 1;
}
/************************************************************************************************************
* ��    �ƣ�SD_Read_Sector
* ��    �ܣ�SD����SPIģʽ�£���SD���У���ָ����λ�ô�������ָ�����������ݵ����档
* ��ڲ�����Addr�����������ĵ�ַ
* 					*Ptr��ָ���Ŷ�ȡ���ݵ����ݻ���
* 					FirstNum���Ӹ�������ȡ�׸��ֽڵ�ƫ�Ƶ�ַ
* 					Num���˴β���Ҫ��ȡ�����ݸ���				(FirstNum+Num<512)
* ���ڲ�����0����ȡʧ��
* 					1����ȡ�ɹ�
* ʹ�÷�����SD_Read_Sector(83241,buffer,0,100); 	 //������83241�ж���100���ֽڣ�����buffer��
************************************************************************************************************/
unsigned char SD_Read_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num)
{
	unsigned char temp=0;
	unsigned int i=0;
	unsigned int EndNum=0;
	unsigned char CMD[6]={0};

	//-----����17-----
	CMD[0]=SD_CMD17;
	//-----��32λ��ַ���------
	Addr = Addr <<9;												//sector = sector*512   �������ַת��Ϊ�߼���ַ��
	CMD[1]=((Addr&0xFF000000)>>24);
	CMD[2]=((Addr&0x00FF0000)>>16);
	CMD[3]=((Addr&0x0000FF00)>>8);
	CMD[4]=Addr&0x000000FF;
	//-----CRCУ�飬SPIģʽ����Ч��������-----
	CMD[5]=SD_SPI_CRC;

	//----�ȴ�SD��Ӧ��----
	i=TIMEOUT;														//��ʱ����ֵ
	do{
	      temp = Write_Command_SD(CMD);
		 i--;
	}while((temp!=SD_COMMAND_ACK)&&i);		//��ʱ�ж�
	if (i==0)
	{
		SD_CS_High();
		return(0);															//��ʱ������ʧ�ܴ���
	}
	//----�ȴ�SD��Ӧ��--------------
	i=TIMEOUT;														//��ʱ����ֵ
	do{
		 temp = SD_Read_Byte();
		 i--;
	}while(( temp !=SD_DATA_ACK)&&i);				//��ʱ�ж�
	if (i==0)
	{
		SD_CS_High();
		return(0);															//��ʱ������ʧ�ܴ���
	}
	//-----��ȡָ���ֽ�-----
	EndNum=FirstNum+Num;
	for(i=0;i<FirstNum;i++)
	{
		//-----�ն��������洢�������ֽ�-----
			SD_Read_Byte();
	}
	SD_Read_Frame(Ptr,Num);			//��ȡ��Ч�ֽ�
	for(i=0;i<512-EndNum;i++)
	{
		//-----�ն��������洢�������ֽ�-----
			SD_Read_Byte();
	}
	//-----�ն����� CRC--Byte-----
	 SD_Read_Byte();
	 SD_Read_Byte();
	SD_CS_High();
	return (1);
}
/************************************************************************************************************
* ��    �ƣ�SD_Init
* ��    �ܣ���ʼ��SD��ΪSPIģʽ
* ��ڲ�������
* ���ڲ�����0����ʼ��ʧ��
* 					1����ʼ���ɹ�
* ˵    ����  ��ʼ��SD��ΪSPIģʽ
* ʹ�÷�����SD_Init();
************************************************************************************************************/
unsigned char SD_Init()
{
	unsigned char temp =0 ;
	//-----��ʼ��֮ǰ���Ƚ�SPI��Ϊ���٣�SPI_clk=300K���ң�-----
	SD_Low_Speed();
	//-----��λSPI-----
	temp = SD_Reset();
	if(temp==0)
		return(0);
	//-----�趨SDΪSPI��ȡ��ģʽ-----
	temp = SD_Set_SPI();
	if(temp==0)
		return(0);
	//--��ʼ������Ժ󣬽�SPI�ٶ��ᵽ���٣�10Mhz���ң����������ڿ��ٶ�д����-----
	SD_High_Speed();
	return(1);
}
/************************************************************************************************************
* ��    �ƣ�SD_Reset()
* ��    �ܣ���λSD��
* ��ڲ�������
* ���ڲ�����0����λSDʧ��
* 					1����λSD�ɹ�
* ˵    ����  �ϵ��SD��λSDģʽ�����ȸ�λ���������ΪSPIģʽ
* ʹ�÷�������
************************************************************************************************************/
unsigned char SD_Reset()
{
	unsigned int i=0,temp = 0;
	unsigned char CMD[6] = {SD_CMD0,0x00,0x00,0x00,0x00,SD_CMD0_CRC};
	SD_CS_High();																	//CSƬѡʹ��
	for(i=0;i<0x0f;i++)															//�շ�80��CLK(10�ֽڣ�
	{
		SD_Write_Byte(SD_EMPTY_CLK);
	}

	i=TIMEOUT;																	//��ʱ����ֵ
	do{
		temp = Write_Command_SD(CMD);							//CMD0
		i--;
	}while((temp != SD_RESET_ACK) && i);						//��ʱ�ж�
	if(i==0)
		return(0);																	//��ʱ������ʧ�ܴ���
	else
		return (1);
}
/************************************************************************************************************
* ��    �ƣ�SD_Set_SPI()
* ��    �ܣ���SD����ΪSPIģʽ
* ��ڲ�������
* ���ڲ�����0����λSDʧ��
* 					1����λSD�ɹ�
* ˵    ����  ���ȸ�λ��������ܽ�������SPIģʽ����
* ʹ�÷�������
************************************************************************************************************/
unsigned char SD_Set_SPI()
{
	unsigned int i=0,temp=0;
	unsigned char CMD[6]={0};
	//-----�������Ϊ4���ֽ�-----
	CMD[0]= SD_CMD1;
	CMD[1]= ((0x00ffc000 & 0xff000000) >>24);
	CMD[2]= ((0x00ffc000 & 0x00ff0000) >>16);
	CMD[3]= ((0x00ffc000 & 0x0000ff00) >>8);
	CMD[4]= 0x00ffc000 & 0x000000ff;
	CMD[5]= SD_SPI_CRC;

	i=TIMEOUT;																			//��ʱ����ֵ
	do{
	       CMD[0] = SD_CMD55;                  									//CMD55����
	       temp = Write_Command_SD(CMD);   							//�ȷ��� CMD55
	       //-----�ж��Ƿ�ΪSD��-----
	       if(temp == 0x01)                 											//����з�Ӧ
	       {
//		//-----�������жϴ洢�����ͣ�ֱ��ִ���߼����-----
				 CMD[0] = SD_CMD1;                 								//CMD41����
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD); 						//����CMD41���м���

				 CMD[0] = SD_CMD59;                 								//CMD41����
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD); 						//����CMD41���м���

				 _nop();
				 if(temp == 0x00)
				 {
						SD_CS_High();														//CSƬѡ����
				 }
//		//---------------------------------------------------
       }
	       //-----����ΪMMC��-----
	       else                             														//�������CMD55�޷�Ӧ���ķ���CMD1
	       {
				 CMD[0] = SD_CMD1;                 								//CMD1����
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD);    					//����CMD1���м���
				 if(temp == 0x00)
				 {
						SD_CS_High();														//CSƬѡ����
				 }
	       }
		i--;
	}while((temp !=SD_COMMAND_ACK)&&i);							//��ʱ�ж�
	if(i==0)
		return(0);																			//��ʱ������ʧ�ܴ���
	else
		return (1);
}



