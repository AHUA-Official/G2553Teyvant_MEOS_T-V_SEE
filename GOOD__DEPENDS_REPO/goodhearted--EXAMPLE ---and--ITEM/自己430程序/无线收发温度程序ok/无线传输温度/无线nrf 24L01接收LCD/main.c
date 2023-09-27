/****************************************************
  本实验是利用无线传输nRF2401，可以测量不在相同地方的温度。
  
  现象是：当有数据正常传输时，LED闪烁，
          当有数据无法正常传输时，LED一直亮，同时LCD显示最后传输的温度。

  可以利用三极管当做开关，来控制nRF2401的通断，即：当MSP430睡眠时，
  用三极管关闭nRF2401，以降低功耗。
  
  当MSP430睡眠时，进入LPM4，可以降到0.4uA(HT1621B-0.3uA,MSP430-0.1uA)

特别注意：当进入睡眠时，一定先关闭LCD,再配置IO口
          要不然关不掉LCD


************************硬件连接*********************

所用到的元件：1.nRF24L01  2.三极管NPN9013 3.HT1621 4.四位半LCD
              5.两个按键

                    nRF 2401的连接
        MISO -------------------------------P1.0
        MOSI -------------------------------P1.1
        SCK  -------------------------------P1.2
        CE   -------------------------------P1.3
        CSN  -------------------------------P1.4
        IRQ  -------------------------------P1.5
        nRF 2401 + -------------------------VCC
        nRF 2401 - -------------------------nRF 2401的三极管9013-C(3)

              nRF 2401的三极管9013的连接
        nRF 2401的三极管9013(2)-------------P2.3 
        nRF 2401的三极管9013(1)-------------GND

                    HT1621的连接
        HT1621_DAT--------------------------P2.0
        HT1621_CS --------------------------P2.1
        HT1621_WR --------------------------P2.2
                 按键的连接
        睡眠sleep（LPM4）-------------------P2.4
        唤醒（LPM4_EXIT) -------------------P2.5 

******************************************************/



#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

#define nRF2401_9013    BIT3
#define nRF2401_9013_H  P2OUT |= BIT3
#define nRF2401_9013_L  P2OUT &= ~BIT3

volatile uchar sleep=0;

#define BIAS    0x52  //0b1000 0100 1010 1/3duty 3com
#define SYSDIS  0X00  //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN   0X02  //0b1000 0000 0010 打开系统振荡器
#define LCDOFF  0X04  //0b1000 0000 0100 关LCD偏压
#define LCDON   0X06  //0b1000 0000 0110 打开LCD偏压
#define TIMERDIS 0X08  //0b1000 0000 0110 禁止时基输出
#define XTAL    0x28  //0b1000 0010 1000 外部接时钟
#define RC256   0X30  //0b1000 0011 0000 内部时钟
#define TONEON  0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A  //0b1000 0000 1010 禁止看门狗
#define HT1621_DAT_H P2OUT|=BIT0
#define HT1621_DAT_L P2OUT&=~BIT0
#define HT1621_CS_H  P2OUT|=BIT1
#define HT1621_CS_L  P2OUT&=~BIT1
#define HT1621_WR_H  P2OUT|=BIT2
#define HT1621_WR_L  P2OUT&=~BIT2

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

#define TX_ADDR_WITDH 5//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 8//
#define RX_DATA_WITDH 8


//nRF24L01对应引脚

#define MISO   BIT0                //input
#define MISO_H P1OUT |= BIT0
#define MISO_L P1OUT &= ~BIT0

#define MOSI   BIT1
#define MOSI_H P1OUT |= BIT1
#define MOSI_L P1OUT &= ~BIT1

#define SCK    BIT2
#define SCK_H  P1OUT |= BIT2
#define SCK_L  P1OUT &= ~BIT2

#define CE     BIT3
#define CE_H   P1OUT |= BIT3
#define CE_L   P1OUT &= ~BIT3

#define CSN    BIT4
#define CSN_H  P1OUT |= BIT4
#define CSN_L  P1OUT &= ~BIT4

#define IRQ    BIT5          
#define IRQ_H  P1OUT |=BIT5 
#define IRQ_L  P1OUT &= ~BIT5

#define  LED   BIT6
#define  LED_H P1OUT |= BIT6;
#define  LED_L P1OUT &= ~BIT6;

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

uchar sta;                   // 状态变量
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志

uchar  TX_Addr[]={0x34,0x43,0x10,0x10,0x01};
uchar  TX_Buffer[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar RX_Buffer[RX_DATA_WITDH];


void port_init(void)                   //端口初始化
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI+LED;   //
  P1DIR &= ~MISO;                     //
  P2DIR |=BIT0+BIT1+BIT2+nRF2401_9013;
  nRF2401_9013_H;                     //
  
  P2OUT |=BIT4+BIT5;   //P2.4-5带上拉电阻
  P2IE  |=BIT4+BIT5;   //P2.4-5中断使能
  P2IES |=BIT4+BIT5;   //P2.4-5中断下降沿触发中断
  P2REN |=BIT4+BIT5;   //P2.4-5上拉电阻使能
  P2IFG &=~(BIT4+BIT5);//P2.4-5中断标志位清零
}
void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=1MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=1MHZ 
}
void int_WDT(void)                 //看门狗设置
{
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}
void Delay_us(uint us)           // 延时us  前提是f=8MHZ
{    
  uint i;
  for(i=0;i<us;i++)
  __delay_cycles(1);      
}
void Delay_ms(uint ms)           // 延时ms  前提是f=8MHZ
{
  uint j;
  for(j=0;j<ms;j++)
  __delay_cycles(8000); 
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
    Delay_ms(200);        //延时使LCD工作电压稳定
    Ht1621WrCmd(BIAS);
    Ht1621WrCmd(RC256);   //使用内部振荡器
    Ht1621WrCmd(SYSDIS);
    Ht1621WrCmd(WDTDIS);
    Ht1621WrCmd(SYSEN);
    Ht1621WrCmd(LCDON);
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
       
       Ht1621WrOneData(0x0d,Ht1621Tab2[3*shi]);
       Ht1621WrOneData(0x0f,Ht1621Tab2[3*shi+1]);
       Ht1621WrOneData(0x11,Ht1621Tab2[3*shi+2]);//调用 Ht1621Tab3 
                                                 //显示字母LOW BATTERY
       Ht1621WrOneData(0x13,Ht1621Tab1[3*ge]);   
       Ht1621WrOneData(0x15,Ht1621Tab1[3*ge+1]);
       Ht1621WrOneData(0x17,Ht1621Tab1[3*ge+2]); //调用 Ht1621Tab3  
                                                 //显示CONTINUITY
}

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
 for(i=0;i<8;i++)//一字节8位循环8次写入
 {
  if(byte&0x80)//如果数据最高位是1
   MOSI_H;//向NRF24L01写1
  else //否则写0
   MOSI_L;
  byte<<=1;//低一位移到最高位
  SCK_H;//SCK拉高，写入一位数据，同时读取一位数据
  if(P1IN & MISO)
   byte|=0x01;
  SCK_L;//SCK拉低
 }
 return byte;//返回读取一字节
}

//SPI写寄存器一字节函数
//reg:寄存器地址
//value:一字节（值）
uchar SPI_W_Reg(uchar reg,uchar value)
{
 uchar status;//返回状态
 CSN_L;//SPI片选
 status=SPI_RW(reg);//写入寄存器地址，同时读取状态
 SPI_RW(value);//写入一字节
 CSN_H;//
 return status;//返回状态
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

//设置发送模式
void nRF24L01_Set_RX_Mode(void)
{
 CE_L;                            //待机
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

void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;           //watchdog
}

void stop_IO(void)
{
  P1DIR  = 0xff;      
  P1OUT  = 0xff;       
  P2DIR |= 0xcf;          //1100 1111 除了按键，其余都为输出
  P2OUT |= 0xc7;          //1100 0111 除了按键，P2.3(三极管)必须为低,其余都输出高
  P2OUT &= ~nRF2401_9013; 
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
}

void main(void)
{
   //uchar i;
   init_devices();
   
   Ht1621_Init();                   //上电初始化LCD
   Delay_ms(1);                     //延时一段时间
   Ht1621WrAllData(0,Ht1621Tab,16);
   
   Delay_ms(5);
   nRF24L01_Init();                  //NRF24L01初始化
   _EINT();                          //开总中断
   while(1)
 {
  nRF24L01_Set_RX_Mode();
  Delay_ms(1);
  if(nRF24L01_RX_Data())
  {
    LED_L;                           //如果有数据收到灯熄
    ge=RX_Buffer[0];
    shi=RX_Buffer[1];
    bai=RX_Buffer[2];
    qian=RX_Buffer[3];
    display();
   // Delay_ms(50);
    //Ht1621WrAllData(0,Ht1621Tab,16);
  }
  else                               //否则灯亮
  LED_H;
  
  if(sleep)
  {
    stop_lcd();                 //特别注意：一定先关闭LCD
                                //          再配置IO口，要不然关不掉LCD
    stop_IO();  
    LPM4;
    
    init_devices();
    Ht1621_Init();               //上电初始化LCD
    sleep=0;
  }
 }   
}

//P2中断服务程序

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{ 
    Delay_ms(10);            //消抖10ms
    if((P2IN & BIT4)==0)
    {
      sleep=1;               //对睡眠标志位置位，准备休眠
    }
    if((P2IN & BIT5)==0)
    {
      sleep=0; 
      LPM4_EXIT;             //退出睡眠LPM4
    }
    P2IFG =0;                //P2中断标志位清零
}  
