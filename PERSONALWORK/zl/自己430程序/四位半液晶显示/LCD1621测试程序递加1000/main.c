#include <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned 
#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN   0X02  //0b1000 0000 0010 打开系统振荡器
#define LCDOFF  0X04  //0b1000 0000 0100 关LCD偏压
#define LCDON   0X06  //0b1000 0000 0110 打开LCD偏压
#define XTAL    0x28  //0b1000 0010 1000 外部接时钟
#define RC256   0X30  //0b1000 0011 0000 内部时钟
#define TONEON  0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A  //0b1000 0000 1010 禁止看门狗
#define HT1621_DAT_H P1OUT|=BIT0
#define HT1621_DAT_L P1OUT&=~BIT0
#define HT1621_CS_H  P1OUT|=BIT1
#define HT1621_CS_L  P1OUT&=~BIT1
#define HT1621_WR_H  P1OUT|=BIT2
#define HT1621_WR_L  P1OUT&=~BIT2
volatile uchar qian=0,bai=0,shi=0,ge=0;//qian,bai,shi,ge 四个变量分别表示液晶的千百十个位
uchar Ht1621Tab[]={0x00};
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
uchar Ht1621Tab3[]={0x0c,0x0a,0x0e,  //0
                    0x00,0x00,0x0e,  //1
                    0x04,0x0e,0x0a,  //2
                    0x00,0x0e,0x0c,  //3
                    0x08,0x04,0x0c,  //4
                    0x08,0x0e,0x04,  //5
                    0x0c,0x0e,0x04,  //6
                    0x00,0x08,0x0c,  //7
                    0x0c,0x0e,0x0c,  //8
                    0x08,0x0e,0x0c}; //9
void display (void);
void port_init(void)               //端口初始化
{
  P1DIR |=BIT0+BIT1+BIT2;  
}
void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_1MHZ;           //DCO数字振荡器=1MHZ
  DCOCTL  = CALBC1_1MHZ;           //DCO数字振荡器=1MHZ 
}
void int_WDT(void)                 //看门狗设置
{
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void Delay_us(uint us)           // 延时us  前提是f=1MHZ
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(1);      
}
void Delay_ms(uint ms)           // 延时ms  前提是f=1MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(1000); 
}
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
    Delay_ms(200); //延时使LCD工作电压稳定
    Ht1621WrCmd(BIAS);
    Ht1621WrCmd(RC256); //使用内部振荡器
    Ht1621WrCmd(SYSDIS);
    Ht1621WrCmd(WDTDIS);
    Ht1621WrCmd(SYSEN);
    Ht1621WrCmd(LCDON);
}
void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;     //watchdog
}
void main( void )
{
   init_devices();               // 初始化配置
   Ht1621_Init();                //上电初始化LCD
   Delay_ms(1);                  //延时一段时间
   Ht1621WrAllData(0,Ht1621Tab,16);
   while(1)
     {
       ge++;
       if(ge>9)
       {
         ge=0;
         shi++;
         if(shi>9)
         {
          shi=0;
          bai++;
          if(bai>9)
          {
            bai=0;
            qian++;
            if(qian>9)
            {
              qian=0;
            }
          }
         }
       }
       display ();
       Delay_ms(100);
       Ht1621WrAllData(0,Ht1621Tab,16);
      }
}
void display (void)
{
       Ht1621WrOneData(0x01,Ht1621Tab1[3*qian]);
       Ht1621WrOneData(0x03,Ht1621Tab1[3*qian+1]);
       Ht1621WrOneData(0x05,Ht1621Tab1[3*qian+2]);//调用 Ht1621Tab3
                                                  //显示 最前端1
       Ht1621WrOneData(0x07,Ht1621Tab1[3*bai]);
       Ht1621WrOneData(0x09,Ht1621Tab1[3*bai+1]);
       Ht1621WrOneData(0x0b,Ht1621Tab1[3*bai+2]);
       
       Ht1621WrOneData(0x0d,Ht1621Tab1[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab1[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab1[3*shi+2]);//调用 Ht1621Tab3 
                                                 //显示字母LOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab1[3*ge+2]); //调用 Ht1621Tab3  
                                                 //显示CONTINUITY
}
