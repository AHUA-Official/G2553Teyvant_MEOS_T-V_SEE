/*
 * Flash.c
 * ʵ��������MSP430x2xxϵ�е�Ƭ������������10�����ù��ܺ�����
(1)��ʼ����Flash_Init(unsigned char Div,unsigned char Seg )������SMCLKƵ�ʼ����趨Flash��ʱ�ӵķ�Ƶϵ������Seg�κ���ȷ���ƻ������Ķ���ʼ��ַ��
(2)���β�����Flash_Erase()���β���������
(3)���ֽڡ�Flash_ReadChar(unsigned int Addr)����ȡƫ�Ƶ�ַAddrλ��1���ֽڵ����ݡ�
(4)���֡�Flash_ReadWord(unsigned int Addr)����ȡƫ�Ƶ�ַAddrλ��1���ֵ����ݡ�
(5)��һ���ֽڵ�RAM���顣Flash_ReadSeg(unsigned int Addr, unsigned int SegSize,char * Array)����ȡ��ʼƫ�Ƶ�ַΪAddr������SegSize���ֽ����ݵ�RAM��Array���顣
(6)ֱ��д1���ֽڡ�Flash_Direct_WriteChar(unsigned int Addr)��ֱ��дƫ�Ƶ�ַAddrλ��1���ֽڵ����ݡ�
(7)ֱ��д1���֡�Flash_Direct_WriteWord(unsigned int Addr)��ֱ��дƫ�Ƶ�ַAddrλ��1���ֵ����ݡ�
(8)���ݺ�д1�ֽڡ�Flash_Bak_WriteChar(unsigned int Addr)���ȱ��ݶ����������ݣ���д����ƫ�Ƶ�ַAddrλ��д1���ֽڵ����ݣ��ٻ�ԭ�����������ݡ���������Ϣflash�Σ�ʹ��RAM���ݣ�
(9)���ݺ�д1���֡�Flash_Bak_WriteWord(unsigned int Addr)���ȱ��ݶ����������ݣ���д����ƫ�Ƶ�ַAddrλ��д1���ֵ����ݣ��ٻ�ԭ�����������ݡ���������Ϣflash�Σ�ʹ��RAM���ݣ���
(10)��SegAר�ú�����Flash_SegA_ReadChar(unsigned int Addr)����ȡSegA��ƫ�Ƶ�ַAddrλ��1���ֽڵ����ݡ�
˵���� 1����д������Ҫ��RAM�е��ú���ָ����ʹ�ã����⺯��δ�漰
           2���������ֽڵ��������Ͷ�д��ʹ�ýṹ�壬���⺯��δ�漰
           3�����к���������޷����������ݣ�����ʹ���з������Σ����޸ĺ���
           5����InfoA�ε�������ֻ�ж��ֽں���Flash_SegA_ReadChar()�����ṩ��д������
           6�����������Ķβ����׵�ַSegAddr��Flash_Init()�������޶��������׷�����д
 *
 *  Created on: 2013-2-18
 *   Author: Administrator
 */
#include  "MSP430G2553.h"
unsigned int SegAddr=0;						//ȫ�ֱ���
unsigned int SegPre=0;						//ȫ�ֱ��� ��ǰ��Ϣ��
/******************************************************************************************************
* ��   	 �ƣ�Flash_Init()
* ��  	 �ܣ���Flashʱ�ӽ��г�ʼ������
* ��ڲ�����Div������SMCLKƵ�ʼ���������Ƶֵ�����趨Ϊ1-64
* 					Seg:�κţ�����Ϊ"0"-"31"��"A"��"B"��"C"��"D"��
* ���ڲ�����1�����óɹ�
*                   0������ʧ��
* ˵   	 ��������Flash��������ǰ����Ҫ���øó�ʼ����������ʱ�ӷ�Ƶ�ʹ��������׵�ַ��
*                   ���������о������־��Ե�ַ����ֹ�������
* ��       ���� Flash_Init(3,'B' ) 3��Ƶ����Info B�β���
 ******************************************************************************************************/
unsigned char Flash_Init(unsigned char Div,unsigned char Seg )
{
	//-----����Flash��ʱ�Ӻͷ�Ƶ����ƵΪǡ��Ϊ���λ��ֱ����Div-1����-----
	if(Div<1) Div=1;
	if(Div>64) Div=64;
	FCTL2 = FWKEY + FSSEL_2 + Div-1; 					// Ĭ��ʹ��SMCLK����Ƶϵ����������
	//-----��������Ϊ��Flash�ε��������ͨ��512�ı������ö���ʼ��ַ-----
	SegPre = Seg;														//��ȡ��ǰ��
	if (Seg <= 31) 													//�ж��Ƿ�����Flash��
		{
		SegAddr=0xFFFF-(Seg+1)*512+1;					//�������ʼ��ַ
		return(1);														//��ֵ�ɹ��󼴿��˳������سɹ���־��1��
		}
	//-----��������Ϊ��ϢFlash�ε��������ټ���-----
	switch(Seg)														//�ж��Ƿ�����ϢFlash��
	{
	case 'A':	case'a': 	SegAddr=0x10C0; break;
	case 'B':	case'b': 	SegAddr=0x1080; break;
	case 'C':	case'c': 	SegAddr=0x1040; break;
	case 'D':	case'd': 	SegAddr=0x1000; break;
	default: 	SegAddr=0x20FF;	return(0);			//0x20FF��ַΪ�հ���������Flash
	}
	return(1);
}
/******************************************************************************************************
* ��       �ƣ�Flash_Erase()
* ��       �ܣ�����Flash��һ�����ݿ飬��д���ɳ�ʼ������ Flash_Init()��SegAddr��������
* ��ڲ�������
* ���ڲ�������
* ˵       ���������и����˲���InfoFlashA�εĲ������루��ע�͵��ˣ������������ѧ��ʹ�á�
* ��       ������
 ******************************************************************************************************/
void Flash_Erase()
{
  unsigned char *Ptr_SegAddr;                   			// Segment  pointer
  Ptr_SegAddr = (unsigned char *)SegAddr;  		// Initialize Flash  pointer
  FCTL1 = FWKEY + ERASE;                    					// �β���ģʽ
  FCTL3 = FWKEY;                            						// ����
  //FCTL3 = FWKEY+LOCKA;                          		    // ��InfoFlashAҲ����
  _disable_interrupts();
  *Ptr_SegAddr = 0;                         						// ������������
  while(FCTL3&BUSY);                           					//Busy
  _enable_interrupts();
  FCTL1 = FWKEY;                           							// ȡ����ģʽ
  FCTL3 = FWKEY+LOCK;                      	 				// ����
  //FCTL3 = FWKEY+LOCK+LOCKA;                       // ��InfoFlashAҲ����
}
/******************************************************************************************************
* ��    �ƣ�Flash_ReadChar()
* ��    �ܣ���Flash�ж�ȡһ���ֽ�
* ��ڲ�����Addr:������ݵ�ƫ�Ƶ�ַ
* ���ڲ��������ص����ݣ���ƫ�����ʱ����0
* ˵       ������
* ��       ������
 ******************************************************************************************************/
unsigned char Flash_ReadChar (unsigned int Addr)
{
  unsigned char Data=0;
  unsigned int *Ptr_SegAddr,temp=0;                    			// Segment  pointer
  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
  if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
  temp =SegAddr+Addr;
  Ptr_SegAddr =(void*)temp;  		 				// Initialize Flash  pointer
  Data=*(Ptr_SegAddr);
  return(Data);
}
/******************************************************************************************************
* ��       �ƣ�Flash_ReadWord()
* ��       �ܣ���FlashROM����һ�����ͱ�������ַӦΪż��
* ��ڲ�����Addr:������ݵ�ƫ�Ƶ�ַ���԰��ֽڼ��㣬��Ϊż��
* ���ڲ��������ص����ͱ���ֵ  ����ƫ�����ʱ����0
* ˵       ������
* ��       ������
 ******************************************************************************************************/
unsigned int Flash_ReadWord (unsigned int Addr)
{
  unsigned int *Ptr_SegAddr;
  unsigned int temp=0,Data=0;                    // Segment  pointer
  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
  if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
  temp = SegAddr+Addr;
    Ptr_SegAddr = (void *)temp;  					// Initialize Flash pointer
  Data=*(Ptr_SegAddr);
  return(Data);
}
/******************************************************************************************************
* ��    �ƣ�Flash_ReadSeg()
* ��    �ܣ���Flash����һ�����ݿ�����RAM��Array����
* ��ڲ�����Addr����ʼƫ�Ƶ�ַ
* 					SegSize�����ݸ���
* 					*Array��RAM�������ͷָ��
* ���ڲ��������س�����Ϣ 0:ƫ����� ��1:��������
* ˵       ������
* ��       ������
 ******************************************************************************************************/
char Flash_ReadSeg(unsigned int Addr, unsigned int SegSize,unsigned char * Array)
{
	unsigned int i=0,temp=0;
	unsigned char *Ptr_SegAddr;                    		// Segment  pointer
	  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if((SegPre<=31&&(Addr+SegSize)>512) ||(SegPre>31&&(Addr+SegSize)>64) )
	  return 0;
	for(i=0;i<SegSize;i++)
	{
		temp=SegAddr+Addr+i;								// ��ֹ����������ָ��ƫ�Ƴ���
		Ptr_SegAddr = (unsigned char *)temp;  	   	// Initialize Flash  pointer
		Array[i]=*Ptr_SegAddr;									//ָ����λ������ֵ
	}
	  return 1;
}
/******************************************************************************************************
* ��    �ƣ�Flash_Direct_WriteChar()
* ��    �ܣ�ǿ����Flash��д��һ���ֽ�(Char�ͱ���)���������Ƿ�Ϊ��
* ��ڲ�����Addr:������ݵ�ƫ�Ƶ�ַ
            		Data:��д�������
* ���ڲ��������س�����Ϣ 0:ƫ����� ��1:��������
* ��    ����Flash_Direct_WriteChar(0,123);������123д��0��Ԫ
            	 Flash_Direct_WriteChar(1,a);�����ͱ���aд��1��Ԫ
 ******************************************************************************************************/
char Flash_Direct_WriteChar (unsigned int Addr,unsigned char Data)
{
	unsigned int temp=0;
	unsigned char *Ptr_SegAddr;                 	// Segment  pointer
	  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
	temp = SegAddr+Addr;
	Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	FCTL1=FWKEY+WRT;									//����д״̬
	FCTL3=FWKEY;											//�������
//	FCTL3=FWKEY+LOCKA;								//�������������A�Σ�
	_disable_interrupts();														//�����ж�
	*Ptr_SegAddr=Data;									//ָ����ַ��д1�ֽ�
	while(FCTL3&BUSY);									//�ȴ��������
	_enable_interrupts();														//�����ж�
	FCTL1=FWKEY;											//�˳�д״̬
	FCTL3=FWKEY+LOCK;								//�ָ���������������
//	FCTL3=FWKEY+LOCK+LOCKA;					//�ָ��������������ݣ�����A�Σ�
	 return 1;
}
/******************************************************************************************************
* ��    �ƣ�Flash_Direct_WriteWord()
* ��    �ܣ�ǿ����Flash��д��һ�����ͱ����������ܴ洢λ���Ƿ����Ȳ���
* ��ڲ�����Addr:������ݵ�ƫ�Ƶ�ַ���԰��ֽڼ��㣬��Ϊż��
            		Data:��д�������
* ���ڲ��������س�����Ϣ 0:ƫ����� ��1:��������
* ��    ����Flash_Direct_WriteWord(0,123);������123д��0��Ԫ
            	 Flash_Direct_WriteWord(2,a);�����ͱ���aд��2��Ԫ
 ******************************************************************************************************/
char Flash_Direct_WriteWord (unsigned int Addr,unsigned int Data)
{
	unsigned int temp=0;
	unsigned int *Ptr_SegAddr;                    	// Segment  pointer
	  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
	temp=SegAddr+Addr;
	Ptr_SegAddr = (unsigned int *)temp;  	   	// Initialize Flash  pointer
	FCTL1=FWKEY+WRT;									//����д״̬
	FCTL3=FWKEY;											//�������
//	FCTL3=FWKEY+LOCKA;								//�������������A�Σ�
	_disable_interrupts();														//�����ж�
	*Ptr_SegAddr=Data;									//д16λ��
	while(FCTL3&BUSY);									//�ȴ��������
	_enable_interrupts();														//�����ж�
	FCTL1=FWKEY;											//�˳�д״̬
	FCTL3=FWKEY+LOCK;								//�ָ���������������
//	FCTL3=FWKEY+LOCK+LOCKA;					//�ָ��������������ݣ�����A�Σ�
	 return 1;
}
/******************************************************************************************************
* ��    �ƣ�Flash_Bak_WriteChar()
* ��    �ܣ����ƻ������������ݣ���Flash��д��һ���ֽ�(Char�ͱ���)
* ��ڲ�����Addr:������ݵĵ�ַ
            		Data:��д�������
* ���ڲ��������س�����Ϣ 0:ƫ����� ��1:��������
* ��    ����Flash_Bak_WriteChar(0,123);������123д��0��Ԫ
            	 Flash_Bak_WriteChar(1,a);������aд��1��Ԫ
 ******************************************************************************************************/
char Flash_Bak_WriteChar (unsigned char Addr,unsigned char Data)
{
	unsigned int temp=0;
	unsigned char *Ptr_SegAddr;                    	// Segment  pointer
	unsigned char BackupArray[64];					//����64�ֽڵ���ʱRAM����Seg
	unsigned char i = 0;
	  //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if((SegPre<=31&&Addr>=512) || (SegPre>31&&Addr>64) )
	  return 0;
	for(i=0;i<64;i++)
	{
	  temp=SegAddr+i;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	  BackupArray[i]=*Ptr_SegAddr;					//ָ����λ������ֵ
	}
	Flash_Erase();  												//������������
	FCTL1 = FWKEY + WRT;                      			// ����д�루�ǿ�д��
	FCTL3 = FWKEY ;                            					// ����
//	FCTL3 = FWKEY ;                            					// ��������A�Σ�
	for (i=0; i<64; i++)
	{
	_disable_interrupts();															//�����ж�
	if(i==Addr)
	{
	  temp=SegAddr+Addr;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	 *Ptr_SegAddr =Data;                    					// д����
	 while(FCTL3&BUSY);                           			// �ȴ�д�������
	 }
	else
	{
	  temp=SegAddr+i;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	 *Ptr_SegAddr = BackupArray[i];           		// �ָ�Flash�ڵ���������
	 while(FCTL3&BUSY);                             		// �ȴ�д�������
	}
	_enable_interrupts();										// �����ж�
	}
	FCTL1 = FWKEY;                           					// ���д
	FCTL3 = FWKEY + LOCK;                     			// ����
// FCTL3 = FWKEY + LOCK;                     			// ��������A�Σ�
	 return 1;
}
/******************************************************************************************************
* ��       �ƣ�Flash_Bak_WriteWord()
* ��       �ܣ����ƻ������������ݣ���Flash��д��һ����(int�ͱ���)
* ��ڲ�����Addr:������ݵĵ�ַ����Ȼ�����ֽ�Ϊ��λ��ƫ�Ƶ�ַ����Ϊż��
            		Data:��д�������
* ���ڲ��������س�����Ϣ 0:ƫ����� ��1:��������
* ˵       ����MSP430��Ƭ�����Զ�16λ����ֱ�Ӳ���������Ϊ�˼ӿ��ٶ�
*                 �����о�ֱ�Ӷ�word���в�����
* ��       ����Flash_Bak_WriteWord(0,123);������123д��0��Ԫ
            	 Flash_Bak_WriteWord(1,a);������aд��1��Ԫ
 ******************************************************************************************************/
char Flash_Bak_WriteWord(unsigned int Addr,unsigned int Data)
{
	unsigned int *Ptr_SegAddr;                    			// Segment  pointer
	Ptr_SegAddr = (unsigned int *)SegAddr;  	   	// Initialize Flash  pointer
	//-----ע�⣺���²�����ȫ��Ϊword16λ��������-----
	unsigned int BackupArray[32];							//����32��(64�ֽڣ�����ʱRAM����Seg
	unsigned int i = 0;
	 //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>64) )
	return 0;
	for(i=0;i<32;i++)												//	word��ռ�����ֽ�
	{
	  BackupArray[i]= *(Ptr_SegAddr+i);					//ָ����λ�������ָ�ֵ
	}
	Flash_Erase();  													//������������
	FCTL1 = FWKEY + WRT;                      				// ����д�루�ǿ�д��
	FCTL3 = FWKEY;                            						// ����
// FCTL3 = FWKEY+LOCKA;                            		// ����	����A�Σ�
	for (i=0; i<32; i++)												//	word��ռ�����ֽڣ�������������ַ
	{
	_disable_interrupts();										//�����ж�
	if(i==Addr)
	{
	 *(Ptr_SegAddr+Addr) =Data;  							// д��������
	 while(FCTL3&BUSY);                           				// �ȴ�д�������
	 }
	else
	{
	 *(Ptr_SegAddr+i)= BackupArray[i];           		// �ָ�Flash�ڵ��������ݣ����ָֻ�
	 while(FCTL3&BUSY);                             			// �ȴ�д�������
	}
	_enable_interrupts();											// �����ж�
	}
	FCTL1 = FWKEY;                           						// ���д
	FCTL3 = FWKEY + LOCK;                     				// ����
//	FCTL3 = FWKEY + LOCK+LOCKA;              		// ��������LOCKA��
	 return 1;
}
/******************************************************************************************************
* ��    �ƣ�Flash_SegA_ReadChar()
* ��    �ܣ���InfoA�ж�ȡһ���ֽ�
* ��ڲ�����Addr:������ݵ�ƫ�Ƶ�ַ
* ���ڲ��������ص�����;ƫ�����ʱ������ 0
 ******************************************************************************************************/
unsigned char Flash_SegA_ReadChar (unsigned int Addr)
{
	unsigned int temp=0;
	unsigned char Data;
	 //----- �η�Χ�޶���Ϊ���ڴ����ȫ��ֻ�����β���-----
	if(Addr>=64)
		return 0;
	unsigned char *Ptr_SegAddr;                    // Segment  pointer
	temp = 0x10c0+Addr;
	Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	Data=*Ptr_SegAddr;					   				//ֱ��ΪInfoA�׵�ַ��δʹ��ȫ�ֱ���SegAddr
	return(Data);
}

