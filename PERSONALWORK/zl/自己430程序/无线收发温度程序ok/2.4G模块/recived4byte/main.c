/*===========================================================
LaunchPad无线接收程序,315M
实现了四字节的传送，修正了无法自动刷新的问题。
LaunchPad上面的P1.6的LED（绿灯）指示刷新的速度。

       0：   01   1ms低，1ms高；====>一共2ms
       1：   0111 1ms低，3ms高；====>一共4ms
  引导码：10ms低，10ms高。====>一共20ms

程序运行后，如果绿色的LED没有闪烁，则没有刷新，数据无法同步。
LED闪烁期间。数据一直在同步更新。
并且LED必须闪烁的频率稳定。

注释：
改进的地方：
1.定义数组，不能用add[]={0};===>add[i];i就是要用到几个数据
2.在ISP里面（port1），i的值决定接收数据的多少。
3.如果要修改接收数据的多少，则一共要修改3处：
  1>.定义数组处，
  2>.ISP里面的最终i值判断
  3>.在irpro（）;函数里面，修改相应的接收数据。
整个程序的重点：ISP和BCSCTL的配置。

    2012-7-16
By---蔡能
===========================================================*/


#include"msp430g2553.h"
#include "msp430.h"
#include"12864.h"
#define uchar unsigned char
#define uint unsigned int

uchar flag,pro_ok,irok,i;

uint irtime;
uchar irdata[33];//存储时间间隔
uchar irdat[4];//存储接收的数据
uchar add[8];//用于显示
//========//
  uint v;
  uint vv;
  uint ad;
uchar tab[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

void irpro();
void display();
//===========P1口的初始化==========//
void initport()
{
  P1DIR=0x41;
  P1OUT=0x18;
  P1REN=0x18;
  P1IE|=0x18;//P1.3和P1.4为中断输入口
  P1IES|=BIT3+BIT4;//触发方式选择
  P1IFG=0x00;
}
//===========OVER==================//



//=========定时器的初始化===========//
void init()
{
  TACTL=TASSEL_2+MC_1;//SMCLK  upmode
  CCTL0=CCIE;
  CCR0=796;//100us
}
//===============OVER=============//


//=============main==============================//
void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
    //======================时钟配置=====================//
    //====配置的是主时钟，和SMCLK时钟===================//
  BCSCTL2 = SELM_0 + DIVM_3 + DIVS_0;//分频？SMCLK==>8MHZ

    if (CALBC1_8MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        delay(100);

        // Follow recommended flow. First, clear all DCOx and MODx bits. Then
        // apply new RSELx values. Finally, apply new DCOx and MODx bit values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
    }
    

    //=========================OVER=====================//
    initport();
    lcdinit();
    init();
    W_str(0,1,"无线接收程序");
    W_str(0,2,"接收电压：");
    W_str(0,3,"地址");
    W_str(4,3,"数据");
    _EINT();
    while(1)
    {
      if(irok)
      {
        irpro();
        irok=0;       
      }
      if(pro_ok)
      {
        display();
        pro_ok=0;
      }
    }
}
//==========main OVER=================//



//=========100us定时中断服务函数=======//
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer(void)
{
  irtime++;
}


//===============接收端中断服务=================//

#pragma vector=PORT1_VECTOR
__interrupt void Port (void)
{
  if(P1IFG&BIT3)//只用到了P1.3口
  {
    if(flag)
    {
      if(irtime>150&&irtime<250)//数据之差不能太小，否则无法进入。
      {
        i=0;
      }
      irdata[i]=irtime;
      irtime=0;
      i++;
      if(i==33)
      { 
//        W_str(0,2,"ISP");//检验用，进得来
        irok=1;
        i=0;
      }
    }
    else
    {
      irtime=0;
      flag=1;
    }
    P1IFG=0x00;
  }
  P1IFG&=~BIT3;
}



/*===========================
对接收的数据进行处理
============================*/
void irpro()
{
  uchar j,k,i;
  uchar code,value;

  if(irdata[0]>150&&irdata[0]<250)//在这有点问题
  {
    k=1;
    for(i=0;i<4;i++)
    {
      for(j=0;j<8;j++)
      {

        if(j<8)
        value=value>>1;//貌似这个语句的位子相当重要。
        
        code=irdata[k];
        if(code>30)
          value|=0x80;      
        k++;
      }
      irdat[i]=value;
      value=0; 
   }
         
    add[0]=irdat[0]/16;
    add[1]=irdat[0]%16;
        
    add[2]=irdat[1]/16;
    add[3]=irdat[1]%16;

    
    add[4]=irdat[2]/16;
    add[5]=irdat[2]%16;
        
    add[6]=irdat[3]/16;
    add[7]=irdat[3]%16;
        
    pro_ok=1;    
//    W_str(3,2,"irpro");//测试表明进来了。
  }
  P1OUT^=0x40;//指示接收作用
}
void display()
{
  uchar a1,a2,a3,a4;
  
   v=add[5]*256+add[6]*16+add[7];//证明v里面有数

   ad=v*3.56*1000/1024;//这个地方很坑爹！！！！！

   vv=ad;
    
  a1=vv/1000+0x30;
  a2=vv%1000/100+0x30;
  a3=vv%100/10+0x30;
  a4=vv%10+0x30;

  
  write_com(0x95);
  write_data(a1);
  write_data('.');
  write_data(a2);
  write_data(a3);
  write_data(a4);
  write_data('V'); 
  
  write_com(0x98);
  write_data(tab[add[0]]);
  write_data(tab[add[1]]);
  write_data('-');
  write_data(tab[add[2]]);
  write_data(tab[add[3]]);
  
  write_com(0x9c);
  write_data(tab[add[4]]);
  write_data(tab[add[5]]);
  write_data('-');
  write_data(tab[add[6]]);
  write_data(tab[add[7]]);
  
//  W_str(7,2,"OK");//指示display是否运行；

}
