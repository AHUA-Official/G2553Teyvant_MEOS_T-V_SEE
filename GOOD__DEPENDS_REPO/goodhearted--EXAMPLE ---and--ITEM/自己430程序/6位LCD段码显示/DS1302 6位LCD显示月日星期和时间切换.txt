/*******************************************************************

            程序功能:6个LED数码管显示百年历（含电子表）
            采用CPU ：MSP430G2553  内部8MHZ
            

硬件连接图：
            HT1621_DAT.................P1_BIT0  //HT1621数据引脚
            HT1621_WR..................P1_BIT1  //HT1621时钟引脚(CLK) 
            HT1621_CS..................P1_BIT2  //HT1621使能引脚 

            DS1302_CLK.................P2_BIT0  //DS1302串行时钟
            DS1302_IO .................P2_BIT1  //DS1302双向数据线
            DS1302_RS .................P2_BIT2  //DS1302复位/片选线
            按键      .................P1_BIT3  //时间和日期切换

********************************************************************/
#include <msp430g2553.h>
#define uchar unsigned char 
#define uint unsigned int

volatile uchar Date_Time_Flag=1; //Date_Time_Flag=1表示时间标志，为0表示日期标志
uchar year2=20,
      year = 0x12,		 //设置年初值为09
      month= 0x09,		 //设置月初值为02
      week,                      //星期不用设置
      day  = 0x03,		 //设置日初值为03
      hour = 0x20,		 //设置时初值为16
      min  = 0x22,		 //设置分初值为15
      sec  = 0x30;   	         //设置秒初值为30


//DS1302控制位（接口定义，根据自已的需要更改）
#define DS1302_CLK       BIT0                 //DS1302串行时钟
#define DS1302_IO        BIT1                 //DS1302双向数据线 
#define DS1302_RST       BIT2                 //DS1302复位/片选线
#define Date_Time        BIT3                 //DS1302时间与日期切换


#define DS1302_CLK_H     P2OUT |=  DS1302_CLK //
#define DS1302_CLK_L     P2OUT &= ~DS1302_CLK //
#define DS1302_IO_IN     P2DIR &= ~DS1302_IO  //定义DS1302双向数据线输入  
#define DS1302_IO_OUT    P2DIR |=  DS1302_IO  //定义DS1302双向数据线输出
#define DS1302_IO_OUT_H  P2OUT |=  DS1302_IO 
#define DS1302_IO_OUT_L  P2OUT &= ~DS1302_IO
#define DS1302_RST_H     P2OUT |=  DS1302_RST  //
#define DS1302_RST_L     P2OUT &= ~DS1302_RST  //


//HT1621控制位（液晶模块接口定义，根据自已的需要更改） 
#define HT1621_DAT   BIT0  //HT1621数据引脚
#define HT1621_WR    BIT1  //HT1621时钟引脚 
#define HT1621_CS    BIT2  //HT1621使能引脚 


#define HT1621_DAT_H P1OUT |= BIT0
#define HT1621_DAT_L P1OUT &= ~BIT0
#define HT1621_WR_H  P1OUT |= BIT1
#define HT1621_WR_L  P1OUT &= ~BIT1
#define HT1621_CS_H  P1OUT |= BIT2
#define HT1621_CS_L  P1OUT &= ~BIT2

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
#define BIAS       0x52 //0b1000 0101 0010  1/3duty 4com 
#define SYSDIS     0X00 //0b1000 0000 0000  关振系统荡器和LCD偏压发生器 
#define SYSEN      0X02 //0b1000 0000 0010  打开系统振荡器 
#define LCDOFF     0X04 //0b1000 0000 0100  关LCD偏压 
#define LCDON      0X06 //0b1000 0000 0110  打开LCD偏压 
#define TIMERDIS   0X08 //0b1000 0000 0110  禁止时基输出
#define XTAL       0x28 //0b1000 0010 1000  外部接时钟 
#define RC256      0X30 //0b1000 0011 0000  内部时钟 
#define TONEON     0X12 //0b1000 0001 0010  打开声音输出 
#define TONEOFF    0X10 //0b1000 0001 0000  关闭声音输出 
#define WDTDIS     0X0A //0b1000 0000 1010  禁止看门狗 

//LCD六位显示(R1最低位即：最右边一位)，同理R2，R3.....
volatile uchar R1=0,R2=0,R3=0,R4=0,R5=0,R6=0;

uchar  Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00, //32个地址缓冲区
                    0x00,0x00,0x00,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00};         

//定义0-9数字不带小数点的二维数组
uchar SEG_7[10][2]=   
 { 
  0xc4,0xc8,//00 
  0x04,0x40,//01  
  0x4c,0x88,//02 
  0x4c,0x48,//03
  0x8c,0x40,//04
  0xc8,0x48,//05
  0xc8,0xc8,//06
  0x44,0x40,//07
  0xcc,0xc8,//08
  0xcc,0x48,//09
 }; 
//定义0-9数字带小数点的二维数组 
//注意：1.最右边数字的小数点不存在，只有COL1= ：（即显示时间秒的两个冒号：）
uchar SEG_8[10][2]=   
 { 
  0xc4,0xcc,//00 
  0x04,0x44,//01  
  0x4c,0x8c,//02 
  0x4c,0x4c,//03
  0x8c,0x44,//04
  0xc8,0x4c,//05
  0xc8,0xcc,//06
  0x44,0x44,//07
  0xcc,0xcc,//08
  0xcc,0x4c,//09
 };

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
        Ht1621Wr_Data(*p,8); //写入数据
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
    Delay_ms(10);       //延时使LCD工作电压稳定
    Ht1621WrCmd(BIAS);  //1/3duty 4com
    Ht1621WrCmd(RC256); //使用内部振荡器
    Ht1621WrCmd(SYSDIS);//关振系统荡器和LCD偏压发生器
    Ht1621WrCmd(WDTDIS);//禁止看门狗
    Ht1621WrCmd(TONEOFF);//关闭声音输出
    Ht1621WrCmd(SYSEN); //打开系统振荡器
    Ht1621WrCmd(LCDON); //打开LCD偏压
}

//六位LCD显示时间(时分秒)子函数
void LCD_Display_Time(void)
{
      Ht1621Tab[2]=SEG_7[R6][0];       //最左边第一位数码管
      Ht1621Tab[3]=SEG_7[R6][1];       //想显示小数点，在此调用SEG_8[R6][1]

      Ht1621Tab[4]=SEG_7[R5][0];       //最左边第二位数码管
      Ht1621Tab[5]=SEG_7[R5][1];       //想显示小数点，在此调用SEG_8[R5][1]
      
      Ht1621Tab[6]=SEG_7[R4][0];       //最左边第三位数码管
      Ht1621Tab[7]=SEG_7[R4][1];       //想显示小数点，在此调用SEG_8[R4][1]
      
      Ht1621Tab[8]=SEG_7[R3][0];       //最左边第四位数码管
      Ht1621Tab[9]=SEG_7[R3][1];       //想显示小数点，在此调用SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[R2][0];      //最左边第五位数码管
      Ht1621Tab[11]=SEG_7[R2][1];       //想显示小数点，在此调用SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[R1][0];      //最左边第六位数码管
      Ht1621Tab[13]=SEG_7[R1][1];     

      Ht1621Tab[13]=SEG_8[R1][1];     //显示时间秒小数点 即(:)COL1
//      Ht1621Tab[14]=0xcc;             //显示R1,R2,R3,R4
      Ht1621Tab[15]=0x40;             //显示时间分小数点 即(:)COL2
//      Ht1621Tab[15]=0x0c;             //显示L1,L2  
      Ht1621WrAllData(0,Ht1621Tab,16);//写1621寄存器数据，即显示    
}

//六位LCD显示日期(年月日)子函数
void LCD_Display_Date(void)
{
      Ht1621Tab[2]=SEG_7[R6][0];       //最左边第一位数码管
      Ht1621Tab[3]=SEG_7[R6][1];       //想显示小数点，在此调用SEG_8[R6][1]

      Ht1621Tab[4]=SEG_7[R5][0];       //最左边第二位数码管
      Ht1621Tab[5]=SEG_8[R5][1];       //想显示小数点，在此调用SEG_8[R5][1]
      
      Ht1621Tab[6]=SEG_7[R4][0];       //最左边第三位数码管
      Ht1621Tab[7]=SEG_7[R4][1];       //想显示小数点，在此调用SEG_8[R4][1]
      
      Ht1621Tab[8]=SEG_7[R3][0];       //最左边第四位数码管
      Ht1621Tab[9]=SEG_8[R3][1];       //想显示小数点，在此调用SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[R2][0];      //最左边第五位数码管
      Ht1621Tab[11]=SEG_7[R2][1];       //想显示小数点，在此调用SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[R1][0];      //最左边第六位数码管
      Ht1621Tab[13]=SEG_8[R1][1];     

//      Ht1621Tab[13]=0x00;             //不显示时间秒小数点 即(:)COL1
//      Ht1621Tab[14]=0xcc;             //显示R1,R2,R3,R4
      Ht1621Tab[15]=0x00;             //不显示时间分小数点 即(:)COL2
//      Ht1621Tab[15]=0x0c;             //显示L1,L2  
      Ht1621WrAllData(0,Ht1621Tab,16);//写1621寄存器数据，即显示    
}
//以上是HT1621 LCD显示所有子函数

//以下是DS1302所有子函数
/****************************************************************
        ds1302的操作函数 

        DS1302 写一个字节
**************************************************/
void DS1302_WriteByte(uchar addr,uchar dat)
{
   uchar i,temp;
   DS1302_RST_L;               // RST拉低，数据传送终止
   DS1302_CLK_L;       
   DS1302_RST_H;               // 逻辑控制有效
   temp = addr;
   DS1302_IO_OUT;
   Delay_us(5);
   for(i=0;i<8;i++)     // 上升沿写入数据
   {
      DS1302_CLK_L;
	  if(temp&0x01)   
            DS1302_IO_OUT_H;
	  else            
            DS1302_IO_OUT_L;
          DS1302_CLK_H;
	  temp >>= 1;
   }
   temp = dat;
   DS1302_IO_OUT;
   Delay_us(5);
   for(i=0;i<8;i++)
   {
     DS1302_CLK_L;
	  if(temp&0x01)   
            DS1302_IO_OUT_H;
	  else           
            DS1302_IO_OUT_L;
           DS1302_CLK_H;
	  temp >>= 1;
   }
   DS1302_RST_L; 
}

/*************************************************
        DS1302 读一个字节
**************************************************/
uchar DS1302_ReadByte(uchar addr)
{
   uchar i,temp;       
   DS1302_RST_L; // RST拉低，数据传送终止     
   DS1302_CLK_L; 
   DS1302_RST_H; // 逻辑控制有效
   temp = addr;
   DS1302_IO_OUT;
   Delay_us(5);
   for(i=0;i<8;i++)     // 上升沿写入数据
   {
     DS1302_CLK_L;
	  if(temp&0x01) 
            DS1302_IO_OUT_H;
	  else           
            DS1302_IO_OUT_L;
          DS1302_CLK_H;
	  temp >>= 1;
   }
   Delay_us(10);
   DS1302_IO_IN;
   temp = 0;
   for(i=0;i<8;i++)     // 下降沿读出数据
   {
      temp >>= 1;
      DS1302_CLK_L;
      Delay_us(1);
             if(P2IN & DS1302_IO)
	  {
	     temp |= 0x80;
        }
      DS1302_CLK_H;
   }
   DS1302_RST_L;
   return temp;
}

/*******************************************************
        DS1302设置时间函数 设置值可在主函数中设置
********************************************************/
void DS1302_SetTime(uchar *sec,uchar *min,uchar *hour,uchar *day,uchar *month,uchar *year)
{
   DS1302_WriteByte(0x8e,0x00);  // 写保护为0，可以进行写操作
   DS1302_WriteByte(0x8c,*year); // 年
   
   DS1302_WriteByte(0x88,*month);// 月
   DS1302_WriteByte(0x86,*day);  // 日
   DS1302_WriteByte(0x84,*hour); // 时，24小时制
   DS1302_WriteByte(0x82,*min);  // 分
   DS1302_WriteByte(0x80,*sec);  // 秒
}

/********************************************************
           DS1302 读取时间
*********************************************************/
void DS1302_GetTime(uchar *sec,uchar *min,uchar *hour,uchar *day,uchar *month,uchar *week,uchar *year)
{
   *sec   = DS1302_ReadByte(0x81);     //读出来的值高四位为十进制的十位  低四位为十进制的个位
   *sec   = (*sec>>4)*10+(*sec&0x0f);  //转换成十进制
   *min   = DS1302_ReadByte(0x83);
   *min   = (*min>>4)*10+(*min&0x0f);   
   *hour  = DS1302_ReadByte(0x85);
   *hour  = (*hour>>4)*10+(*hour&0x0f);
   *day   = DS1302_ReadByte(0x87);
   *day   = (*day>>4)*10+(*day&0x0f);
   *month = DS1302_ReadByte(0x89);
   *month = (*month>>4)*10+(*month&0x0f);
   *week = DS1302_ReadByte(0x8b);
   *week = (*week&0x0f);
   *year  = DS1302_ReadByte(0x8d);
   *year  = (*year>>4)*10+(*year&0x0f);
}
//以上是DS1302所有子函数

void port_init(void)                       //端口初始化
{
  P1DIR |= HT1621_DAT+HT1621_WR+HT1621_CS; //定义HT1621输出
  
  P1DIR &= ~Date_Time;    //定义DS1302 Date_Time转换输入
  P1OUT |=  Date_Time;    //DS1302 Date_Time转换上拉电阻
  P1REN |=  Date_Time;    //使能DS1302 Date_Time转换的上拉电阻
  P1IE  |=  Date_Time;    //DS1302 Date_Time转换口中断使能
  P1IES |=  Date_Time;    //DS1302 Date_Time转换口边沿触发中断方式设置
  P1IFG &= ~Date_Time;    //DS1302 Date_Time转换口中断标志位清除
  
  
  P2DIR |= DS1302_CLK+DS1302_RST;          //定义DS1302输出
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

void init_devices(void)
{
   port_init();                     //IO口设置
   int_Clock();                     //时钟设置
   int_WDT();                       //看门狗设置
}
void stop_lcd(void)        //关闭LCD子程序
{
    HT1621_CS_H;
    HT1621_WR_H;
    HT1621_DAT_H;
    Ht1621WrCmd(LCDOFF);   //关LCD偏压
    Ht1621WrCmd(TONEOFF);  //关闭声音输出
                           //其实这个没大必要关，因为开始就没有打开
    Ht1621WrCmd(TIMERDIS); //禁止时基输出
    Ht1621WrCmd(SYSDIS);   //关振系统荡器和LCD偏压发生器
    P1DIR =0XFF;           //设置IO
    P1OUT =0XFF;     
    P2DIR =0xff;       
    P2OUT =0xff;     
}

/******************************************************** 
函数名称：main() 
功能描述: 主程序
********************************************************/
void main() 
{ 
  init_devices();                      // 初始化配置
  Ht1621_Init();                       //上电初始化LCD 
  Ht1621WrAllData(0,Ht1621Tab,16);     //清除1621寄存器数据，清屏 
//  DS1302_SetTime(&sec,&min,&hour,&day,&month,&year);//设置DS1302时间寄存器的初值
  Delay_ms(10);
  _EINT();                             //开总中断
  while(1)
  { 
     if(Date_Time_Flag)   //Date_Time_Flag=1表示时间标志，为0表示日期标志
     {
        DS1302_GetTime(&sec,&min,&hour,&day,&month,&week,&year);//读取DS1302各个时间寄存器的值
        R1=sec%10;
        R2=sec/10;
        R3=min%10;
        R4=min/10;
        R5=hour%10;
        R6=hour/10;
        LCD_Display_Time();
     }
     else
     {
       DS1302_GetTime(&sec,&min,&hour,&day,&month,&week,&year);//读取DS1302各个时间寄存器的值
       R1=week%10;
       R2=week/10;
       R3=day%10;
       R4=day/10;
       R5=month%10;
       R6=month%100/10;
       LCD_Display_Date();
     }
    }    
} 

//P1中断服务程序
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{ 
    Delay_ms(10);
    if((P1IN & Date_Time)==0)
    {
      if(Date_Time_Flag)    //Date_Time_Flag=1表示时间标志，为0表示日期标志
        Date_Time_Flag=0;
      else
        Date_Time_Flag=1;
    }
    P1IFG =0;
}  