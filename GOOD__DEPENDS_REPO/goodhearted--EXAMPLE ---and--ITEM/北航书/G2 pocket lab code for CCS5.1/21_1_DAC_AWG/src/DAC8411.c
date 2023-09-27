/**********DAC8411.c******************/


#include "MSP430G2553.h"
//--------IO宏定义----------
#define SYNC_HIGH 		P1OUT |=BIT0
#define SYNC_LOW  		P1OUT &=~BIT0
#define SCLK_HIGH 		P2OUT |=BIT2
#define SCLK_LOW 		P2OUT &=~BIT2
#define DIN_HIGH 		P1OUT |=BIT3
#define DIN_LOW 			P1OUT &=~BIT3
/******************************************************************************************************
 * 名       称：DAC8411_Init()
 * 功       能：初始化DAC8411
 * 入口参数：无
 * 出口参数：无
 * 说       明：就是初始化相关IO的状态
 * 范       例：无
 ******************************************************************************************************/
void DAC8411_Init()
{
	//-----设置IO为输出-----
	P1DIR |=BIT0+BIT3;
	P2DIR |=BIT2;
	//-----设置IO初始状态为高-----
	SCLK_HIGH;
	SYNC_HIGH;
}
/******************************************************************************************************
 * 名       称：write2DAC8411(unsigned int Data)
 * 功       能：对DAC8411写16位数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：共需发18位，头两位0，后16位是DAC量化数据
 * 范       例：无
 ******************************************************************************************************/
void write2DAC8411(unsigned int Data)
{
	unsigned int Temp;
	unsigned char i;
	Temp=Data;
	SYNC_LOW;			//使能开始
	//-----发送00，代表是非节能模式（节能就停止工作了）。-----
	SCLK_HIGH;
	DIN_LOW;			//数据0
	SCLK_LOW;

	SCLK_HIGH;
	DIN_LOW;			//数据0
	SCLK_LOW;
	//-----依次发送16位数据-----
	for(i=0;i<16;i++)
	{
		SCLK_HIGH;
		//-----通过位与，判断最高位是1还是0，已决定发什么数据-----
		if(Temp&BITF)	DIN_HIGH;
		else 		DIN_LOW;
		SCLK_LOW;
		Temp=Temp<<1;		//左移一位，永远发最高位
	}
	SYNC_HIGH;	//使能禁止。数据锁存入DAC8411。
}








