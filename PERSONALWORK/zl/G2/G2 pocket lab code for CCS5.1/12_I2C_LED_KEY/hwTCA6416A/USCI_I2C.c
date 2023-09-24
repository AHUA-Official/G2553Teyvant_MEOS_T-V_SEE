/*
 * USCI_I2C.c
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */
#include"MSP430G2553.h"

#define TX_STATE 						0					/*I2C发送状态*/
#define RX_STATE						1					/*I2C接收状态*/
//-----对SMCLK分频产生I2C通信波特率-----
#define I2C_BAUDRATE_DIV 	14				/*I2C波特率控制*/
#define SLAVE_ADDR 				0x20				/*从机TCA6416A的地址*/
static unsigned char  TxByteCnt=0;				//剩余发送数据
static unsigned char  RxByteCnt=0;				//剩余接收数据
static unsigned char *pTxData;                     // 待发送TX 数据的指针
static unsigned char *pRxData;                     // Rx接收存放数据的指针
unsigned char I2C_State =  0;						//收发状态指示变量
/******************************************************************************************************
 * 名       称：USCI_I2C_Init()
 * 功       能：初始化USCI_B0模块为I2C模式
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C设为3线制主机状态，暂不使能Tx和Rx中断
 * 范       例：无
 ******************************************************************************************************/
void USCI_I2C_Init()
{
	 _disable_interrupts();
	  P1SEL |= BIT6 + BIT7;                      						// GPIO 配置为USCI_B0功能
	  P1SEL2|= BIT6 + BIT7;                    						 // GPIO 配置为USCI_B0功能
	  UCB0CTL1 |= UCSWRST;                      					 // 软件复位状态
	  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // 同步通信I2C主机状态
	  UCB0CTL1 = UCSSEL_2 + UCSWRST;            		 // 使用SMCLK，软件复位状态
	  UCB0BR0 =I2C_BAUDRATE_DIV ;                          // 除了分频系数，实际波特率还与SMCLK有关
	  UCB0BR1 = 0;														//这一级别的分频一般不启用
	  UCB0I2CSA = SLAVE_ADDR;                    			// I2C从机地址，可在宏定义中修改
	  UCB0CTL1 &= ~UCSWRST;                     				// 开启I2C
	  _enable_interrupts();
}
/******************************************************************************************************
 * 名       称：USCI_I2C_Tx_Init()
 * 功       能：仅使能I2C的Tx中断
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C通信只能半双工，只使能一个中断，可靠
 * 范       例：无
 ******************************************************************************************************/
void USCI_I2C_Tx_Init()
{
	  _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // 确保总线空闲
	  IE2 &= ~UCB0RXIE;							//关闭Rx中断
	  I2C_State=TX_STATE;
	  IE2 |= UCB0TXIE;                          	    //允许Tx中断
	  _enable_interrupts();						// 开总中断
}
/******************************************************************************************************
 * 名       称：USCI_I2C_Rx_Init()
 * 功       能：仅使能I2C的Rx中断
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C通信只能半双工，只使能一个中断，可靠
 * 范       例：无
 ******************************************************************************************************/
void USCI_I2C_Rx_Init()
{
	 _disable_interrupts();
	  while ((UCB0STAT & UCBUSY)||UCB0CTL1 & UCTXSTP);       // 确保总线空闲
	  IE2 &= ~UCB0TXIE;							//关闭Rx中断
	  I2C_State=RX_STATE;
	  IE2 |= UCB0RXIE;                          	    //允许Tx中断
	  _enable_interrupts();						// 开总中断
}
/******************************************************************************************************
 * 名       称：PreSet_Tx_Frame()
 * 功       能：构造待发射数据的“弹夹”
 * 入口参数：无
 * 出口参数：无
 * 说       明：只有不BUSY且STOP已复位的情况下才允许操作“弹夹”
 * 范       例：无
 ******************************************************************************************************/
unsigned char PreSet_Tx_Frame(unsigned char *p_Tx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
 	pTxData = (unsigned char *)p_Tx;     	// 更新数据指针
    TxByteCnt = num;                  					// 更新剩余发送数据个数
    UCB0CTL1 |= UCTR + UCTXSTT;            // I2C Tx位, 软件start condition
    _bis_SR_register(CPUOFF+GIE);       		// 进LPM0模式，开总中断
    return(1);
}
/******************************************************************************************************
 * 名       称：PreSet_Rx_Frame()
 * 功       能：构造待接收数据的“弹夹”
 * 入口参数：无
 * 出口参数：无
 * 说       明：只有不BUSY且STOP已复位的情况下才允许操作“弹夹”
 * 范       例：无
 ******************************************************************************************************/
unsigned char PreSet_Rx_Frame(unsigned char *p_Rx,unsigned char num)
{
	if ((UCB0STAT & UCBUSY)||(UCB0CTL1 & UCTXSTP))		return(0);
	pRxData = (unsigned char *)p_Rx;     	// 更新数据指针
	 RxByteCnt= num;                  				// 更新剩余接收数据个数
	 UCB0CTL1 &= ~UCTR;
     UCB0CTL1 |= UCTXSTT;                    		// I2C Rx位, 软件start condition
    _bis_SR_register(CPUOFF+GIE);       	    //  进LPM0模式，开总中断
    return(1);
 }
void I2C_TxFrame_ISR(void);
void I2C_RxFrame_ISR(void);
/******************************************************************************************************
 * 名       称：USCIAB0TX_ISR()
 * 功       能：响应I2C的收发中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：I2C的Tx和Rx共用中断入口
 * 范       例：无
 ******************************************************************************************************/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	 _disable_interrupts();							//等同_DINT
  if(I2C_State==TX_STATE)							//判断是收状态还是发状态
    I2C_TxFrame_ISR();                        			// 事件：发送帧
  else
	I2C_RxFrame_ISR();                     			// 事件：接收帧
  	//-------预留给主循环中唤醒CPU用途-------
  	  if(RxByteCnt == 0 || TxByteCnt == 0)	//如果没有待发送或待接收数据
  	    __bic_SR_register_on_exit(CPUOFF);   // Exit LPM0
  	 _enable_interrupts();							//等同_ENIT
}
/******************************************************************************************************
 * 名       称：I2C_TxFrame_ISR()
 * 功       能：I2C的Tx事件处理函数，发送缓存数组中的数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：类似FIFO操作，但指针无需循环
 * 范       例：无
 ******************************************************************************************************/
void I2C_TxFrame_ISR(void)
{
   if (TxByteCnt)                            			// 检查数据是否发完
  {
    UCB0TXBUF = *pTxData;                   // 装填待发送数据
    pTxData++;											//数据指针移位
    TxByteCnt--;                            			// 待发送数据个数递减
  }
  else														//数据发送完毕
  {
    UCB0CTL1 |= UCTXSTP;                   	// 置停止位
    IFG2 &= ~UCB0TXIFG;                     	// 人工清除标志位（由于没有写Buffer，MCU不会自动清除）
  }
}
/******************************************************************************************************
 * 名       称：I2C_RxFrame_ISR()
 * 功       能：I2C的Rx事件处理函数，读取UCB0RXBUF写入指定缓存数组中
 * 入口参数：无
 * 出口参数：无
 * 说       明：类似读FIFO操作，但指针无需循环
 * 范       例：无
 ******************************************************************************************************/
void I2C_RxFrame_ISR(void)
{
	  if (RxByteCnt == 1)                       			// 只剩1字节没接收时（实际已经在RxBuff里了）
	      UCB0CTL1 |= UCTXSTP;                  		// 软件产生停止位
	  RxByteCnt--;                              				// 待接收字节数递减
	  *pRxData = UCB0RXBUF;                  		// 存储已接收的数据
	  *pRxData++;												//数据指针移位
}
