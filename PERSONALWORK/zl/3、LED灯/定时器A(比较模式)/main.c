#include  <msp430x14x.h>

void main(void)
{
  /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
  
  WDTCTL = WDTPW + WDTHOLD;                 // 关狗
  //P6DIR |= BIT2;P6OUT |= BIT2;              //关闭电平转换
  P2DIR = 0xff;                             // P2端口设置为输出
  P2OUT = 0xff;                             // 关闭其他LED
  P2SEL |= BIT2 + BIT3 + BIT4;                     // P2.3和P2.4连接内部模块
  CCTL0 = OUTMOD_7;
  CCR0 = 4096-1;                            // PWM周期为1S
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = 3072;                              // CCR1 PWM duty cycle
  CCTL2 = OUTMOD_7;                         // CCR2 reset/set
  CCR2 = 1024;                              // CCR2 PWM duty cycle
  TACTL = TASSEL_1 + ID_3 + MC_1;           // ACLK/8, up mode

  _BIS_SR(LPM3_bits);                       // Enter LPM3
}

/*zhongduan
#include  <msp430x14x.h>

void main(void)
{  
    
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
//    P3DIR = 0XFF;P3OUT = 0XFF;
//    P4DIR = 0XFF;P4OUT = 0XFF;
//    P5DIR = 0XFF;P5OUT = 0XFF;
//    P6DIR = 0XFF;P6OUT = 0XFF;
  
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    
//    P6DIR |= BIT2;P6OUT |= BIT2;    //关闭电平转换
    CCTL0 = CCIE;                   //使能CCR0中断
    TA0CCTL0;                    //设定周期0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //定时器A的时钟源选择ACLK，增计数模式
    P1DIR = 0xff;                   //设置P2口方向为输出
    P1OUT = 0xff;
 
    _EINT();                        //使能全局中断
    LPM3;                           //CPU进入LPM3模式
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{  
    P1OUT ^= 0xff;                  //P2口输出取反
}*/