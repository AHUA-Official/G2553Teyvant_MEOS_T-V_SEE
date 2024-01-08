/**************************************************************
    
    其实用74HC595也是一种动态显示，所以必须利用循环才行。
    运用74HC595串入并出的方式显示8位共阴数码管,本实验是用P2.0,P2.1,P2.2

    利用看门狗定时器，实现电子表。即：数码管第1.2显示时，第3.4位显示分
    最后7.8显示秒。
   
    并且利用DS18B20温度传感器显示温度，通过按键P1.0 切换温度
    
    切换温度和时间，调整时间分配：
    通过P1.1切换时间（前提是把开发板上的LED1调帽拔出）。
    通过P1.2调节分加一分钟
    通过P1.3调节分减一分钟
    通过P1.2调节时加一小时
    通过P1.3调节时减一小时

   //////       硬件连接     //////////////

    P2.0接74HC595的12 脚，存储时钟(STCP)
    P2.1接74HC595的14 脚，串行数据输入脚(DS)
    P2.2接74HC595的11 脚，移位时钟(SHCP)
    P2.3接DS18B20的输出（记得接一个上拉电阻4.7K或10K）
    P1.0切换温度
    P1.1切换温度
    P1.2时间分的调节，即：加一分钟
    P1.3时间分的调节，即：减一分钟
    P1.4时间分的调节，即：加一小时
    P1.5时间分的调节，即：减一小时
*************************************************************/
#include <msp430g2452.h>
#define uint unsigned int 
#define uchar unsigned char

//以下是74HC595所需的宏定义
#define HC595_Store_H P2OUT |= BIT0
#define HC595_Store_L P2OUT &= ~ BIT0
#define HC595_A_H     P2OUT |= BIT1
#define HC595_A_L     P2OUT &= ~ BIT1
#define HC595_Shift_H P2OUT |= BIT2
#define HC595_Shift_L P2OUT &= ~ BIT2
#define DS18B20       BIT3
#define DS18B20_H     P2OUT |= BIT3
#define DS18B20_L     P2OUT &= ~BIT3
//以上是74HC595所需的宏定义
unsigned char const LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                                0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71,//共阴数码管段选数组(0-F）
                                0xBf,0x86,0xDb,0xCf,0xE6,0xEd,0xFd,0x87,    
                                0xff,0xef,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,};//共阴数码管(带小数点)段选数组(0-F）
unsigned char const LedPos[] = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}; //数码管位选数组，分别选通每一位数码管
volatile uint shijian=0;                     //时间计数
volatile uchar sec=0,minter=0,hour=0;
volatile uchar temp_flag=1,time_flag=0;     //定义键值全局变量

/**********************************************

                函数声明

**********************************************/
void init_devices(void);
void port_init(void);
void int_WDT(void);
void int_Clock();
volatile uchar key_value=0;           //定义键值全局变量

//以下是DS18B20所需的函数定义

void B20_init(void);
uchar B20_readB(void);
void B20_writeB(uchar wrd);
uint Read_temp(void);
void B20_display(void);
//以上是DS18B205所需的函数定义

void miaobiao(void); 
void Delay_ms(uint ms);           // 延时ms  前提是f=8MHZ

//以下是74HC595所需的函数定义
void HC595shift(void);
void HC595store(void);
void HC595send(uchar x);
void display(uchar pos,uchar dat); //pos则是表示选通哪一位，dat则是表示显示什么数据
//以上是74HC595所需的函数定义
void main( void )         //主函数
{
   
   init_devices();        //初始化设置
   _EINT();               //开总中断
   while(1)
     {  
       while(temp_flag)
       {
         B20_display();     //显示温度
       }
       while(time_flag)
       {
          miaobiao();        //显示电子表
       }
     }
}
void init_devices(void)
{
   port_init();          //IO设置
   int_WDT();            //看门狗设置
   int_Clock();          //时钟设置
}
void port_init(void)           //端口初始化
{
   P2DIR |= BIT0+BIT1+BIT2+BIT3;
    
   P1OUT |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //P2.0-P2.5上拉电阻
   P1REN |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //使能P2.0-P2.5的上拉电阻
   P1IE  |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     // P2.0-P2.5 IO口中断使能
   P1IES |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;     //P2.0-P2.5 IO口边沿触发中断方式设置
   P1IFG &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);  // P2.0-P2.5 IO口中断标志位清除
}
void int_WDT(void)                 //看门狗设置
{   
  WDTCTL = WDT_ADLY_1000;          //  打开看门狗（在辅助系统时钟下定时1s）
  IE1 |=WDTIE;                     // 开看门狗中断
}
void int_Clock(void)               //时钟初始化 （即配置主时钟为8MHZ）
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
}
void Delay_ms(uint ms)             // 延时ms  前提是f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}
/********************************
///////////////////////////////

以下是温度传感器DS18B02函数
延时

/////////////////////////////////

函数名称: B20_init
功    能: 复位DS18B20
参    数: 无
返回值  : 无
********************************/
void B20_init(void)
{
 P2DIR |= DS18B20;		             //配置为输出			  
 DS18B20_L;			             //拉低
 //Delay_us(600);			     //等待600微秒
 __delay_cycles(4800); 
 DS18B20_H;			             //释放总线
 //Delay_us(60);			     //等待60微秒
 __delay_cycles(480); 
 P2DIR &= ~DS18B20;		             //配置为输入 
 while((P2IN &(DS18B20)));	             //等待DS18B20拉低
 while(!(P2IN &(DS18B20)));                  //等待DS18B20释放总线
}
/*******************************************
函数名称: B20_readB
功    能: 读取一个字节的数据
参    数: 无
返回值  : retd--返回的一个字节数据
********************************************/
uchar B20_readB(void)
{
 uchar i,retd=0;
 for(i=0;i<8;i++)	 	//位计数值
 {
  retd>>=1;			//右移，准备接受新的数据位
  P2DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动读数据位
  DS18B20_H;			//释放总线
  //Delay_us(5);		//等待5微秒
  __delay_cycles(40); 
  P2DIR &=~DS18B20;		//配置为输入，开始读取数据位
  if(P2IN&DS18B20)		//该位是否为高
  {
   retd|=0x80;			//是就将此位置高
  }
  //Delay_us(50);		//等待50微秒
  __delay_cycles(400); 
 }
 return retd;			//将读到的一个字节返回
}
/*******************************************
函数名称: B20_writeB
功    能: 写入一个字节的数据
参    数: wrd--要写入的数据
返回值  : 无
********************************************/
void B20_writeB(uchar wrd)
{
 uchar i;
 for(i=0;i<8;i++)	  	//位计数值
 {
  P2DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动写数据位
 // Delay_us(1);		//等待1微秒
  __delay_cycles(8); 
  if(wrd&0x01)			//此位数据是否为高
  {
   DS18B20_H;		        //是高则将单总线拉高
  }
  else
  {
   DS18B20_L;		        //是低则将单总线拉低
  }
  //Delay_us(50);	        //等待50微秒
  __delay_cycles(400); 
  DS18B20_H;		        //释放总线
  wrd>>=1;			//右移，为写入新的数据位做准备
 }
 // Delay_us(50);		//等待50微秒
 __delay_cycles(400); 
}
/*******************************************
函数名称: Read_temp
功    能: 读取温度值
参    数: 无
返回值  : rettemp--返回的温度值
********************************************/
uint Read_temp(void)
{
 uchar templ,temph;
 uint temp;
 B20_init();	        //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc);      //跳过ROM
 B20_writeB(0x44);      //启动温度转换
 B20_init();	        //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc);      //跳过ROM
 B20_writeB(0xbe);      //读寄存器
 templ=B20_readB();     //读温度低字节
 temph=B20_readB();     //读温度高字节
 temp=templ+temph*256;  //将温度整理成16位变量
  return temp;	        //返回16位变量
}
void B20_display(void)
{
   uint t;
   t=Read_temp();          //读取温度值
   t=t*6.25;              //显示的温度保留小数点后一位，
                           //要想保留两位可以乘以0.0625
   display(2,t/1000);       //显示温度的十位
   Delay_ms(5);
   display(3,t%1000/100+16); //显示温度的个位（个位显示小数点即查表时加16）
   Delay_ms(5);
   display(4,t%100/10);        //显示温度的小数位
   Delay_ms(5);
   display(5,t%10);        //显示温度的小数位
   Delay_ms(4);
}
/*******************************************
///////////////////////////////

以上是温度传感器DS18B02函数

////////////////////////////
秒表显示子函数：即共阴数码管的第3位显示时，
第4,5位显示分，第6,7位显示秒。

********************************************/
void miaobiao(void)
{
      hour=shijian/3600;
      minter=shijian%3600/60;
      sec=shijian%60;
        if(shijian>43199)
        {
          shijian=0;
        }
        display(6,sec/10);
        Delay_ms(3);
        display(7,sec%10);
        Delay_ms(3);
        display(3,minter/10);
        Delay_ms(3);
        display(4,minter%10+16);//显示电子表的分的后一位（要显示小数点即查表时加16）
        Delay_ms(3);
        display(0,hour/10);     //显示电子表的时位（要显示小数点即查表时加16）
        Delay_ms(3);
        display(1,hour%10+16); 
        Delay_ms(2);
     }
/*******************************************
///////////////////////////////

以下是74HC595驱动数码管的函数

////////////////////////////
秒表显示子函数：即共阴数码管的第3位显示时，
第4,5位显示分，第6,7位显示秒。

********************************************/

void HC595send(uchar x)
{
   uchar n,temp;
   for(n=0;n<8;n++)	//一个字节要分8次写入
   {
      temp=x&0x80;	//记住一定要先写最高位
      if(temp!=0)	//判断是否不等于0
      {
         HC595_A_H;     //P1.1赋为1
         HC595shift();	//使寄存器移位
      }
      else		//等于0
      {
         HC595_A_L;     //P1.1赋为0
         HC595shift();  //使寄存器移位
      }	 
      x<<=1;		//数据左移一位，那么下一个循环将是获取
                        //第7位的数据，跟着是第6位,第5位,,,第1位
                        //最后把所有数据都移入HC595
    }
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
/*************************************
        显示函数，在某位显示某数据
*************************************/
void display(uchar pos,uchar dat) 
{
   HC595send(LedPos[pos]);   //一定要先把位选数据送入上级HC595 
                             //当把段选数据送入上级HC595时，
                             //位选数据则会移到下级HC595中
   HC595send(LedData[dat]);  //把数据送入下级HC595时，
                             //位选数据则会移到上级HC595中
   HC595store(); 	     //把数据显示出来
   
}
/******************************************************

                  中断服务函数

*******************************************************/
#pragma vector=WDT_VECTOR             //看门狗中断服务程序
__interrupt void watchdog_time (void)
{
 shijian++;
}

//P1中断服务程序

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(10000);
    {
      if((P1IN & BIT0)==0)
      {
         temp_flag=1;
         time_flag=0;
      }
       if((P1IN & BIT1)==0)
      {
         temp_flag=0;
         time_flag=1;
      }
       if((P1IN & BIT2)==0)
      {
         shijian=shijian+60;
         __delay_cycles(20000);
      }
       if((P1IN & BIT3)==0)
      { 
         shijian=shijian-60;
         __delay_cycles(20000);
       }
      if((P1IN & BIT4)==0)
      {
         shijian=shijian+3600;
         __delay_cycles(20000);
      }
      if((P1IN & BIT5)==0)
      {
         shijian=shijian-3600;
         __delay_cycles(20000);
      }
    }
        P1IFG=0;  // P2.0-P2.2 IO口中断标志位清除
}