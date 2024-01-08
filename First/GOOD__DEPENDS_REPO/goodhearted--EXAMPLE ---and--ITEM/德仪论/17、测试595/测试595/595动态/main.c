
#include "msp430g2553.h"
//#include "intrins.h"

#define uchar unsigned char 
#define uint  unsigned int

#define HC595_Shift_H  P2OUT |= BIT2    //移位时钟     ，595的11脚
#define HC595_Shift_L  P2OUT &= ~BIT2
#define HC595_Store_H  P2OUT |= BIT1    //串行转并行时钟，595的12脚
#define HC595_Store_L  P2OUT &= ~BIT1
#define SDI_H          P2OUT |= BIT0
#define SDI_L          P2OUT &= ~BIT0


//共阴码
unsigned char DIG_CODE[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                            0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned char segbit[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

unsigned char a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;

void HC595_Send(uchar dat0,uchar dat1);
void HC595store(void);
void HC595shift(void);
void P1_IODect();//P1口的外部中断事件检测函数
void GPIO_Init();//GPIO初始化函数



void main(void)
{    
      

      WDTCTL = WDTPW +WDTHOLD;  // Stop Watchdog Timer
      GPIO_Init();//初始化GPIO
      _BIS_SR(GIE);//等同_EINT，使能总中断

      P2DIR = BIT0+BIT1+BIT2;
      P2OUT = BIT0+BIT1+BIT2;
      
      while(1)
      {
         
         HC595_Send(~DIG_CODE[a],segbit[0]);
         HC595_Send(~DIG_CODE[b],segbit[1]);
         HC595_Send(~DIG_CODE[c],segbit[2]);
         HC595_Send(~DIG_CODE[d],segbit[3]);
         HC595_Send(~DIG_CODE[e],segbit[4]);
         HC595_Send(~DIG_CODE[f],segbit[5]);
         HC595_Send(~DIG_CODE[g],segbit[6]);
         HC595_Send(~DIG_CODE[h],segbit[7]);
         
       
      }
}

//595子函数
void HC595_Send(uchar dat0,uchar dat1)
{
	uchar i,temp;
	HC595_Store_H;
	HC595_Shift_H;
	for(i=0;i<8;i++)
	{
                temp = dat0 & 0x80;
		if(temp != 0)  SDI_H;	//从高位到di位                
                else           SDI_L;
                HC595shift();
		dat0 <<= 1;               
	}
	for(i=0;i<8;i++)
	{
                temp = dat1 & 0x80;
		if(temp != 0)  SDI_H;	//从高位到di位                  
                else           SDI_L;
                HC595shift();
		dat1 <<= 1;               
	}      
        _NOP();
        _NOP();
        HC595store(); 
}

/***************************************
 字节写入存储寄存器，并且送往输出端口，数据刷新输出
***************************************/
void HC595store(void) 
{
   HC595_Store_L;   //先将存储时钟置低                  
   HC595_Store_H;   //将存储时钟置高产生一个上升沿，就会把一个字节
                    //写入存储寄存器，即将74HC595 的数据刷新输出。
} 
/**************************************
           寄存器移位函数
**************************************/
void HC595shift(void) 
{
   HC595_Shift_L;  //要将时钟线置低                 
   HC595_Shift_H;  //把时钟线拉高，则是上升沿，就会把进行寄存器移位操作
}
//以上为595全部子函数

//zhongduan子函数
void GPIO_Init()
{
	
	//-----配合机械按键，启用内部上拉电阻-----
	P1REN = 0xff;//启用P1.3内部上下拉电阻
	P1OUT = 0xff;//将电阻设置为上拉
	//-----配置P1.3中断参数-----
	//P1DIR &= ~BIT3;// P1.3设为输入(可省略)
	P1IES = 0xff; // P1.3设为下降沿中断
	P1IE  = 0xff; // 允许P1.3中断
        
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//-----启用Port1事件检测函数-----
	P1_IODect();//检测通过，则会调用事件处理函数
   	P1IFG=0;//退出中断前必须手动清除IO口中断标志
}

/******************************************************************************************************
 * 名       称：P1_IODect()
 * 功       能：判断具体引发中断的IO，并调用相应IO的中断事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：该函数兼容所有8个IO的检测，请根据实际输入IO激活“检测代码”。
 * 					 本例中，仅有P1.3被用作输入IO，所以其他7个IO的“检测代码”没有被“激活”。
 * 范       例：无
 ******************************************************************************************************/
void P1_IODect()
{
	unsigned int Push_Key=0,pp=0;
        pp = ~P1DIR;
	//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
	Push_Key=P1IFG&(pp);//??????
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);	//消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P1IN&Push_Key)==0)//如果该次按键确实有效
        {
	 //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
	 switch(Push_Key)
         {
	 case BIT0:	a++;		break;
	 case BIT1: 	b++;		break;
	 case BIT2: 	c++;		break;
	 case BIT3: 	d++;		break;
	 case BIT4: 	e++;		break;
	 case BIT5: 	f++;		break;
	 case BIT6: 	g++;		break;
	 case BIT7: 	h++;		break;
	 default:			break;		//任何情况下均加上default
	 }
     }
}