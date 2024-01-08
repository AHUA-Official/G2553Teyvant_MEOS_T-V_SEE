/*
 * SPI.c
 * 根据CKPH和CKPL控制位的不同，SPI的时序有4种约定
 * 本文件采用CKPH=0和CKPL=0的模式：
 * （1）CLK上升沿Tx，下降沿Rx
 * （2）时钟空闲状态为低电平
 * （3）CS低电平使能后的第一个CLK边沿开始收发数据。
 *  Created on: 2013-4-3
 *      Author: Administrator
 */

#include"MSP430G2553.h"
#include"SPI.h"

#ifdef	SOFT_SPI				//Begin of SOFT_SPI
static unsigned char SPI_Delay=50;			//=5us
#define DelayMCLK_FREQ			12000000		//用于精确延时函数
//-----对从机使能CS（STE）引脚宏定义-----
#define SPI_CS_HIGH			P2OUT |=BIT4
#define SPI_CS_LOW			P2OUT &=~BIT4
//-----对通信时钟CLK引脚宏定义-----
#define SPI_CLK_HIGH		P1OUT |=BIT4
#define SPI_CLK_LOW		P1OUT &=~BIT4
//-----对从收主发SIMO引脚宏定义-----
#define SPI_SIMO_HIGH	P1OUT |=BIT2
#define SPI_SIMO_LOW		P1OUT &=~BIT2
//-----对从发主收SOMI引脚宏定义-----
#define SPI_SOMI_IN			P1IN &BIT1
/******************************************************************************************************
 * 名       称：SPI_init(void)
 * 功       能：兼容硬件SPI的初始化命名
 * 入口参数：无
 * 出口参数：无
 * 说       明：实际功能是初始化SPI相关IO的方向
 * 范       例：无
 ******************************************************************************************************/
void SPI_init(void)
{
	P1DIR |=BIT2+BIT4;
	P2DIR |= BIT4;
	P1DIR &=~BIT1;
}
/******************************************************************************************************
 * 名       称：SPI_CS_High(void)
 * 功       能：兼容上层函数的调用
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void SPI_CS_High(void)
{
	SPI_CS_HIGH;
}
/******************************************************************************************************
 * 名       称：SPI_CS_Low(void)
 * 功       能：兼容上层函数的调用
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void SPI_CS_Low(void)
{
	SPI_CS_LOW;
}
/******************************************************************************************************
 * 名       称：delay_us()
 * 功       能：us级延时
 * 入口参数：无
 * 出口参数：无
 * 说       明：通过全局变量SPI_Delay传入参数
 * 范       例：无
 ******************************************************************************************************/
static void delay_us(void)
{
	unsigned int i=0;
	for(i=0;i<SPI_Delay;i++)
		__delay_cycles(DelayMCLK_FREQ/10000000);		//us延时
}
/******************************************************************************************************
 * 名       称：Tx_Char()
 * 功       能：主机向从机发送1个字节数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void Tx_Char(unsigned char data)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		SPI_CLK_LOW;			delay_us();
		if((data<<i)&BIT7)			SPI_SIMO_HIGH;
		else		SPI_SIMO_LOW;
		delay_us();
		SPI_CLK_HIGH;		delay_us();
	}
}
/******************************************************************************************************
 * 名       称：Rx_Char()
 * 功       能：主机接收从机1个字节数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
unsigned char Rx_Char()
{
	unsigned char i=0;
	unsigned char Temp=0;
	for(i=0;i<8;i++)
	{
//		SPI_CLK_HIGH;		delay_us();
//		SPI_CLK_LOW ;		delay_us();

		SPI_CLK_LOW ;		delay_us();
		SPI_CLK_HIGH;		delay_us();
		Temp=Temp<<1;		//移位，这句需放在前面
		if(SPI_SOMI_IN )			//先收高位
			Temp |=BIT0;			//置1
//		else Temp &=~BIT0;	//可省略，默认就是0
	}
	return Temp;
}
/******************************************************************************************************
 * 名       称：SPI_TxFrame()
 * 功       能：主机向从机发送1个帧数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
unsigned char SPI_TxFrame(unsigned char  *pBuffer, unsigned int   size)
{
	_disable_interrupts();
	unsigned char i=0;
	for(i=0;i<size;i++)						//然后依次发送各字节数据
	{
		Tx_Char(*pBuffer);
		pBuffer++;
	}
	 _enable_interrupts();
	return 1;
}

/******************************************************************************************************
 * 名       称：SPI_RxFrame()
 * 功       能：主机接收从机1帧数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
unsigned char SPI_RxFrame(unsigned char  *pBuffer, unsigned int size)
{
	unsigned char i=0;
	_disable_interrupts();
	for(i=0;i<size;i++)						//然后依次接收各个字节数据
	{
		 *pBuffer=Rx_Char();
		 pBuffer++;
	}
	 _enable_interrupts();
	return 1;
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
	SPI_Delay=1;
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
 SPI_Delay=50;
}
#endif		//End of SOFT_SPI

#ifdef HARD_SPI			//Begin of HRAD_SPI

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

//-----定义发送/接收缓存-----
unsigned char  *SPI_Tx_Buffer;
unsigned char  *SPI_Rx_Buffer;
//-----定义待发送/接收的字节数-----
unsigned char  SPI_Tx_Size=0;
unsigned char  SPI_Rx_Size=0;
//-----定义发送/接收模式标志-----
unsigned char SPI_Rx_Or_Tx =0;			// 0:仅接收	1：仅发送   2：收发
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
/****************************************************************************
* 名    称：SPI_Interrupt_Sel()
* 功    能：开启发送或接收中断
* 入口参数：onOff = 0 :关闭发送中断，打开接收中断；
* 					onOff = 1 :关闭接收中断，打开发送中断；
* 出口参数：无
* 说    明： 使用此函数来控制选择当前终端模式，便于合理的运用中断
* 使用范例：SPI_Interrupt_Sel(0);		// 关闭发送中断，打开接收中断；
* 					SPI_Interrupt_Sel(1);		// 关闭接收中断，打开发送中断；
****************************************************************************/
void SPI_Interrupt_Sel(unsigned char onOff)
{
	if(onOff == 0)						// 只开接收中断
	{
		  IE2 &=~UCA0TXIE ;
		  IE2 |= UCA0RXIE ;
	}
	else	if(onOff==1)				// 只开启发送中断
	{
		  IE2 &=~UCA0RXIE ;
		  IE2 |= UCA0TXIE ;
	}
	else										//收发全开
	{
		 IE2 |= UCA0RXIE ;
		 IE2 |= UCA0TXIE ;
	}
}
/****************************************************************************
* 名    称：SPI_RxFrame()
* 功    能：3线硬件SPI模式下，接收指定数目的字节
* 入口参数：pBuffer：指向存放接收数据的数组
* 					size：要接收的字节数
* 出口参数：0：当前硬件SPI在忙，
* 					1：当前数据已发送完毕，
* 说    明：使用该函数可以接收指定个数的一帧数据
* 使用范例：SPI_RxFrame(CMD,6);// 接收6个字节，并依次放入CMD中
****************************************************************************/
unsigned char SPI_RxFrame(unsigned char  *pBuffer, unsigned int size)
{
	if(size==0)									return (1);
	if(UCA0STAT & UCBUSY)			return	(0);			// 判断硬件SPI正忙，返回0
	 _disable_interrupts();											// 关闭总中断
    SPI_Rx_Or_Tx = 0;													// 开启接收模式
    SPI_Rx_Buffer = pBuffer;										// 将发送缓存指向待发送的数组地址
    SPI_Rx_Size = size-1;												// 待发送的数据个数
    SPI_Interrupt_Sel(SPI_Rx_Or_Tx);							// SPI中断开启选择
    _enable_interrupts();                                				// 开总中断
    UCA0TXBUF = 0xff;													// 在接收模式下，也要先发送一次空字节，以便提供通信时钟。
    _bis_SR_register(LPM0_bits);									// 进入低功耗模式0
	return (1);
}
/****************************************************************************
* 名    称：SPI_TxFrame()
* 功    能：3线硬件SPI模式下，发送指定数目的字节缓存
* 入口参数：pBuffer：指向待发送的数组地址
* 					size：待发送的字节数
* 出口参数：0：当前硬件SPI在忙，
* 					1：当前数据已发送完毕，
* 说    明：使用该函数可以发送指定个数的一帧数据
* 使用范例：SPI_TxFrame(CMD,6);	// 从CMD中取出并发送6个字节
****************************************************************************/
unsigned char SPI_TxFrame(unsigned char  *pBuffer, unsigned int  size)
{
	if(size==0)									return (1);
	if(UCA0STAT & UCBUSY)			return	(0);			// 判断硬件SPI正忙，返回0
    _disable_interrupts();											// 关闭总中断
    SPI_Rx_Or_Tx = 1;													// 开启发送模式
    SPI_Tx_Buffer = pBuffer;										// 将发送缓存指向待发送的数组地址
    SPI_Tx_Size = size-1;												// 待发送的数据个数
    SPI_Interrupt_Sel(SPI_Rx_Or_Tx);							// SPI中断开启选择
    _enable_interrupts();	                               				// 开总中断
    UCA0TXBUF = *SPI_Tx_Buffer;								// 先发送第一个字节人工触发第一次"发送"中断
	_bis_SR_register(LPM0_bits);									// 进入低功耗模式0
	return (1);
}
//-----提前申明事件处理函数-----
static void SPI_TxISR();
static void SPI_RxISR();
/******************************************************************************************************
 * 名       称：USCI0TX_ISR_HOOK()
 * 功       能：响应Tx中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：包含唤醒主循环CPU的代码
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR_HOOK(void)
{
	//-----发送中断事件引擎函数-----
	SPI_TxISR();
	//-----判断此次操作是否完成，完成则退出低功耗-----
	 if(SPI_Tx_Size==0)
	_bic_SR_register_on_exit(LPM0_bits);
}
/******************************************************************************************************
 * 名       称：USCI0RX_ISR_HOOK()
 * 功       能：响应Rx中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：包含唤醒主循环CPU的代码
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void)
{
	//-----接收中断事件引擎函数-----
	 SPI_RxISR();
	//-----判断此次操作是否完成，完成则退出低功耗-----
	 if(SPI_Rx_Size==0)
	_bic_SR_register_on_exit(LPM0_bits);
}
/******************************************************************************************************
 * 名       称：SPI_RxISR()
 * 功       能：SPI的Rx事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：对接收到的数据，区别对待进行处理
 * 范       例：无
 ******************************************************************************************************/
static void SPI_RxISR()
{

	*SPI_Rx_Buffer = UCA0RXBUF;								//  读取接收缓存，同时，用于清除“UCA0RXIFG”中断标志位
	if(SPI_Rx_Size!=0)
	{
		SPI_Rx_Size-- ;														// 待发送的数据减1
		SPI_Rx_Buffer++;												// 接收指针向下一字节偏移
		UCA0TXBUF = 0xff;												// 纯粹为了提供CLK。UCA0TXIFG标志位同时被清除。
	}
    IFG2 &= ~UCA0TXIFG;                                   			// 清除发送中断标志位

}
/******************************************************************************************************
 * 名       称：SPI_TxISR()
 * 功       能：SPI的Tx事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：对接收到的数据，区别对待进行处理
 * 范       例：无
 ******************************************************************************************************/
static void SPI_TxISR()
{
    UCA0RXBUF;                                            				// Tx和Rx中断标志位都会置位。此处对UCA0RXBUF空操作，用于清除“UCA0RXIFG”中断标志位
	if(SPI_Tx_Size!=0)
	{
		SPI_Tx_Size-- ;														// 待发送的数据减1
		SPI_Tx_Buffer++;											// 发送指针向下一字节偏移
		UCA0TXBUF = *SPI_Tx_Buffer;							// 放入发送缓存，同时，用于清除“UCA0TXIFG”中断标志位
	}
	else
		IFG2 &= ~UCA0TXIFG;                                   		// 最后一次，由于不对UCA0TXBUF进行操作，需要人为清除发送中断标志位
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
#endif	//end of HARD_SPI
