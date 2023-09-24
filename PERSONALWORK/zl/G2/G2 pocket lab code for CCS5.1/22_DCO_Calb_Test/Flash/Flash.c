/*
 * Flash.c
 * 实际适用于MSP430x2xx系列单片机，包含以下10个常用功能函数：
(1)初始化。Flash_Init(unsigned char Div,unsigned char Seg )：依据SMCLK频率计算设定Flash的时钟的分频系数，靠Seg段号码确定计划操作的段起始地址。
(2)整段擦除。Flash_Erase()：段擦除函数。
(3)读字节。Flash_ReadChar(unsigned int Addr)：读取偏移地址Addr位置1个字节的数据。
(4)读字。Flash_ReadWord(unsigned int Addr)：读取偏移地址Addr位置1个字的数据。
(5)读一串字节到RAM数组。Flash_ReadSeg(unsigned int Addr, unsigned int SegSize,char * Array)：读取起始偏移地址为Addr，长度SegSize个字节数据到RAM的Array数组。
(6)直接写1个字节。Flash_Direct_WriteChar(unsigned int Addr)：直接写偏移地址Addr位置1个字节的数据。
(7)直接写1个字。Flash_Direct_WriteWord(unsigned int Addr)：直接写偏移地址Addr位置1个字的数据。
(8)备份后写1字节。Flash_Bak_WriteChar(unsigned int Addr)：先备份段内其他数据，擦写后，在偏移地址Addr位置写1个字节的数据，再还原段内其他数据。（仅限信息flash段，使用RAM备份）
(9)备份后写1个字。Flash_Bak_WriteWord(unsigned int Addr)：先备份段内其他数据，擦写后，在偏移地址Addr位置写1个字的数据，再还原段内其他数据。（仅限信息flash段，使用RAM备份）。
(10)读SegA专用函数。Flash_SegA_ReadChar(unsigned int Addr)：读取SegA段偏移地址Addr位置1个字节的数据。
说明： 1、块写函数需要在RAM中调用函数指针来使用，本库函数未涉及
           2、其他长字节的数据类型读写需使用结构体，本库函数未涉及
           3、所有函数均针对无符号整形数据，如需使用有符号整形，需修改函数
           5、对InfoA段单独处理，只有读字节函数Flash_SegA_ReadChar()，不提供擦写函数。
           6、其他函数的段操作首地址SegAddr被Flash_Init()函数“限定”，不易发生误写
 *
 *  Created on: 2013-2-18
 *   Author: Administrator
 */
#include  "MSP430G2553.h"
unsigned int SegAddr=0;						//全局变量
unsigned int SegPre=0;						//全局变量 当前信息段
/******************************************************************************************************
* 名   	 称：Flash_Init()
* 功  	 能：对Flash时钟进行初始化设置
* 入口参数：Div：根据SMCLK频率计算的所需分频值，可设定为1-64
* 					Seg:段号，可设为"0"-"31"或”"A"、"B"、"C"、"D"。
* 出口参数：1：配置成功
*                   0：配置失败
* 说   	 明：操作Flash其他函数前，需要调用该初始化函数设置时钟分频和待操作段首地址。
*                   其他函数中均不出现绝对地址，防止误操作。
* 范       例： Flash_Init(3,'B' ) 3分频、对Info B段操作
 ******************************************************************************************************/
unsigned char Flash_Init(unsigned char Div,unsigned char Seg )
{
	//-----设置Flash的时钟和分频，分频为恰好为最低位，直接用Div-1即可-----
	if(Div<1) Div=1;
	if(Div>64) Div=64;
	FCTL2 = FWKEY + FSSEL_2 + Div-1; 					// 默认使用SMCLK，分频系数参数传入
	//-----操作对象为主Flash段的情况，可通过512的倍数设置段起始地址-----
	SegPre = Seg;														//获取当前段
	if (Seg <= 31) 													//判断是否处于主Flash段
		{
		SegAddr=0xFFFF-(Seg+1)*512+1;					//计算段起始地址
		return(1);														//赋值成功后即可退出并返回成功标志”1“
		}
	//-----操作对象为信息Flash段的情况，穷举即可-----
	switch(Seg)														//判断是否处于信息Flash段
	{
	case 'A':	case'a': 	SegAddr=0x10C0; break;
	case 'B':	case'b': 	SegAddr=0x1080; break;
	case 'C':	case'c': 	SegAddr=0x1040; break;
	case 'D':	case'd': 	SegAddr=0x1000; break;
	default: 	SegAddr=0x20FF;	return(0);			//0x20FF地址为空白区，保护Flash
	}
	return(1);
}
/******************************************************************************************************
* 名       称：Flash_Erase()
* 功       能：擦除Flash的一个数据块，擦写段由初始化函数 Flash_Init()的SegAddr变量决定
* 入口参数：无
* 出口参数：无
* 说       明：函数中给出了擦除InfoFlashA段的操作代码（已注释掉了），但不建议初学者使用。
* 范       例：无
 ******************************************************************************************************/
void Flash_Erase()
{
  unsigned char *Ptr_SegAddr;                   			// Segment  pointer
  Ptr_SegAddr = (unsigned char *)SegAddr;  		// Initialize Flash  pointer
  FCTL1 = FWKEY + ERASE;                    					// 段擦除模式
  FCTL3 = FWKEY;                            						// 解锁
  //FCTL3 = FWKEY+LOCKA;                          		    // 对InfoFlashA也解锁
  _disable_interrupts();
  *Ptr_SegAddr = 0;                         						// 擦除待操作段
  while(FCTL3&BUSY);                           					//Busy
  _enable_interrupts();
  FCTL1 = FWKEY;                           							// 取消擦模式
  FCTL3 = FWKEY+LOCK;                      	 				// 上锁
  //FCTL3 = FWKEY+LOCK+LOCKA;                       // 对InfoFlashA也上锁
}
/******************************************************************************************************
* 名    称：Flash_ReadChar()
* 功    能：从Flash中读取一个字节
* 入口参数：Addr:存放数据的偏移地址
* 出口参数：读回的数据；当偏移溢出时返回0
* 说       明：无
* 范       例：无
 ******************************************************************************************************/
unsigned char Flash_ReadChar (unsigned int Addr)
{
  unsigned char Data=0;
  unsigned int *Ptr_SegAddr,temp=0;                    			// Segment  pointer
  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
  if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
  temp =SegAddr+Addr;
  Ptr_SegAddr =(void*)temp;  		 				// Initialize Flash  pointer
  Data=*(Ptr_SegAddr);
  return(Data);
}
/******************************************************************************************************
* 名       称：Flash_ReadWord()
* 功       能：从FlashROM读回一个整型变量，地址应为偶数
* 入口参数：Addr:存放数据的偏移地址，仍按字节计算，需为偶数
* 出口参数：读回的整型变量值  ；当偏移溢出时返回0
* 说       明：无
* 范       例：无
 ******************************************************************************************************/
unsigned int Flash_ReadWord (unsigned int Addr)
{
  unsigned int *Ptr_SegAddr;
  unsigned int temp=0,Data=0;                    // Segment  pointer
  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
  if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
  temp = SegAddr+Addr;
    Ptr_SegAddr = (void *)temp;  					// Initialize Flash pointer
  Data=*(Ptr_SegAddr);
  return(Data);
}
/******************************************************************************************************
* 名    称：Flash_ReadSeg()
* 功    能：将Flash段内一串数据拷贝到RAM的Array数组
* 入口参数：Addr：起始偏移地址
* 					SegSize：数据个数
* 					*Array：RAM中数组的头指针
* 出口参数：返回出错信息 0:偏移溢出 ；1:正常工作
* 说       明：无
* 范       例：无
 ******************************************************************************************************/
char Flash_ReadSeg(unsigned int Addr, unsigned int SegSize,unsigned char * Array)
{
	unsigned int i=0,temp=0;
	unsigned char *Ptr_SegAddr;                    		// Segment  pointer
	  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if((SegPre<=31&&(Addr+SegSize)>512) ||(SegPre>31&&(Addr+SegSize)>64) )
	  return 0;
	for(i=0;i<SegSize;i++)
	{
		temp=SegAddr+Addr+i;								// 防止编译器处理指针偏移出错
		Ptr_SegAddr = (unsigned char *)temp;  	   	// Initialize Flash  pointer
		Array[i]=*Ptr_SegAddr;									//指针移位方法赋值
	}
	  return 1;
}
/******************************************************************************************************
* 名    称：Flash_Direct_WriteChar()
* 功    能：强行向Flash中写入一个字节(Char型变量)，而不管是否为空
* 入口参数：Addr:存放数据的偏移地址
            		Data:待写入的数据
* 出口参数：返回出错信息 0:偏移溢出 ；1:正常工作
* 范    例：Flash_Direct_WriteChar(0,123);将常数123写入0单元
            	 Flash_Direct_WriteChar(1,a);将整型变量a写入1单元
 ******************************************************************************************************/
char Flash_Direct_WriteChar (unsigned int Addr,unsigned char Data)
{
	unsigned int temp=0;
	unsigned char *Ptr_SegAddr;                 	// Segment  pointer
	  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
	temp = SegAddr+Addr;
	Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	FCTL1=FWKEY+WRT;									//正常写状态
	FCTL3=FWKEY;											//解除锁定
//	FCTL3=FWKEY+LOCKA;								//解除锁定（包括A段）
	_disable_interrupts();														//关总中断
	*Ptr_SegAddr=Data;									//指定地址，写1字节
	while(FCTL3&BUSY);									//等待操作完成
	_enable_interrupts();														//开总中断
	FCTL1=FWKEY;											//退出写状态
	FCTL3=FWKEY+LOCK;								//恢复锁定，保护数据
//	FCTL3=FWKEY+LOCK+LOCKA;					//恢复锁定，保护数据（包括A段）
	 return 1;
}
/******************************************************************************************************
* 名    称：Flash_Direct_WriteWord()
* 功    能：强行向Flash中写入一个字型变量，而不管存储位置是否事先擦除
* 入口参数：Addr:存放数据的偏移地址，仍按字节计算，需为偶数
            		Data:待写入的数据
* 出口参数：返回出错信息 0:偏移溢出 ；1:正常工作
* 范    例：Flash_Direct_WriteWord(0,123);将常数123写入0单元
            	 Flash_Direct_WriteWord(2,a);将整型变量a写入2单元
 ******************************************************************************************************/
char Flash_Direct_WriteWord (unsigned int Addr,unsigned int Data)
{
	unsigned int temp=0;
	unsigned int *Ptr_SegAddr;                    	// Segment  pointer
	  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>=64) )
	  return 0;
	temp=SegAddr+Addr;
	Ptr_SegAddr = (unsigned int *)temp;  	   	// Initialize Flash  pointer
	FCTL1=FWKEY+WRT;									//正常写状态
	FCTL3=FWKEY;											//解除锁定
//	FCTL3=FWKEY+LOCKA;								//解除锁定（包括A段）
	_disable_interrupts();														//关总中断
	*Ptr_SegAddr=Data;									//写16位字
	while(FCTL3&BUSY);									//等待操作完成
	_enable_interrupts();														//开总中断
	FCTL1=FWKEY;											//退出写状态
	FCTL3=FWKEY+LOCK;								//恢复锁定，保护数据
//	FCTL3=FWKEY+LOCK+LOCKA;					//恢复锁定，保护数据（包括A段）
	 return 1;
}
/******************************************************************************************************
* 名    称：Flash_Bak_WriteChar()
* 功    能：不破坏段内其他数据，向Flash中写入一个字节(Char型变量)
* 入口参数：Addr:存放数据的地址
            		Data:待写入的数据
* 出口参数：返回出错信息 0:偏移溢出 ；1:正常工作
* 范    例：Flash_Bak_WriteChar(0,123);将常数123写入0单元
            	 Flash_Bak_WriteChar(1,a);将变量a写入1单元
 ******************************************************************************************************/
char Flash_Bak_WriteChar (unsigned char Addr,unsigned char Data)
{
	unsigned int temp=0;
	unsigned char *Ptr_SegAddr;                    	// Segment  pointer
	unsigned char BackupArray[64];					//开辟64字节的临时RAM备份Seg
	unsigned char i = 0;
	  //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if((SegPre<=31&&Addr>=512) || (SegPre>31&&Addr>64) )
	  return 0;
	for(i=0;i<64;i++)
	{
	  temp=SegAddr+i;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	  BackupArray[i]=*Ptr_SegAddr;					//指针移位方法赋值
	}
	Flash_Erase();  												//擦除待操作段
	FCTL1 = FWKEY + WRT;                      			// 正常写入（非块写）
	FCTL3 = FWKEY ;                            					// 解锁
//	FCTL3 = FWKEY ;                            					// 解锁（含A段）
	for (i=0; i<64; i++)
	{
	_disable_interrupts();															//关总中断
	if(i==Addr)
	{
	  temp=SegAddr+Addr;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	 *Ptr_SegAddr =Data;                    					// 写数据
	 while(FCTL3&BUSY);                           			// 等待写操作完成
	 }
	else
	{
	  temp=SegAddr+i;
	  Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	 *Ptr_SegAddr = BackupArray[i];           		// 恢复Flash内的其他数据
	 while(FCTL3&BUSY);                             		// 等待写操作完成
	}
	_enable_interrupts();										// 开总中断
	}
	FCTL1 = FWKEY;                           					// 清除写
	FCTL3 = FWKEY + LOCK;                     			// 上锁
// FCTL3 = FWKEY + LOCK;                     			// 上锁（含A段）
	 return 1;
}
/******************************************************************************************************
* 名       称：Flash_Bak_WriteWord()
* 功       能：不破坏段内其他数据，向Flash中写入一个字(int型变量)
* 入口参数：Addr:存放数据的地址，仍然是以字节为单位的偏移地址，需为偶数
            		Data:待写入的数据
* 出口参数：返回出错信息 0:偏移溢出 ；1:正常工作
* 说       明：MSP430单片机可以对16位数据直接操作，所以为了加快速度
*                 函数中均直接对word进行操作。
* 范       例：Flash_Bak_WriteWord(0,123);将常数123写入0单元
            	 Flash_Bak_WriteWord(1,a);将变量a写入1单元
 ******************************************************************************************************/
char Flash_Bak_WriteWord(unsigned int Addr,unsigned int Data)
{
	unsigned int *Ptr_SegAddr;                    			// Segment  pointer
	Ptr_SegAddr = (unsigned int *)SegAddr;  	   	// Initialize Flash  pointer
	//-----注意：以下操作数全部为word16位数据类型-----
	unsigned int BackupArray[32];							//开辟32字(64字节）的临时RAM备份Seg
	unsigned int i = 0;
	 //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if((SegPre<=31&&Addr>=512) ||(SegPre>31&&Addr>64) )
	return 0;
	for(i=0;i<32;i++)												//	word型占两个字节
	{
	  BackupArray[i]= *(Ptr_SegAddr+i);					//指针移位方法对字赋值
	}
	Flash_Erase();  													//擦除待操作段
	FCTL1 = FWKEY + WRT;                      				// 正常写入（非块写）
	FCTL3 = FWKEY;                            						// 解锁
// FCTL3 = FWKEY+LOCKA;                            		// 解锁	（含A段）
	for (i=0; i<32; i++)												//	word型占两个字节，需跳过奇数地址
	{
	_disable_interrupts();										//关总中断
	if(i==Addr)
	{
	 *(Ptr_SegAddr+Addr) =Data;  							// 写字型数据
	 while(FCTL3&BUSY);                           				// 等待写操作完成
	 }
	else
	{
	 *(Ptr_SegAddr+i)= BackupArray[i];           		// 恢复Flash内的其他数据，按字恢复
	 while(FCTL3&BUSY);                             			// 等待写操作完成
	}
	_enable_interrupts();											// 开总中断
	}
	FCTL1 = FWKEY;                           						// 清除写
	FCTL3 = FWKEY + LOCK;                     				// 上锁
//	FCTL3 = FWKEY + LOCK+LOCKA;              		// 上锁（含LOCKA）
	 return 1;
}
/******************************************************************************************************
* 名    称：Flash_SegA_ReadChar()
* 功    能：从InfoA中读取一个字节
* 入口参数：Addr:存放数据的偏移地址
* 出口参数：读回的数据;偏移溢出时，返回 0
 ******************************************************************************************************/
unsigned char Flash_SegA_ReadChar (unsigned int Addr)
{
	unsigned int temp=0;
	unsigned char Data;
	 //----- 段范围限定。为了内存管理安全，只允许本段操作-----
	if(Addr>=64)
		return 0;
	unsigned char *Ptr_SegAddr;                    // Segment  pointer
	temp = 0x10c0+Addr;
	Ptr_SegAddr = (unsigned char *)temp;  	// Initialize Flash  pointer
	Data=*Ptr_SegAddr;					   				//直接为InfoA首地址，未使用全局变量SegAddr
	return(Data);
}

