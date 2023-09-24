/**************************************************

      本实验利用温度传感器DS18B20，再通过HT1621驱动四位半液晶LCD
      来显示当前的温度（精确到0.6C）。

注意：
     1.P1.0，P1.4最好先别急着用,可以利用P1.0（ACLK）外部手表时钟检测是否进入LPM4 
       P1.4 (SMCLK)内部DCO,为了测试是否进入低功耗模式LPM3,要是没有输出方波
       说明已经进入LPM3。
     2.最后进入低功耗的时候，一定要配置好IO口，除键盘所用的IO口设置输入外，其他
       一定要设置输出，并且最好输出高电平。

///////////////////////硬件连接/////////////////////
            
      液晶驱动HT1621D的7引脚   即：HT1621_DAT-------------------P1.5
      液晶驱动HT1621D的4引脚   即：HT1621_CS--------------------P1.1
      液晶驱动HT1621D的6引脚   即：HT1621_WR--------------------P1.2
      温度传感器DS18B20的2引脚 即：DS18B20----------------------P1.3
      睡眠按键------------------------------------------------- P2.0 
      唤醒温度按键--------------------------------------------- P2.1
      唤醒分秒按键--------------------------------------------- P2.2
      时间分的调节按键----------------------------------------- P2.3
      唤醒时按键----------------------------------------------- P2.4
      时间时的调节按键----------------------------------------- P2.5

*************************************************/
#include <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned

//以下是HT1621 LCD显示宏定义
#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN   0X02  //0b1000 0000 0010 打开系统振荡器
#define LCDOFF  0X04  //0b1000 0000 0100 关LCD偏压
#define LCDON   0X06  //0b1000 0000 0110 打开LCD偏压
#define TIMERDIS 0X08  //0b1000 0000 0110 禁止时基输出
#define XTAL    0x28  //0b1000 0010 1000 外部接时钟
#define RC256   0X30  //0b1000 0011 0000 内部时钟
#define TONEON  0X12  //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10  //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A  //0b1000 0000 1010 禁止看门狗
#define HT1621_DAT_H P1OUT|=BIT5
#define HT1621_DAT_L P1OUT&=~BIT5
#define HT1621_CS_H  P1OUT|=BIT1
#define HT1621_CS_L  P1OUT&=~BIT1
#define HT1621_WR_H  P1OUT|=BIT2
#define HT1621_WR_L  P1OUT&=~BIT2

//以下是DS18B20显示宏定义

#define DS18B20      BIT3
#define DS18B20_H    P1OUT |= BIT3
#define DS18B20_L    P1OUT &= ~BIT3

volatile uchar qian=0,bai=0,shi=0,ge=0;//qian,bai,shi,ge 四个变量分别表示液晶的千百十个位
volatile uchar sleep=0, time_flag=0,temp_flag=1,shi_flag=0,huanxing=0; 
  //sleep为睡眠标志位， temp_flag为温度唤醒标志位,time_flag为时间唤醒标志位
  //shi_flag为显示时唤醒标志位，huangxing为唤醒显示标志位
volatile uint jishu=0;               //电子表的秒计数
volatile uchar hour=0,min=0,sec=0;   //电子表的时，分，秒
uchar Ht1621Tab[]={0x00};            //LCD清屏数组
uchar Ht1621Tab1[]={0x0c,0x0a,0x0c,  //0  不带小数点表格
                    0x00,0x00,0x0c,  //1
                    0x04,0x0e,0x08,  //2
                    0x00,0x0e,0x0c,  //3
                    0x08,0x04,0x0c,  //4
                    0x08,0x0e,0x04,  //5
                    0x0c,0x0e,0x04,  //6
                    0x00,0x08,0x0c,  //7
                    0x0c,0x0e,0x0c,  //8
                    0x08,0x0e,0x0c}; //9
uchar Ht1621Tab2[]={0x0e,0x0a,0x0c,  //0    带小数点表格
                    0x02,0x00,0x0c,  //1
                    0x06,0x0e,0x08,  //2
                    0x02,0x0e,0x0c,  //3
                    0x0a,0x04,0x0c,  //4
                    0x0a,0x0e,0x04,  //5
                    0x0e,0x0e,0x04,  //6
                    0x02,0x08,0x0c,  //7
                    0x0e,0x0e,0x0c,  //8
                    0x0a,0x0e,0x0c}; //9
uchar Ht1621Tab3[]={0x0c,0x0a,0x0e,  //0  显示最前端1，LOW BATTERY CONTINUITY
                    0x00,0x00,0x0e,  //1
                    0x04,0x0e,0x0a,  //2
                    0x00,0x0e,0x0e,  //3
                    0x08,0x04,0x0e,  //4
                    0x08,0x0e,0x06,  //5
                    0x0c,0x0e,0x06,  //6
                    0x00,0x08,0x0e,  //7
                    0x0c,0x0e,0x0e,  //8
                    0x08,0x0e,0x0e}; //9
/******************************************************

                   函数声明

****************************************************/
void display (void);            //液晶显示
void display_shi(void);         //液晶显示中间两位
void temp_dsp(void);            //温度处理函数
void time_dsp(void);            //分秒处理函数
void shi_dsp(void);             //时处理函数
void stop_lcd(void);

/////////////  DS18B20函数声明   //////
void B20_init(void);
uchar B20_readB(void);
void B20_writeB(uchar wrd);
uint Read_temp(void);

void port_init(void)            //端口初始化
{
  P1DIR |= BIT1+BIT2+BIT3+BIT5; //P1.1 P1.2 P1.3 P1.5 OUT
  
  P2OUT |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5带上拉电阻
  P2IE  |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5中断使能
  P2IES |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5中断下降沿触发中断
  P2REN |=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5;   //P2.0-5上拉电阻使能
  P2IFG &=~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);//P2.0-5中断标志位清零
}
void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
}
void int_WDT(void)                 //看门狗设置
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void int_Time_A(void)              //定时器A初始化
{
  TACTL = TASSEL_1 +ID_3+ MC_1;    // ACLK，8div,upmode
  CCTL0 = CCIE;                    //CCR0 interrupt enabled
  CCR0 = 4096;                     //定时1s t=1S
}
void Delay_us(uint us)             // 延时us  前提是f=8MHZ（不建议用该延时，不是很精确）
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(8);      
}
void Delay_ms(uint ms)             // 延时ms  前提是f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
}

//以下是HT1621 LCD显示子程序

/******************************************************
写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
    uchar i;
    for (i=0;i<cnt;i++)
     {
        HT1621_WR_L;
        Delay_us(1);
        if(Data&0x80)
          HT1621_DAT_H;
        else
          HT1621_DAT_L;
        Delay_us(1);
        HT1621_WR_H;
        Delay_us(1);
        Data<<=1;
 }
}
/********************************************************
函数名称：void Ht1621WrCmd(uchar Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
说明：写入命令标识位100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
HT1621_CS_L;
Delay_us(1);
Ht1621Wr_Data(0x80,4); //写入命令标志100
Ht1621Wr_Data(Cmd,8); //写入命令数据
HT1621_CS_H;
Delay_us(1);
}
/********************************************************
函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
说明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
    HT1621_CS_L;
    Ht1621Wr_Data(0xa0,3);    //写入数据标志101
    Ht1621Wr_Data(Addr<<2,6); //写入地址数据
    Ht1621Wr_Data(Data<<4,4); //写入数据
    HT1621_CS_H;
    Delay_us(1);
}
/********************************************************
函数名称：void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
cnt为写入数据总数
说明：HT1621的数据位4位，此处每次数据为8位，写入数据
总数按8位计算
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
    uchar i;
    HT1621_CS_L;
    Ht1621Wr_Data(0xa0,3); //写入数据标志101
    Ht1621Wr_Data(Addr<<2,6); //写入地址数据
    for (i=0;i<cnt;i++)
      {
        Ht1621Wr_Data(0x00,8); //写入数据
        p++;
      }
    HT1621_CS_H;
    Delay_us(1);
}
/********************************************************
函数名称：void Ht1621_Init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
说明：初始化后，液晶屏所有字段均显示
********************************************************/
void Ht1621_Init(void)
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Delay_ms(20);       //延时使LCD工作电压稳定
    Ht1621WrCmd(BIAS);  //1/3duty 3com
    Ht1621WrCmd(RC256); //使用内部振荡器
    Ht1621WrCmd(SYSDIS);//关振系统荡器和LCD偏压发生器
    Ht1621WrCmd(WDTDIS);//禁止看门狗
    Ht1621WrCmd(TONEOFF);//关闭声音输出
    Ht1621WrCmd(SYSEN); //打开系统振荡器
    Ht1621WrCmd(LCDON); //打开LCD偏压
}

//以上是HT1621 LCD显示子程序

void init_devices(void)
{
   port_init();                     //IO口设置
   int_Clock();                     //时钟设置
   int_WDT();                       //看门狗设置
}
void main( void )
{  
   init_devices();                   // 初始化配置
   int_Time_A();                     //定时器A 的配置
   Ht1621_Init();                    //上电初始化LCD
   Ht1621WrAllData(0,Ht1621Tab,16);  //LCD清屏
   _EINT();                          //开总中断
  while(1)
  {   
       if(temp_flag)                 //是否显示温度判断
        {
           temp_dsp();               //温度采集与数据处理
           display();                //数据LCD四位显示
        }
       if(time_flag)                 //是否显示分和秒判断
        {
         time_dsp();
         display();                  //数据LCD四位显示
        }
       if(shi_flag)                 //是否显示时判断
       {
         shi_dsp();
         display_shi();
       }
       if(sleep)                    //睡眠标志位
       {
         stop_lcd();                //关闭LCD以及配置IO的输出
         LPM3;                      //进入睡眠LPM4
        }
       if(huanxing)                 //唤醒温度初始化
       {
         init_devices();            // 初始化配置
         Ht1621_Init();             //上电初始化LCD
         huanxing=0;
        } 
  }
}
void display (void)                               //显示四位子程序
{
       Ht1621WrOneData(0x01,Ht1621Tab1[3*qian]);
       Ht1621WrOneData(0x03,Ht1621Tab1[3*qian+1]);
       Ht1621WrOneData(0x05,Ht1621Tab1[3*qian+2]);//在此地址调用 Ht1621Tab3
                                                  //显示 最前端1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab2[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab2[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab3[3*shi+2]);//在此地址调用 Ht1621Tab3 
                                                 //显示字母LOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab3[3*ge+2]); //在此地址调用 Ht1621Tab3  
                                                 //显示CONTINUITY
}

void display_shi(void)                           //显示中间两位子程序
{
       Ht1621WrOneData(0x01,0x00);
       Ht1621WrOneData(0x03,0x00);
       Ht1621WrOneData(0x05,0x00);                //在此地址调用 Ht1621Tab3
                                                  //显示 最前端1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab1[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab1[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab1[3*shi+2]);//在此地址调用 Ht1621Tab3 
                                                 //显示字母LOW BATTERY
       Ht1621WrOneData(0x13,0x00);   
       Ht1621WrOneData(0x15,0x00);
       Ht1621WrOneData(0x17,0x00);               //在此地址调用 Ht1621Tab3  
                                                 //显示CONTINUITY
}

//以下是DS18B20温度检测子程序
/********************************
函数名称: B20_init
功    能: 复位DS18B20
参    数: 无
返回值  : 无
********************************/
void B20_init(void)
{
 P1DIR |= DS18B20;		     //配置为输出			  
 DS18B20_L;			     //拉低
 Delay_us(600);			     //等待600微秒
 DS18B20_H;			     //释放总线
 Delay_us(60);			     //等待60微秒
 P1DIR &= ~DS18B20;		     //配置为输入 
 while((P1IN &(DS18B20)));	     //等待DS18B20拉低
 while(!(P1IN &(DS18B20)));          //等待DS18B20释放总线
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
  P1DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动读数据位
  DS18B20_H;			//释放总线
  Delay_us(5);			//等待5微秒
  P1DIR &=~DS18B20;		//配置为输入，开始读取数据位
  if(P1IN&DS18B20)	        //该位是否为高
  {
   retd|=0x80;			//是就将此位置高
  }
  Delay_us(50);			//等待50微秒
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
  P1DIR |=DS18B20;		//配置为输出
  DS18B20_L;		        //拉低，启动写数据位
  Delay_us(1);			//等待1微秒
  if(wrd&0x01)			//此位数据是否为高
  {
   DS18B20_H;		        //是高则将单总线拉高
  }
  else
  {
   DS18B20_L;		        //是低则将单总线拉低
  }
  Delay_us(50);		        //等待50微秒
  DS18B20_H;		        //释放总线
  wrd>>=1;			//右移，为写入新的数据位做准备
 }
  Delay_us(50);			//等待50微秒
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
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0x44); //启动温度转换
 B20_init();	   //初始化，每次写命令都从初始化开始
 B20_writeB(0xcc); //跳过ROM
 B20_writeB(0xbe); //读寄存器
 templ=B20_readB();//读温度低字节
 temph=B20_readB();//读温度高字节
 temp=templ+temph*256;//将温度整理成16位变量
 
  return temp;	   //返回16位变量

}
//以上是DS18B20温度检测子程序

void temp_dsp(void)   //
{
   uint t;
   t=Read_temp();                    //读取温度值
   t=t*6.25;
   qian=t/1000;
   bai=(t-qian*1000)/100;
   shi=((t-qian*1000)-bai*100)/10;
   ge=t%10;   
}
void time_dsp(void)
{
  if(jishu>43200)
  {
     jishu=0;
  }
      min=jishu%3600/60;
      sec=jishu%60;
      
      qian=min/10;
      bai=min%10;
      shi=sec/10;
      ge=sec%10;
}

void shi_dsp(void)
{
  if(jishu>43200)
  {
     jishu=0;
  }
    hour=jishu/3600;
    bai=hour/10;
    shi=hour%10;   
}

void stop_lcd(void)      //关闭LCD子程序
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Ht1621WrCmd(LCDOFF); //关LCD偏压
    Ht1621WrCmd(TONEOFF);//关闭声音输出
                         //其实这个没大必要关，因为开始就没有打开
    Ht1621WrCmd(TIMERDIS);
    Ht1621WrCmd(SYSDIS); //关振系统荡器和LCD偏压发生器
      
    P1DIR=0XFF;        //设置IO
    P1OUT =0XFF;       //
    P2DIR=0xC0;        //
    P2OUT |=0xC0;      //
}
//timeA定时中断服务程序
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  jishu++;
}

//P2中断服务程序

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
  if ((P2IN&BIT0)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN&BIT0)==0)
    {
      sleep=1;               //进入睡眠LPM3
    }
  }
  if((P2IN&BIT1)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN&BIT1)==0)
    {
      huanxing=1;            //唤醒LPM3
      temp_flag=1;           //温度标志位置1，       显示温度
      time_flag=0;           //时间分秒标志位置0，   同时关闭分秒显示
      shi_flag=0;            //时标志位置0，         同时关闭时显示
      sleep=0;               //同时对睡眠标志位清零，开始显示温度
      LPM3_EXIT;             //退出睡眠LPM3
     }
   }
  if((P2IN&BIT2)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN&BIT2)==0)
    {
      huanxing=1;            //唤醒LPM3
      time_flag=1;           //时间分秒标志位置1，   显示分秒
      temp_flag=0;           //温度标志位置0，       同时关闭温度显示
      shi_flag=0;            //时标志位置0，         同时关闭时的显示
      sleep=0;               //同时对睡眠标志位清零，开始显示温度
      LPM3_EXIT;             //退出睡眠LPM3
    }
  }
  if((P2IN & BIT3)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN & BIT3)==0)
    {
      jishu=jishu+60;        //时间分的加一，(前提是唤醒时)  
      Delay_ms(10);          //消抖10ms
    }
  }
  
  if((P2IN & BIT4)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN & BIT4)==0)
    {
      huanxing=1;            //唤醒LPM3
      shi_flag=1;            //时间时标志位置1，     显示时
      time_flag=0;           //时间分秒标志位置0，   同时关闭分秒的显示
      temp_flag=0;           //温度标志位置0，       同时关闭分秒的显示
      sleep=0;               //同时对睡眠标志位清零，开始显示时
      LPM3_EXIT;             //退出睡眠LPM3
    }
  }
  
  if((P2IN & BIT5)==0)
  {
    Delay_ms(10);            //消抖10ms
    if((P2IN & BIT5)==0)
    {
      jishu=jishu+3600;      //时间时的加一，(前提是唤醒时) 
      Delay_ms(10);          //消抖10ms
    }
  }
    P2IFG =0;                //P2中断标志位清零
}  
