/*
 * main.c
 */
#include "msp430g2553.h"
#include"SD_HardWare.h"
#include "SD_SPI.h"
#include"SPI.h"
#include "DAC8411.h"
#include "softFIFO.h"
//#define  WAV_SECTOR    532615			// TF卡中歌曲存放物理扇区首地址   532615（背包）
//#define  WAV_SECTOR    2642695		// TF卡中歌曲存放物理扇区首地址   2642695（老男孩）另一张TF
#define  WAV_SECTOR    619	// TF卡中歌曲存放物理扇区首地址   672（老男孩）128M的TF
#define SYSCLK 12000000						//系统时钟
#define SAMPLE_FREQ 11025				//音频文件的采样率
#define VOL_REDUCE   1  					//取2的N次方，减少运算量
void Time0_A_Init();
 char SD_Read_InFIFO(unsigned long sector);
 void TA0_OnTime();
 void main()
 {
    WDTCTL = WDTPW + WDTHOLD;
 	BCSCTL1 = CALBC1_12MHZ;                   // Set range
 	DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation
 	__delay_cycles(100000);
 	//-----初始化SPI-----
 	 SPI_init();
  	//-----初始化SD-----
 	 SD_Init();
 	DAC8411_Init();
 	// -----初始化采样率定时器-----
 	Time0_A_Init();
 	while(1)
 	{
 		 SD_Read_InFIFO(WAV_SECTOR);			// 读取指定歌曲
 	}
 }
 /******************************************************************************************************
  * 名       称：SD_Read_InFIFO(unsigned long sector)
  * 功       能：不停的读取SD卡数据
  * 入口参数：头扇区地址
  * 出口参数：无
  * 说       明：与SD卡库函数不同的是，本函数读取的数据是往FIFO里写的
  * 范       例：无
  ******************************************************************************************************/
 char SD_Read_InFIFO(unsigned long sector)
 {
 	unsigned char CMD[6]={0};
 	static unsigned  long sec=1750;			// 预设"歌曲"读取偏移，跳过伴奏部分
    unsigned char retry = 0;
    unsigned char temp = 0;
    unsigned int i=0;
    sector = (sector+sec )<<9;					     //转成逻辑地址
    //=====以下部分与SD卡库函数一致=====
    CMD[0]=0x51;	   									//SD_CMD17
    //-----转换32位扇区地址-----
    CMD[1]= ((sector & 0xff000000) >>24);
    CMD[2]= ((sector & 0x00ff0000) >>16);
    CMD[3]= ((sector & 0x0000ff00) >>8);
    CMD[4]= sector & 0x000000ff;
    CMD[5]=0xFF;										    //SD_SPI_CRC
 	sec ++;				//扇区递增
    //-----将命令写入SD卡最多100遍直到应答-----
       do
    {
       temp = Write_Command_SD(CMD);
       retry++;
       if(retry == 100)
       {
         return(0);
       }
    } while(temp!=0);
      while (SD_Read_Byte() != 0xfe) ;	//应答正确
    //=====以上部分和SD卡库函数一致，以下部分改为写数据到FIFO=====
    for(i=0;i<512;i++)
    {
 	   while(FIFO_Count>= FIFOBUFF_SIZE)
 		   _NOP();															// FIFO 溢出，则等待FIFO中的数被取走
 	   Write_FIFO(SD_Read_Byte());							//将数据写入FIFO
    }
    //-----空收循环冗余校验位-----
    SD_Read_Byte();													//CRC - Byte
    SD_Read_Byte();													//CRC - Byte

 	SD_CS_High();														//CS片选禁能
    return(1);
 }
//==============以上为主函数和后台程序，以下为前台程序部分================
/******************************************************************************************************
 * 名       称：Time0_A_Init()
 * 功       能：初始化TA0
 * 入口参数：无
 * 出口参数：无
 * 说       明：就是初始化相关IO的状态
 * 范       例：无
 ******************************************************************************************************/
void Time0_A_Init()
{
  TA0CCTL0 = CCIE;                         	// 允许比较/捕获模块0的中断
  TA0CCR0 = SYSCLK/SAMPLE_FREQ;		//设定TA定时周期
  TA0CTL = TASSEL_2 + MC_1;           //TA0设为增计数模式，时钟=SMCLK
  _enable_interrupts();                       //开全局总中断，
}
/******************************************************************************************************
 * 名       称：Timer0_A0
 * 功       能：TACCR0中断服务函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：此中断源独占中断向量入口，无需判断
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	TA0CCTL0 &= ~CCIE;                       // 关闭比较/捕获模块0的中断
	TA0_OnTime();
	TA0CCTL0 = CCIE;                         // 允许比较/捕获模块0的中断
}
/******************************************************************************************************
 * 名       称： TA0_OnTime()
 * 功       能：TA0的定时事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void TA0_OnTime()
{
	unsigned int temp =0,tempL=0,tempH=0;
 	if(FIFO_Count != 0)
 	{
 		tempL =  Read_FIFO();
 		tempH =  Read_FIFO();
 		temp = ((tempH<<8) &0xff00)|(tempL &0x00ff);
		temp = temp+0x8000;	      //有符号数变无符号数
 		write2DAC8411(temp/VOL_REDUCE);
 	}
}


