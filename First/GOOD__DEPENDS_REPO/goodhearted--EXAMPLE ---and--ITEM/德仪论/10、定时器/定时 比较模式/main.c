
#include  <msp430x14x.h>

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
  uchar j=0;
//***********************************************************************
//                   系统时钟初始化，外部8M晶振
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS+DIVM_3+DIVS_3;  //MCLK为8MHZ，SMCLK为8MHZ
  do{                               //8分频
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
/*void Clock_Init_Inc()
{
  uchar i;
  
 // DCOCTL = DCO0 + DCO1 + DCO2;              // Max DCO
 // BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
  
  DCOCTL = 0x60 + 0x00;                       //DCO约3MHZ，3030KHZ
  BCSCTL1 = DIVA_0 + 0x07;
  BCSCTL2 = SELM_2 + DIVM_0 + SELS + DIVS_0;
}


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
}*/


void main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  Clock_Init();
  P2DIR |= 0x01;                            // P1.0 output
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 50000;
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, contmode

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    
//    CCR0 += 50000;                            // Add Offset to CCR0
    j++;
    if(j==20)  //延时一秒
    {
     j = 0;
     P2OUT ^= 0x01;                            // Toggle P1.0
    } 
}

