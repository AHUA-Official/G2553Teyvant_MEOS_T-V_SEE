/*
 * main.c
 * 往指定扇区写128字节数据，再读回到DATA[]数组
 * 程序只执行一遍，在CCS中查看变量值，核对是否正确读写SD卡
 */
#include"MSP430G2553.h"
#include"SD_SPI.h"
#include"USCI_SPI.h"
#define SD_SECTOR_ADDR  52334		//SD卡待操作的扇区地址
#define SD_SECTOR_FIRST  30
#define SD_SECTOR_NUM  128
unsigned char DATA[128]={0};
void My_Write_Data();
void My_Read_Data();

 void main()
 {
	 unsigned char temp = 0;
	 WDTCTL = WDTPW + WDTHOLD;
	 //-----DCO设为12MHz----
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL = CALDCO_12MHZ;

	//-----操作完成指示灯-----
	P1DIR |= BIT0;
	P1OUT &=~BIT0;
	//-----初始化硬件SPI-----
	 USCI_A0_init();
	 // -----直到初始化成功（一般一次就可初始化成功）-----
	do{
		temp = SD_Init();
	} while(temp == 0);
	//-----写入指定的扇区128个字节-----
	My_Write_Data();
	//-----指定的扇区读取128个字节-----
	My_Read_Data();
	 P1OUT |= BIT0;
	 _bis_SR_register(LPM3_bits);
 }
 /******************************************************************************************************
  * 名       称：My_Write_Data()
  * 功       能：往SD卡中写128字节数
  * 入口参数：无
  * 出口参数：无
  * 说       明：此函数用于演示写SD卡
  * 范       例：无
  ******************************************************************************************************/
 void My_Write_Data()
 {
	 unsigned char i=0,temp=0;
	 unsigned char Temp[128]={0};
	 for (i=0;i<=127;i++)
	 {
		 Temp[i]=9;		// 给临时变量赋值
	 }
	do{ 						// 一直写入SD扇区，直到此次写入操作成功（一般一次就可成功）
		temp = SD_Write_Sector(SD_SECTOR_ADDR,Temp,SD_SECTOR_FIRST,SD_SECTOR_NUM);
	}while(temp ==0 );
}
 /******************************************************************************************************
  * 名       称：My_Write_Data()
  * 功       能：从SD卡指定位置中读128字节数
  * 入口参数：无
  * 出口参数：无
  * 说       明：此函数用于演示读SD卡
  * 范       例：无
  ******************************************************************************************************/
void My_Read_Data()
 {
	 unsigned char i=0,temp=0;
	 for (i=0;i<127;i++)
	 {
		 DATA[i]=0;			//	先清除缓存内容
	 }
	 do{							// 一直读取SD扇区，直到此次读取操作成功（一般一次就可成功）
		 temp = SD_Read_Sector(SD_SECTOR_ADDR ,DATA,SD_SECTOR_FIRST,SD_SECTOR_NUM);
	 }while(temp ==0);
 }
