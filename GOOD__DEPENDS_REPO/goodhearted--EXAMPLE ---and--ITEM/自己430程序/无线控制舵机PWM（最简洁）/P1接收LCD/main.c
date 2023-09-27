/****************************************************
           本实验是利用无线nRF24L01传输接收程序.
           制作无线遥控小车。

           并利用6位LCD得后两位显示当前舵机的PWM_24的值
              
************************硬件连接*********************

                    nRF 2401的连接
             nRF 2401的连接(6个IO)
        CE   -------------------------------P1.0
        SCK  -------------------------------P1.1
        MISO -------------------------------P1.2
        CSN  -------------------------------P1.3
        MOSI -------------------------------P1.4
        IRQ  -------------------------------P1.5
        nRF 2401 + -------------------------VCC
        nRF 2401 -  ------------------------GND

        6位LCD HT1621的连接（用做观察PWM_24）即占空比

        HT1621_DI ..................P2.0  //HT1621数据引脚
        HT1621_CLK..................P2.1  //HT1621时钟引脚(CLK) 
        HT1621_CS ..................P2.2  //HT1621使能引脚 
        
        舵机控制的连接

        舵机控制脚..................P2.4  //


******************************************************/

#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
}
void int_WDT(void)                 //看门狗设置
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
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

#define PWM_24       BIT4   // P2_4 定义舵机的引脚

//以下是HT1621 LCD显示所需配置

//HT1621控制位（液晶模块接口定义，根据自已的需要更改） 
#define HT1621_DI     BIT0  //HT1621数据引脚
#define HT1621_CLK    BIT1  //HT1621时钟引脚 
#define HT1621_CS     BIT2  //HT1621使能引脚 

#define HT1621_DI_H P2OUT |= BIT0
#define HT1621_DI_L P2OUT &= ~BIT0
#define HT1621_CLK_H  P2OUT |= BIT1
#define HT1621_CLK_L  P2OUT &= ~BIT1
#define HT1621_CS_H  P2OUT |= BIT2
#define HT1621_CS_L  P2OUT &= ~BIT2

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
uchar  Ht1621Tab_0[]={0x00};                       //清屏数组
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

/******************************************************
写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
    uchar i;
    for (i=0;i<cnt;i++)
     {
        HT1621_CLK_L;
        Delay_us(1);
        if(Data&0x80)
          HT1621_DI_H;
        else
          HT1621_DI_L;
        Delay_us(1);
        HT1621_CLK_H;
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
    HT1621_CLK_H;
    HT1621_DI_H;
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
      Ht1621Tab[2]=0x00;       //不最左边第一位数码管
      Ht1621Tab[3]=0x00;       //想显示小数点，在此调用SEG_8[R6][1]

      Ht1621Tab[4]=0x00;       //不最左边第二位数码管
      Ht1621Tab[5]=0x00;       //想显示小数点，在此调用SEG_8[R5][1]
      
      Ht1621Tab[6]=0x00;       //不最左边第三位数码管
      Ht1621Tab[7]=0x00;       //想显示小数点，在此调用SEG_8[R4][1]
      
      Ht1621Tab[8]=0x00;       //不最左边第四位数码管
      Ht1621Tab[9]=0x00;       //想显示小数点，在此调用SEG_8[R3][1]
      
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

#define TX_ADDR_WITDH 5  //发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5  //接收地址宽度设置为5个字节
#define TX_DATA_WITDH 8  //发送数据位数
#define RX_DATA_WITDH 8  //接收数据位数

//nRF24L01对应引脚

#define CE     BIT0
#define SCK    BIT1
#define MISO   BIT2 //input
#define CSN    BIT3
#define MOSI   BIT4

#define CE_H   P1OUT |= BIT0
#define CE_L   P1OUT &= ~BIT0
#define IRQ    BIT5  

#define SCK_H  P1OUT |= BIT1
#define SCK_L  P1OUT &= ~BIT1              
#define MISO_H P1OUT |= BIT2
#define MISO_L P1OUT &= ~BIT2
#define CSN_H  P1OUT |= BIT3
#define CSN_L  P1OUT &= ~BIT3
#define MOSI_H P1OUT |= BIT4
#define MOSI_L P1OUT &= ~BIT4       
#define IRQ_H  P1OUT |=BIT5 
#define IRQ_L  P1OUT &= ~BIT5

#define R_REGISTER    0x00  // 读寄存器
#define W_REGISTER    0x20  // 写寄存器
#define R_RX_PLOAD    0x61  // 读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式
#define W_TX_PLOAD    0xA0  // 写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式
#define FLUSH_TX    0xE1  // 清除TX FIFO寄存器，应用于发射模式
#define FLUSH_RX    0xE2  // 清除RX FIFO寄存器，应用于接收模式
#define REUSE_TX_PL 0xE3  // 重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射
#define NOP         0xFF  // 空操作，可以用来读状态寄存器

#define CONFIG      0x00  // 配置寄存器
#define EN_AA       0x01  // “自动应答”功能寄存
#define EN_RX_ADDR  0x02  // 接收通道使能寄存器
#define SETUP_AW    0x03  // 地址宽度设置寄存器
#define SETUP_RETR  0x04  // 自动重发设置寄存器
#define RF_CH       0x05  // 射频通道频率设置寄存器
#define RF_SETUP    0x06  // 射频设置寄存器
#define STATUS      0x07  // 状态寄存器
#define OBSERVE_TX  0x08  // 发送检测寄存器
#define CD          0x09  // 载波检测寄存器
#define RX_ADDR_P0  0x0A  // 数据通道0接收地址寄存器
#define RX_ADDR_P1  0x0B  // 数据通道1接收地址寄存器
#define RX_ADDR_P2  0x0C  // 数据通道2接收地址寄存器
#define RX_ADDR_P3  0x0D  // 数据通道3接收地址寄存器
#define RX_ADDR_P4  0x0E  // 数据通道4接收地址寄存器
#define RX_ADDR_P5  0x0F  // 数据通道5接收地址寄存器
#define TX_ADDR     0x10  // 发送地址寄存器
#define RX_PW_P0    0x11  // 数据通道0有效数据宽度设置寄存器
#define RX_PW_P1    0x12  // 数据通道1有效数据宽度设置寄存器
#define RX_PW_P2    0x13  // 数据通道2有效数据宽度设置寄存器
#define RX_PW_P3    0x14  // 数据通道3有效数据宽度设置寄存器
#define RX_PW_P4    0x15  // 数据通道4有效数据宽度设置寄存器
#define RX_PW_P5    0x16  // 数据通道5有效数据宽度设置寄存器
#define FIFO_STATUS 0x17  // FIFO状态寄存器

uchar sta;                // 状态变量
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志

//为了和别人无线模块冲突，TX_Addr[]最好不要雷同
uchar  TX_Addr[]={0x12,0x89,0x70,0x81,0x16};
//uchar  TX_Buffer[]={3,3,3,3};
uchar RX_Buffer[RX_DATA_WITDH];

//nRF24L01初始化
void nRF24L01_Init(void)
{
 Delay_ms(2);
 CE_L;//待机模式Ⅰ
 CSN_H;
 SCK_L;
 IRQ_H;
}

//SPI时序函数
uchar SPI_RW(uchar byte)
{
 uchar i;
 for(i=0;i<8;i++)                  //一字节8位循环8次写入
 {
  if(byte&0x80)                    //如果数据最高位是1
   MOSI_H;                         //向NRF24L01写1
  else                             //否则写0
   MOSI_L;
  byte<<=1;                        //低一位移到最高位
  SCK_H;                           //SCK拉高，写入一位数据，同时读取一位数据
  if(P1IN & MISO)
   byte|=0x01;
  SCK_L;                           //SCK拉低
 }
 return byte;                      //返回读取一字节
}

//SPI写寄存器一字节函数
//reg:寄存器地址
//value:一字节（值）
uchar SPI_W_Reg(uchar reg,uchar value)
{
 uchar status;                     //返回状态
 CSN_L;                            //SPI片选
 status=SPI_RW(reg);               //写入寄存器地址，同时读取状态
 SPI_RW(value);                    //写入一字节
 CSN_H;                            //
 return status;                    //返回状态
}

//SPI读一字节
uchar SPI_R_byte(uchar reg)
{
 uchar status;
 CSN_L;
 SPI_RW(reg);
 status=SPI_RW(0);
 CSN_H;
 return status;
}

//SPI读取RXFIFO寄存器数据
//reg:寄存器地址
//Dat_Buffer:用来存读取的数据
//DLen:数据长度
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
 uchar reg_value,i;
 CSN_L;
 reg_value=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  Dat_Buffer[i]=SPI_RW(0);
 }
 CSN_H;
 return reg_value;
}

//SPI向TXFIFO寄存器写入数据
//reg:写入寄存器地址
//TX_Dat_Buffer:存放需要发送的数据
//Dlen:数据长度
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen)
{
 uchar reg_value,i;
 CSN_L;
 reg_value=SPI_RW(reg);
 for(i=0;i<Dlen;i++)
 {
  SPI_RW(TX_Dat_Buffer[i]);
 }
 CSN_H;
 return reg_value; 
}

//设置接收模式
void nRF24L01_Set_RX_Mode(void)
{
 CE_L;//待机
 SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);
 SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);
 SPI_W_Reg(W_REGISTER+EN_AA,0x01);//auot ack
 SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);
 SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);
 SPI_W_Reg(W_REGISTER+RX_PW_P0,RX_DATA_WITDH);
 SPI_W_Reg(W_REGISTER+RF_CH,0);
 SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);//0db,lna
 SPI_W_Reg(W_REGISTER+CONFIG,0x0f);
 CE_H;
 Delay_ms(5);
}

//检测应答信号
uchar nRF24L01_RX_Data(void)
{
 sta=SPI_R_byte(R_REGISTER+STATUS);
 if(RX_DR)
 {
  CE_L;
  SPI_R_DBuffer(R_RX_PLOAD,RX_Buffer,RX_DATA_WITDH);
  SPI_W_Reg(W_REGISTER+STATUS,0xff);
  CSN_L;
  SPI_RW(FLUSH_RX);
  CSN_H;
  return 1;
 }
 else
  return 0;
}
void port_init(void)               //端口初始化
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI;
  P1DIR &= ~MISO;
  
  P2DIR |= HT1621_DI+HT1621_CLK+HT1621_CS; //定义HT1621输出
  
  P2DIR |= PWM_24;                   // P2.2用于外围模块输出即：产生PWM2
  P2SEL |= PWM_24;
}

void int_Time_A(void)              //定时器A初始化
{
  BCSCTL1 |=DIVA_2;                //辅助系统时钟不分频
  TA1CTL = TASSEL_1 + MC_1;        // AMCLK, up mode
  TA1CCR0 =163;        // PWM Period T=32768/4/164=50Hz 即20ms左右（舵机）
  TA1CCTL2 = OUTMOD_7;             //OUTMOD_7
  TA1CCR2 = 7;                     // 占空比D=7/163=0.42ms
}
void init_devices(void)
{
   port_init();                     //IO口设置
   int_Clock();                     //时钟设置
   int_WDT();                       //看门狗设置
   int_Time_A();
}
void main(void)
{
   init_devices(); 
   Ht1621_Init();                   //上电初始化LCD
   Delay_ms(5);                     //延时一段时间
   Ht1621WrAllData(0,Ht1621Tab_0,16); //清屏
   nRF24L01_Init();                 //NRF24L01初始化
//   _EINT();                        //开总中断
   while(1)
   {
     nRF24L01_Set_RX_Mode();
     Delay_ms(5);
      if(nRF24L01_RX_Data())          //接收数据
       {
          R1=RX_Buffer[0]%10;          //
          R2=RX_Buffer[0]/10;
          LCD_Display();  
          TA1CCR2=RX_Buffer[0];
        } 

   }
}