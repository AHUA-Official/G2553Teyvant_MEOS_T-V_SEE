
//延时函数，IAR自带，经常使用到
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long


//4个独立按键连接在P10~P13
#define KeyPort         P1IN                              //独立键盘接在P10~P13


//彩屏/12864液晶/1602液晶的数据口，三液晶共用
#define DataDIR         P2DIR                     //数据口方向
#define DataPort        P2OUT                     //P4口为数据口

//12864/1602液晶控制管脚
#define RS_CLR	        P6OUT &= ~BIT3           //RS置低
#define RS_SET	        P6OUT |=  BIT3           //RS置高

#define RW_CLR	        P6OUT &= ~BIT4           //RW置低
#define RW_SET	        P6OUT |=  BIT4           //RW置高

#define EN_CLR	        P6OUT &= ~BIT5          //E置低
#define EN_SET	        P6OUT |=  BIT5           //E置高

#define PSB_CLR	        P6OUT &= ~BIT6            //PSB置低，串口方式
#define PSB_SET	        P6OUT |=  BIT6            //PSB置高，并口方式

//#define RST_CLR	        P6OUT &= ~BIT7            //RST置低
//#define RST_SET	        P6OUT |= BIT7             //RST置高

//12864应用指令集
#define CLEAR_SCREEN	0x01		          //清屏指令：清屏且AC值为00H
#define AC_INIT		0x02		          //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD	0x06		          //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE	0x30		          //工作模式：8位基本指令集
#define DISPLAY_ON	0x0f		          //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF	0x08		          //显示关
#define CURSE_DIR	0x14		          //游标向右移动:AC=AC+1
#define SET_CG_AC	0x40		          //设置AC，范围为：00H~3FH
#define SET_DD_AC	0x80                      //设置DDRAM AC
#define FUN_MODEK	0x36		          //工作模式：8位扩展指令集

//***********************************************************************
//                   系统时钟初始化，外部8M晶振
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
}

//***********************************************************************
//                   系统时钟初始化，内部RC晶振
//***********************************************************************
void Clock_Init_Inc()
{
  //uchar i;
  
 // DCOCTL = DCO0 + DCO1 + DCO2;              // Max DCO
 // BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
  
  DCOCTL = 0x60 + 0x00;                       //DCO约3MHZ，3030KHZ
  BCSCTL1 = DIVA_0 + 0x07;
  BCSCTL2 = SELM_2 + DIVM_0 + SELS + DIVS_0;
}

//***********************************************************************
//                   系统时钟初始化，外部32.768K晶振
//***********************************************************************
void Clock_Init_Ex32768()
{
  uchar i;

  BCSCTL2|=SELM1 + SELM0 + SELS;    //MCLK为32.768KHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
}

//***********************************************************************
//               MSP430内部看门狗初始化
//***********************************************************************
void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
}

