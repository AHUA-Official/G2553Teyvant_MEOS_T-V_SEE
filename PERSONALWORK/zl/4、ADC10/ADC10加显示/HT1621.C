
#include "HT1621.H"

//LCD六位显示(R1最低位即：最右边一位)，同理R2，R3.....
volatile uchar R1=1,R2=2,R3=3,R4=4,R5=5,R6=6;

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

void port_init(void)                       //端口初始化
{
        P1DIR |= HT1621_DAT+HT1621_WR+HT1621_CS;
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

//      Ht1621Tab[13]=SEG_8[R1][1];     //显示时间秒小数点 即(:)COL1
//      Ht1621Tab[14]=0xcc;             //显示R1,R2,R3,R4
//      Ht1621Tab[15]=0x40;             //显示时间分小数点 即(:)COL2
//      Ht1621Tab[15]=0x0c;             //显示L1,L2  
      Ht1621WrAllData(0,Ht1621Tab,16);//写1621寄存器数据，即显示    
}
//以上是HT1621 LCD显示所有子函数

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
    
}
