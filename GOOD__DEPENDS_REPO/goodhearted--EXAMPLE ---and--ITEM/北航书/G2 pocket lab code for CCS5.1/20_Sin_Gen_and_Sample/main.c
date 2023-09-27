/*
 * main.c
 */
#include "msp430g2553.h"
#include "TA_PWM.h"						//需调用以前编写的PWM库函数

#define	cSMCLK			12000000	// 定义SMCLK为12MHz
#define	SIN_F				200				// 定义输出正弦波频率为200Hz
#define	SIN_NUM		32				// 定义正弦波查表采样点数
unsigned int table_Valu[64] = {0};	//存放ADC10采样数据

//----各种正弦表，注意采样点数与SIN_NUM宏定义一致-----
//const unsigned int sin_table[SIN_NUM]={64,88,108,122,127,122,108,88,64,39,19,5,0,5,19,39};	//16采样点正弦表
//const unsigned int sin_table[SIN_NUM]={0,7,15,23,31,39,47,55,63,71,79,87,95,103,111,119};//16采样点锯齿波表
//const unsigned int sin_table[SIN_NUM]={32,44,54,61,63,61,54,44,32,19,9,2,0,2,9,19};//16采样点幅值减半正弦表
//const unsigned int sin_table[SIN_NUM]={0,10,20,30,40,50,60,70,80,90,100,110,120,110,100,90,80,70,60,50,40,30,20,10};//24采样点三角波表
const unsigned int sin_table[SIN_NUM]={64,88,108,122,127,122,108,88,64,39,19,5,0,5,19,39	//一半高幅值正弦表，另一半低幅值正弦表
		                                                              ,59,71,81,88,91,88,81,71,59,47,37,30,27,30,37,47};	//32采样点不等幅正弦表
//const unsigned int sin_table[SIN_NUM]={127,127,127,127,127,127,127,127,0,0,0,0,0,0,0,0};	//16采样点方波表


void  ADC10_WaveSample(unsigned char inch);
void Timer0_A_Init();
void Change_Duty();
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	//-----DCO频率设定为12MHz
	BCSCTL1 = CALBC1_12MHZ;          // Set range
	DCOCTL = CALDCO_12MHZ;         // Set DCO step + modulation
	DCOCTL = DCOCTL&0xE0;				//关闭混频器，得到纯净频率
    P1DIR |=BIT6;									//P1.6将来作为示波器的同步信号
	//-----初始化TA1的PWM输出-----
	TA1_PWM_Init('S',1,'F',0);		// TA时钟设为ACLK,通道1超前PWM输出，通道2不作TA用;
	TA1_PWM_SetPeriod(128);   // PWM的周期为128个时钟
	//-----ADC10 初始化-----
	ADC10CTL0 =SREF_1 + ADC10SHT_3 + REF2_5V + REFON+REFOUT + ADC10ON;
	ADC10CTL1 = INCH_0+CONSEQ_0;
	//=====定时器初始化=======
	Timer0_A_Init();
	_enable_interrupts();
	while(1)
	{
		ADC10_WaveSample(0);
		_nop();  		//可在这里设置断点，取出“ADC10.c”数组中的数据
	}
}
/******************************************************************************************************
 * 名       称：ADC10_WaveSample(unsigned char inch)
 * 功       能：完成一次波形采样
 * 入口参数：无
 * 出口参数：无
 * 说       明：共采样64个点
 * 范       例：无
 ******************************************************************************************************/
void  ADC10_WaveSample(unsigned char inch)
{
	int j=0,i=0;
     for( j=0;j<64;j++)        								//循环采样64次
     {
      ADC10CTL0 |= ENC + ADC10SC;              // Sampling and conversion start
       for(i=0;i<300;i++);									//延时采样
       table_Valu[j]=ADC10MEM;
      }
}
/******************************************************************************************************
 * 名       称：Timer0_A_Init()
 * 功       能：TA0初始化
 * 入口参数：无
 * 出口参数：无
 * 说       明：TA0用于定时改变PWM占空比，其实就是改变整个SPWM的频率
 * 范       例：无
 ******************************************************************************************************/
void Timer0_A_Init()
{
  TA0CCTL0 = CCIE;                       					  // 允许比较/捕获模块0的中断
  TA0CCR0 = cSMCLK/SIN_F/SIN_NUM;		  // 配置合适的查表定时时间
  TA0CTL = TASSEL_2 + MC_1;                		  // TA0设为增计数模式，时钟=SMCLK
  _EINT();                                 							  // 开全局总中断
}

/******************************************************************************************************
 * 名       称：Timer0_A0()
 * 功       能：TA0中断子函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：定时中断内一般先关后开总中断操作。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	 _disable_interrupts();
	Change_Duty();
	_enable_interrupts();
}
/******************************************************************************************************
 * 名       称：Change_Duty()
 * 功       能：TA0定时事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：用正弦表定时改写TA1输出PWM的占空比
 * 范       例：无
 ******************************************************************************************************/
void Change_Duty()
{
	static  int i=0;
	unsigned int cnt=0;
	if(i>=SIN_NUM)
	{
		i=0;
		P1OUT^=BIT6;					                // 作为示波器的同步信号输出，便于示波器观测SPWM信号
	}
	cnt = (unsigned int)sin_table[i++];
	TA1CCR1 = cnt;										// 这里进行正弦波查表后，更改占空比
}
