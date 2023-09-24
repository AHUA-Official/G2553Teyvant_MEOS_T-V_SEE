
#include "msp430g2553.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

/**************∫Í∂®“Â***************/
#define DataDir     P1DIR
#define DataPort    P1OUT				
#define Busy	    0x80
#define CtrlDir     P2DIR
#define CLR_RS P2OUT&=~BIT3;    //RS = P3.0	
#define SET_RS P2OUT|=BIT3;	
#define CLR_RW P2OUT&=~BIT4;	//RW = P3.1
#define SET_RW P2OUT|=BIT4;	
#define CLR_EN P2OUT&=~BIT5;	//EN = P3.2	
#define SET_EN P2OUT|=BIT5;	



void DispStr(unsigned char x,unsigned char y,unsigned char *ptr);
void DispNChar(unsigned char x,unsigned char y, unsigned char n,unsigned char *ptr);
void LocateXY(unsigned char x,unsigned char y);
void Disp1Char(unsigned char x,unsigned char y,unsigned char data);
void LcdReset(void);
void LcdWriteCommand(unsigned char cmd,unsigned char chk);
void LcdWriteData( unsigned char data );
void WaitForEnable(void);
void Delay5ms(void);
