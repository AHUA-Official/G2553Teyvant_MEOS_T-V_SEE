

#include <msp430g2553.h>


#define uchar unsigned char 
#define uint  unsigned int

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

extern uchar  Ht1621Tab[],SEG_7[10][2],SEG_8[10][2];

void Delay_us(uint us);
void Delay_ms(uint ms);
void port_init(void); 
void Ht1621Wr_Data(uchar Data,uchar cnt);
void Ht1621WrCmd(uchar Cmd);
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt);
void Ht1621_Init(void);
void LCD_Display(void);
void stop_lcd(void);