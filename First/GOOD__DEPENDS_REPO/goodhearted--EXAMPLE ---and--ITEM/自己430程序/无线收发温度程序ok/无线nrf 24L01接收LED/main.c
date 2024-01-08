#include  <msp430g2553.h>
#define uint unsigned int 
#define uchar unsigned char

//以下是74HC595所需的宏定义
#define HC595_Store_H P2OUT |= BIT0
#define HC595_Store_L P2OUT &= ~ BIT0
#define HC595_A_H     P2OUT |= BIT1
#define HC595_A_L     P2OUT &= ~ BIT1
#define HC595_Shift_H P2OUT |= BIT2
#define HC595_Shift_L P2OUT &= ~ BIT2
//以上是74HC595所需的宏定义

unsigned char  LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                                0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71,//共阴数码管段选数组(0-F）
                                0xBf,0x86,0xDb,0xCf,0xE6,0xEd,0xFd,0x87,    
                                0xff,0xef,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,};//共阴数码管(带小数点)段选数组(0-F）
unsigned char  LedPos[] = {0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}; //数码管位选数组，分别选通每一位数码管

void HC595shift(void);

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

#define IRQ    BIT5          //input 
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

uchar sta;                // 状态变量
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志

uchar  TX_Addr[]={0x34,0x43,0x10,0x10,0x01};
uchar  TX_Buffer[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar RX_Buffer[RX_DATA_WITDH];


void port_init(void)               //端口初始化
{
  P1DIR |= CE+CSN+SCK+IRQ+MOSI+LED;
  P1DIR &= ~MISO;
  P2DIR |= (BIT0+BIT1+BIT2);
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
  __delay_cycles(8000); 
}

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

void init_devices(void)
{
   port_init();
   int_Clock();
   int_WDT()  ;     //watchdog
}
void main(void)
{
   //uchar i;
   init_devices();
   Delay_ms(5);
   nRF24L01_Init();//NRF24L01初始化
   
   while(1)
 {
  nRF24L01_Set_RX_Mode();
  Delay_ms(1);
  if(nRF24L01_RX_Data())
  {
    LED_H;                       //如果有数据收到灯亮
    //Delay_ms(10);
//    if(RX_Buffer[0]==0x3f)
    display(0,RX_Buffer[0]);
    Delay_ms(2);
    display(1,RX_Buffer[1]);
    Delay_ms(2);
    display(2,RX_Buffer[2]);
    Delay_ms(2);
    display(3,RX_Buffer[3]);
    Delay_ms(2);
    display(4,RX_Buffer[4]);
    Delay_ms(2);
    display(5,RX_Buffer[5]);
    Delay_ms(2);
    display(6,RX_Buffer[6]);
    Delay_ms(2);
    display(7,RX_Buffer[7]);
    Delay_ms(1);  
  }
  else                          //否则灯熄
   LED_L;
 }   
}


