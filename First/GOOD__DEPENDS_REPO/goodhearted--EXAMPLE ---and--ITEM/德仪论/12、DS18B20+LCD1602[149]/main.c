
#include <msp430x14x.h>
//#include  "cry1602.h"
#include  "cry1602.c"
//#include  "DS18B20.h"
#include  "DS18B20.c"


//要显示的6位温度数字
uchar temp[6]; 
void Clock_Init();
void Disp_Numb(uint temper);

void main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;           //关狗
     /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    
    Clock_Init();
    //计数时钟选择SMLK=8MHz，1/8分频后为1MHz
    TACTL |= TASSEL_2 + ID_3; 
    //打开全局中断
    _EINT();
        
    
    LcdReset();
    DispStr(0,0,"Temperature is:"); 
    Disp1Char(8,1,0X27);
    Disp1Char(9,1,'C');
  
     while(1)
     {
       Disp_Numb(Do1Convert());
       Disp1Char(1,1,temp[5]+0x30);
       Disp1Char(2,1,temp[4]+0x30);
       Disp1Char(3,1,0x2e);         //0x2e是小数点对应的ASCII码值
       Disp1Char(4,1,temp[3]+0x30);
       Disp1Char(5,1,temp[2]+0x30);
       Disp1Char(6,1,temp[1]+0x30);
       Disp1Char(7,1,temp[0]+0x30);         
     }
}


/*******************************************
函数名称：Disp_Numb
功    能：将从DS18B20读取的11bit温度数据转换
          成液晶显示的温度数字
参    数：temper--11bit温度数据
返回值  ：无
********************************************/
void Disp_Numb(uint temper)
{
    uchar i;
    
    for(i = 0;i < 6;i++) 
      temp[i] = 0; //初始化显示变量

    //数值转换
    if(temper & BIT0) 
    {
        temp[0] = 5;
        temp[1] = 2;
        temp[2] = 6;
    }
    if(temper&BIT1)     
    {
        temp[1] += 5;
        temp[2] += 2;
        temp[3] += 1;
    }
    if(temper & BIT2)     
    {
        temp[2] += 5;
        temp[3] += 2;
        if(temp[2] >= 10)
        {
            temp[2] -= 10;
            temp[3] += 1;
        }
    }
    if(temper&BIT3)     
    {
        temp[3] += 5;
    }
    if(temper & BIT4)
    {
        temp[4] += 1;
    }
    if(temper & BIT5)     
    {
        temp[4] += 2;
    }
    if(temper & BIT6)
    {
        temp[4] += 4;
    }
    if(temper & BIT7)     
    {
        temp[4] += 8;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BIT8)
    {
        temp[4] += 6;
        temp[5] += 1;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BIT9)
    {
        temp[4] += 2;
        temp[5] += 3;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
    }
    if(temper & BITA)
    {
        temp[4] += 4;
        temp[5] += 6;
        if(temp[4] >= 10)
        {
            temp[4] -= 10;
            temp[5] += 1;
        }
        if(temp[5] >= 10)
        {
            temp[5] -= 10;
        }
    }
}

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
