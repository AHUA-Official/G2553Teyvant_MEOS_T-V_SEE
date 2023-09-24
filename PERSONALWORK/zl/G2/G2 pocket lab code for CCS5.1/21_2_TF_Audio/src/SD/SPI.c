/*
 * SPI.c
硬件SPI，阻塞代码
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#include"MSP430G2553.h"
#include"SPI.h"


//*********************************以下是硬件SPI底层驱动代码**********************************
//-----硬件SPI管脚宏定义-----
#define SPI_SIMO        	BIT2		//1.2
#define SPI_SOMI        	BIT1		//1.1
#define SPI_CLK         	BIT4		//1.4
#define SPI_CS           	BIT4		//P2.4
//-----硬件SPI控制端口宏定义-----
#define SPI_SEL2         	P1SEL2
#define SPI_SEL         		P1SEL
#define SPI_DIR         	P1DIR
#define SPI_OUT         	P1OUT
#define SPI_REN         	P1REN
//-----使能端CS端口宏定义-----
#define SPI_CS_SEL2       P2SEL2
#define SPI_CS_SEL       	 P2SEL
#define SPI_CS_OUT       P2OUT
#define SPI_CS_DIR       	 P2DIR


/****************************************************************************
* 名    称：SPI_init()
* 功    能：对硬件SPI进行初始化设置
* 入口参数：无
* 出口参数：无
* 说    明：如需使用后面的读写函数，在程序开始必须先调用该初始化函数
* 使用范例：SPI_init();
****************************************************************************/
void SPI_init(void)
{
    //-----管脚初始化为 SPI 功能-----
    SPI_SEL |= SPI_CLK + SPI_SOMI + SPI_SIMO;
    SPI_SEL2 |= SPI_CLK + SPI_SOMI + SPI_SIMO;
    SPI_DIR |= SPI_CLK + SPI_SIMO;
    //-----SD 卡SPI模式下，需要将SOMI加上拉电阻-----
    SPI_REN |= SPI_SOMI;
    SPI_OUT |= SPI_SOMI;
    //-----使能CS管脚为输出功能-----
    SPI_CS_SEL   &= ~SPI_CS;
    SPI_CS_SEL2 &= ~SPI_CS;
    SPI_CS_OUT |= SPI_CS;
    SPI_CS_DIR  |= SPI_CS;
    //-----复位UCA0-----
    UCA0CTL1 |= UCSWRST;
    //-----3-pin, 8-bit SPI 主机模式- 上升沿----
    UCA0CTL0 = UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC;
    //-----时钟选择SMCLK，MSB first-----
    UCA0CTL1 = UCSWRST + UCSSEL_2;
    //-----f_UCxCLK = 12MHz/50 = 240kHz-----
    UCA0BR0 = 50;
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    //-----开启UCA0-----
    UCA0CTL1 &= ~UCSWRST;
    //-----清除中断标志位-----
    IFG2 &= ~(UCA0RXIFG+UCA0TXIFG );
    __bis_SR_register(GIE);
}
/****************************************************************************
* 名       称：SPI_CS_High()
* 功       能：3线硬件SPI模式，控制使能CS管脚为高电平
* 入口参数：无
* 出口参数：无
* 说       明：此处的CS管脚可以根据硬件的需要，任意指定管脚作CS均可。
* 使用范例：SPI_CS_High();
****************************************************************************/
void SPI_CS_High(void)
{
    SPI_CS_OUT |= SPI_CS;
}
/****************************************************************************
* 名       称：SPI_HighSpeed()
* 功       能：设置SPI为高速
* 入口参数：无
* 出口参数：无
* 说       明：有些SPI设备可工作在高速SPI状态
* 使用范例：无
****************************************************************************/
void SPI_HighSpeed()
{
	UCA0CTL1 |= UCSWRST;
	UCA0BR0 = 2;                                // f_UCxCLK = 12MHz/2 = 6MHz
	UCA0BR1 = 0;
	UCA0MCTL = 0;
	UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* 名       称：SPI_LowSpeed()
* 功       能：设置SPI为低速
* 入口参数：无
* 出口参数：无
* 说       明：有些SPI设备需要工作在低速SPI状态
* 使用范例：无
****************************************************************************/
void SPI_LowSpeed()
{
    UCA0CTL1 |= UCSWRST;
    UCA0BR0 =50;                          		// f_UCxCLK = 12MHz/50 = 240KHz
    UCA0BR1 = 0;
    UCA0MCTL = 0;
    UCA0CTL1 &= ~UCSWRST;
}
/****************************************************************************
* 名    称：SPI_CS_Low()
* 功    能：3线硬件SPI模式，控制使能CS管脚为低电平
* 入口参数：无
* 出口参数：无
* 说    明： 此处的CS管脚可以根据硬件的需要，任意指定管脚作CS均可。
* 使用范例：SPI_CS_SLow();
****************************************************************************/
void SPI_CS_Low(void)
{
    SPI_CS_OUT &= ~SPI_CS;
}

/***************************************************************************
* 名    称：SPI_RxFrame()
* 功    能：3线硬件SPI模式下，接收指定数目的字节
* 入口参数：pBuffer：指向存放接收数据的数组
* 					size：要接收的字节数
* 出口参数：0：当前硬件SPI在忙，
* 					1：当前数据已发送完毕，
* 说    明：使用该函数可以接收指定个数的一帧数据
* 使用范例：SPI_RxFrame(CMD,6);// 接收6个字节，并依次放入CMD中
 ******************************************************************************/
unsigned char SPI_RxFrame(unsigned char  *pBuffer, unsigned int size)
{
    unsigned int gie = __get_SR_register() & GIE;              //保存当前 全部 GIE 状态
//    __disable_interrupt();                                 						//禁止中断
    IFG2 &= ~UCA0RXIFG;                                   				//清除接收中断标志位
    while (size--){
        while (!(IFG2 & UCA0TXIFG)) ;                    			 	//等待发送完毕
        UCA0TXBUF = 0xff;                                  					//发送空字节
        while (!(IFG2 & UCA0RXIFG)) ;                     				//等待接收完毕
        *pBuffer++ = UCA0RXBUF;
    }
//    __bis_SR_register(gie);                               						 //还原全部中断信息
    return 1;
}
/***************************************************************************//**
* 名    称：SPI_TxFrame()
* 功    能：3线硬件SPI模式下，发送指定数目的字节缓存
* 入口参数：pBuffer：指向待发送的数组地址
* 					size：待发送的字节数
* 出口参数：0：当前硬件SPI在忙，
* 					1：当前数据已发送完毕，
* 说    明：使用该函数可以发送指定个数的一帧数据
* 使用范例：SPI_TxFrame(CMD,6);	// 从CMD中取出并发送6个字节
 ******************************************************************************/
unsigned char SPI_TxFrame(unsigned char  *pBuffer, unsigned int  size)
{
	unsigned int  gie = __get_SR_register() & GIE;              //保存当前 全部 GIE 状态
//	_disable_interrupts();                                		 				 //禁止中断
    while (size--){
        while (!(IFG2 & UCA0TXIFG)) ;                   					 //等待发送完毕
        UCA0TXBUF = *pBuffer++;                       					 //写入待写字节
    }
    while (UCA0STAT & UCBUSY) ;                     					 //等待收发完毕

    UCA0RXBUF;                                            	   					 //空读RXBUF，用于下一次写入信息
 //   _bis_SR_register(gie);                                					 //还原全部中断信息
    return 1;
}
