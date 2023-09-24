/*******************************************************************

            程序功能:  HT1621芯片测试程序 ：6个LED数码管显示电子表
            采用CPU ：MSP430G2553 	  8MHZ
            程序说明: 本测试程序能够6个LED数码管显示显示电子表

硬件连接图：
            HT1621_DAT.................BIT0  //HT1621数据引脚
            HT1621_WR..................BIT1  //HT1621时钟引脚(CLK) 
            HT1621_CS..................BIT2  //HT1621使能引脚 
********************************************************************/
#include <msp430f5529.h>
#define uchar unsigned char 
#define uint unsigned int


//HT1621控制位（液晶模块接口定义，根据自已的需要更改） 
#define HT1621_DAT   BIT2  //HT1621数据引脚
#define HT1621_WR    BIT3  //HT1621时钟引脚 
#define HT1621_CS    BIT4  //HT1621使能引脚 

#define HT1621_DAT_H P1OUT |= BIT2
#define HT1621_DAT_L P1OUT &= ~BIT2
#define HT1621_WR_H  P1OUT |= BIT3
#define HT1621_WR_L  P1OUT &= ~BIT3
#define HT1621_CS_H  P1OUT |= BIT4
#define HT1621_CS_L  P1OUT &= ~BIT4
void clock_init()
{
  volatile unsigned int i;
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
 // __delay_cycles(375000);
	
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);     
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

//六位LCD显示子函数
void LCD_Display(void)
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
//以上是HT1621 LCD显示所有子函数

void port_init(void)                       //端口初始化
{
  P1DIR |= HT1621_DAT+HT1621_WR+HT1621_CS; //定义HT1621输出
}


void int_WDT(void)                 //看门狗设置
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}

void init_devices(void)
{
   port_init();                     //IO口设置
   clock_init();                     //时钟设置
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
  uint i=0;
  init_devices();                       // 初始化配置
  Ht1621_Init();                        //上电初始化LCD 
  //Delay_ms(10);                      //延时一段时间 
  Ht1621WrAllData(0,Ht1621Tab,16);      //清除1621寄存器数据，暨清屏 
  _EINT();                              //开总中断
  while(1)
  { 
    for(i=0;i<10;i++)
    {
      R1++;
      if(R1>9)
      {
        R1=0;
        R2++;
        if(R2>5)
        {
          R2=0;
          R3++;
          if(R3>9)
          {
           R3=0;
           R4++;
           if(R4>5)
           {
             R4=0;
             R5++;
           }
          }
        }
      }
      LCD_Display();
      Delay_ms(1000);                 //一秒的延时
    }
  }    
} 