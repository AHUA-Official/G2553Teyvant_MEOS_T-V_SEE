/*
 * SD_SPI.c
 *说明：该函数库为通用SD卡读写顶层库函数。
 * 			 用户只需要更改“SD_HardWare.c”中的少量底层代码，即可移植到任何处理器上。
 *  Created on: 2013-3-4
 *      Author: Administrator
 */
#include"SD_HardWare.h"
#include "SD_SPI.h"
#include"SPI.h"


//-----SD 卡相关的命令宏定义-----
#define SD_WRITE_DELAY				100				// 先发74个以上的时钟
#define SD_EMPTY_CLK				0xFF
#define SD_EMPTY_DATA				0xFF
#define SD_SPI_CRC						0xFF				//SPI模式下CRC无效，随便给
#define SD_CMD0_CRC					0x95				//复位命令CMD0的CRC校验码
#define SD_CMD0 						0+0x40		//复位命令
#define SD_CMD1 						1+0x40		//SPI模式命令
#define SD_CMD17 						17+0x40		//读扇区命令
#define SD_CMD24						24+0x40		//写扇区命令
#define SD_CMD41						41+0x40		//初始化扇区命令
#define SD_CMD55						55+0x40		//初始化扇区命令
#define SD_CMD59						59+0x40		//初始化扇区命令

#define SD_ACMD41						55+0x40		//初始化扇区命令
//-----SD卡相关应答-----
#define SD_COMMAND_ACK		0x00   			//命令应答
#define SD_RESET_ACK					0x01				//复位应答
#define SD_DATA_ACK					0xFE				//数据应答
#define SD_WRITE_ACK					0x05				//写扇区应答
//----系统超时控制----
#define SD_TIMEOUT					100
#define TIMEOUT							200
/************************************************************************************************************
* 名    称：Write_Command_SD()
* 功    能：SD卡在SPI模式下，向SD卡中写入6个字节的命令
* 入口参数：CMD：指向存放六个字节命令的数组
* 出口参数：SD 卡应答值
* 说    明：  向SD卡中一次写入六个字节的命令
* 使用范例：Write_Command_SD(CMD);
************************************************************************************************************/
unsigned char Write_Command_SD(unsigned char *CMD)
{
   unsigned char tmp=0;
   unsigned char i = 0;
   SD_CS_High();
   //-----先发8个空clock-----
   SD_Write_Byte(0xFF);
   SD_CS_Low();
   //-----写入6个字节的命令字帧-----
   SD_Write_Frame(CMD,6);
   //-----空第一次，即忽略第一个返回值-----
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
* 名    称：SD_Write_Sector
* 功    能：SD卡在SPI模式下，向SD卡中，在指定的位置写入指定个数的数据。
* 入口参数：Addr：物理扇区的地址
* 					*Ptr：指向待写数据的数据缓存
* 					FirstNum：写入该扇区的首个字节偏移地址
* 					Num：此次操作要写入的数据个数				(FirstNum+Num<512)
* 出口参数：0：写入失败
* 					1：写入成功
* 使用范例：SD_Write_Sector(83241,buffer,0,100); 	 //向扇区83241中写入100个字节
************************************************************************************************************/
unsigned char SD_Write_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num)
{
	unsigned char temp=0;
	unsigned int EndNum = 0;
	unsigned int i=0;
	unsigned char CMD[6]={0};
	//-----发送命令-----
	CMD[0]=SD_CMD24;
	//-----将32位地址拆分------
	Addr = Addr <<9;													//sector = sector*512   将物理地址转换为逻辑地址；(注意一定需要此处的转换)
	CMD[1]=((Addr&0xFF000000)>>24);
	CMD[2]=((Addr&0x00FF0000)>>16);
	CMD[3]=((Addr&0x0000FF00)>>8);
	CMD[4]=Addr&0x000000FF;
	//-----CRC校验，SPI模式下无效，可随便给-----
	CMD[5]=SD_SPI_CRC;
	//----等待SD卡应答----
	i=TIMEOUT;															//超时门限值
	do{
		temp = Write_Command_SD(CMD);
		 i--;
	}while((temp != SD_COMMAND_ACK)&&i);		//超时判断
	if (i==0) 	return(0);													//超时，返回失败代码
	//----空发若干次CLK----
	for(i=0;i<SD_WRITE_DELAY;i++)
	{
		SD_Write_Byte(SD_EMPTY_CLK);
	}
	//SD_Write_Frame(pTemp,SD_WRITE_DELAY);
	//-----发送数据头字节-----
	SD_Write_Byte(SD_DATA_ACK);
//	//-----发送512字节数据，含空数据----
	EndNum=FirstNum+Num;

	for(i=0;i<FirstNum;i++)
	{
		//-----写空数据-----
		SD_Write_Byte(SD_EMPTY_DATA	);
	}
	SD_Write_Frame(Ptr,Num);				//写有效数据
	for(i=0;i<512-EndNum;i++)
	{
		//-----写空数据-----
		SD_Write_Byte(SD_EMPTY_DATA	);
	}
	//-----发送两个字节的校验位-----
	SD_Write_Byte(0xff);
	SD_Write_Byte(0xff);

	//-----判断SD的应答是否为xxx00101b-------
	temp=SD_Read_Byte();
	temp =temp &0x01F;
	if(temp != SD_WRITE_ACK)
	{
		SD_CS_High();
		return(0);
	}
	//-------一直发CLK，直到不BUSY------
	i=TIMEOUT;														//超时门限值
	do{
			 i--;
	}while(SD_Read_Byte() != 0xff &&i);					//超时判断
	if (i==0)
	{
		SD_CS_High();
		return(0);															//超时，返回失败代码
	}
	//---成功完成写数据，将片选CS拉高------
	SD_CS_High();
	return 1;
}
/************************************************************************************************************
* 名    称：SD_Read_Sector
* 功    能：SD卡在SPI模式下，从SD卡中，在指定的位置处，读出指定个数的数据到缓存。
* 入口参数：Addr：物理扇区的地址
* 					*Ptr：指向存放读取数据的数据缓存
* 					FirstNum：从该扇区读取首个字节的偏移地址
* 					Num：此次操作要读取的数据个数				(FirstNum+Num<512)
* 出口参数：0：读取失败
* 					1：读取成功
* 使用范例：SD_Read_Sector(83241,buffer,0,100); 	 //从扇区83241中读出100个字节，放入buffer中
************************************************************************************************************/
unsigned char SD_Read_Sector(unsigned long Addr ,unsigned char *Ptr,unsigned int FirstNum,unsigned int Num)
{
	unsigned char temp=0;
	unsigned int i=0;
	unsigned int EndNum=0;
	unsigned char CMD[6]={0};

	//-----命令17-----
	CMD[0]=SD_CMD17;
	//-----将32位地址拆分------
	Addr = Addr <<9;												//sector = sector*512   将物理地址转换为逻辑地址；
	CMD[1]=((Addr&0xFF000000)>>24);
	CMD[2]=((Addr&0x00FF0000)>>16);
	CMD[3]=((Addr&0x0000FF00)>>8);
	CMD[4]=Addr&0x000000FF;
	//-----CRC校验，SPI模式下无效，可随便给-----
	CMD[5]=SD_SPI_CRC;

	//----等待SD卡应答----
	i=TIMEOUT;														//超时门限值
	do{
	      temp = Write_Command_SD(CMD);
		 i--;
	}while((temp!=SD_COMMAND_ACK)&&i);		//超时判断
	if (i==0)
	{
		SD_CS_High();
		return(0);															//超时，返回失败代码
	}
	//----等待SD卡应答--------------
	i=TIMEOUT;														//超时门限值
	do{
		 temp = SD_Read_Byte();
		 i--;
	}while(( temp !=SD_DATA_ACK)&&i);				//超时判断
	if (i==0)
	{
		SD_CS_High();
		return(0);															//超时，返回失败代码
	}
	//-----读取指定字节-----
	EndNum=FirstNum+Num;
	for(i=0;i<FirstNum;i++)
	{
		//-----空读（放弃存储）其他字节-----
			SD_Read_Byte();
	}
	SD_Read_Frame(Ptr,Num);			//读取有效字节
	for(i=0;i<512-EndNum;i++)
	{
		//-----空读（放弃存储）其他字节-----
			SD_Read_Byte();
	}
	//-----空读两次 CRC--Byte-----
	 SD_Read_Byte();
	 SD_Read_Byte();
	SD_CS_High();
	return (1);
}

/************************************************************************************************************
* 名    称：SD_Init
* 功    能：初始化SD卡为SPI模式
* 入口参数：无
* 出口参数：0：初始化失败
* 					1：初始化成功
* 说    明：  初始化SD卡为SPI模式
* 使用范例：SD_Init();
************************************************************************************************************/
unsigned char SD_Init()
{
	unsigned char temp =0 ;
	//-----初始化之前，先将SPI设为低速（SPI_clk=300K左右）-----
	SD_Low_Speed();
	//-----复位SPI-----
	temp = SD_Reset();
	if(temp==0)
		return(0);
	//-----设定SD为SPI都取名模式-----
	temp = SD_Set_SPI();
	if(temp==0)
		return(0);
	//--初始化完成以后，将SPI速度提到高速（10Mhz左右）上来，便于快速读写扇区-----
	SD_High_Speed();
	return(1);
}
/************************************************************************************************************
* 名    称：SD_Reset()
* 功    能：复位SD卡
* 入口参数：无
* 出口参数：0：复位SD失败
* 					1：复位SD成功
* 说    明：  上电后SD卡位SD模式，需先复位后才能设置为SPI模式
* 使用范例：无
************************************************************************************************************/
unsigned char SD_Reset()
{
	unsigned char i=0,temp = 0;
	unsigned char CMD[6] = {SD_CMD0,0x00,0x00,0x00,0x00,SD_CMD0_CRC};
	SD_CS_High();																	//CS片选禁能
	for(i=0;i<0x1f;i++)															//空发80个CLK(10字节）
	{
		SD_Write_Byte(SD_EMPTY_CLK);
	}

	i=TIMEOUT;																	//超时门限值
	do{
		temp = Write_Command_SD(CMD);							//CMD0
		i--;
	}while((temp != SD_RESET_ACK) && i);						//超时判断
	_nop();
	if(i==0)
		return(0);																	//超时，返回失败代码
	else
		return (1);
}
/************************************************************************************************************
* 名    称：SD_Set_SPI()
* 功    能：将SD卡设为SPI模式
* 入口参数：无
* 出口参数：0：复位SD失败
* 					1：复位SD成功
* 说    明：  需先复位操作后才能进行设置SPI模式操作
* 使用范例：无
************************************************************************************************************/
unsigned char SD_Set_SPI()
{
	unsigned char i=0,temp=0;
	unsigned char CMD[6]={0};
	//-----拆分命令为4个字节-----
	CMD[0]= SD_CMD0;
	CMD[1]= ((0x00ffc000 & 0xff000000) >>24);
	CMD[2]= ((0x00ffc000 & 0x00ff0000) >>16);
	CMD[3]= ((0x00ffc000 & 0x0000ff00) >>8);
	CMD[4]= 0x00ffc000 & 0x000000ff;
	CMD[5]= 0xFF;

	i=TIMEOUT;																			//超时门限值
	do{
	       CMD[0] = SD_CMD55;                  									//CMD55命令
	       temp = Write_Command_SD(CMD);   							//先发送 CMD55
	       _nop();
	       //-----判断是否为SD卡-----
	       if(temp == 0x01)                 											//如果有反应
	       {
//		//-----如无需判断存储卡类型，直接执行线间代码-----
				 CMD[0] = SD_CMD1;                 								//CMD41命令
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD); 						//发送CMD41进行激活

				 CMD[0] = SD_CMD59;                 								//CMD41命令
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD); 						//发送CMD41进行激活

				 _nop();
				 if(temp == 0x00)
				 {
						SD_CS_High();														//CS片选禁能
				 }
//		//---------------------------------------------------
	       }
	       //-----否则，为MMC卡-----
	       else                             														//如果发送CMD55无反应，改发送CMD1
	       {
				 CMD[0] = SD_CMD1;                 								//CMD1命令
				 CMD[5] = 0xFF;
				 temp = Write_Command_SD(CMD);    					//发送CMD1进行激活
				 if(temp == 0x00)
				 {
						SD_CS_High();														//CS片选禁能
				 }
	       }
		i--;
	}while((temp !=SD_COMMAND_ACK)&&i);							//超时判断
	if(i==0)
		return(0);																			//超时，返回失败代码
	else
		return (1);
}

