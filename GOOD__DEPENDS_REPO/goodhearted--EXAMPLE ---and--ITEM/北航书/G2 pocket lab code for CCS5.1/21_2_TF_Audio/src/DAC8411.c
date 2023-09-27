/**********DAC8411.c******************/


#include "MSP430G2553.h"
//--------IO宏定义----------
#define SYNC_HIGH 		P1OUT |=BIT0
#define SYNC_LOW  		P1OUT &=~BIT0
#define SCLK_HIGH 		P2OUT |=BIT2
#define SCLK_LOW 		P2OUT &=~BIT2
#define DIN_HIGH 		P1OUT |=BIT3
#define DIN_LOW 			P1OUT &=~BIT3

void DAC8411_Init()
{
	P1DIR |=BIT0+BIT3;
	P2DIR |=BIT2;
	SCLK_HIGH;
	SYNC_HIGH;
}
void write2DAC8411(unsigned int Data)
{
	unsigned int Temp;
	unsigned char i;
	Temp=Data;
	SYNC_LOW;
	//-----发送00-----
	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;

	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;
	//-----发送16位数据-----
	for(i=0;i<16;i++)
	{
		SCLK_HIGH;
		if(Temp&BITF)	DIN_HIGH;
		else 		DIN_LOW;
		SCLK_LOW;
		Temp=Temp<<1;
	}
	SYNC_HIGH;
}








