
#include <msp430g2553.h>


#define keyin    (P1IN & 0x0f)
#define uchar unsigned char
#define uint  unsigned int


uchar KeyVal = 0;   // 按键的键值

void delay(void);

/********************主函数********************/
void main( void )
{   
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    
    P1IES = 0x0f;               // P1.0~P1.3选择下降沿中断
    P1IE = 0x0f;                // 打开中断使能
    P2DIR = (BIT0+BIT1+BIT2+BIT3);
    P2OUT = (BIT0+BIT1+BIT2+BIT3);
    
    _EINT();                    //打开全局中断控制位
    while(1)
    {
        LPM3;
        P2OUT = KeyVal;
    }
}
/*******************************************
函数名称：delay
功    能：用于消抖的延时
参    数：无
返回值  ：无
********************************************/
void delay(void)
{
    uint tmp;
    
    for(tmp = 12000;tmp > 0;tmp--);
}
/*******************************************
函数名称：PORT1_ISR 
功    能：P1端口的中断服务函数
参    数：无
返回值  ：无
********************************************/
#pragma vector=PORT1_VECTOR
__interrupt void  PORT1_ISR(void)
{
    if(P1IFG & 0x0f)
    {
        switch(P1IFG)
        {
         case 0x01:
                    if(keyin == 0x0e)   //如果是第一个按键被按下
                    {
                        delay();
                        if(keyin == 0x0e)
                        {
                            //while(keyin != 0x0f);       //等待按键放开
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x02:
                    if(keyin == 0x0d)   //如果是第二个按键被按下
                    {
                        delay();
                        if(keyin == 0x0d)
                        {
                            //while(keyin != 0x0f);       //等待按键放开
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x04:
                    if(keyin == 0x0b)   //如果是第三个按键被按下
                    {
                        delay();
                        if(keyin == 0x0b)
                        {
                            //while(keyin != 0x0f);       //等待按键放开
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         case 0x08:
                    if(keyin == 0x07)   //如果是第四个按键被按下
                    {
                        delay();
                        if(keyin == 0x07)
                        {
                            //while(keyin != 0x0f);       //等待按键放开
                            KeyVal = keyin;
                            LPM3_EXIT;
                            P1IFG = 0;
                            return;
                        }
                    }
         default:
                    //while(keyin != 0x0f);       //等待按键放开
                    //KeyVal = 0;
                    //LPM1_EXIT;
                    P1IFG = 0;
                    return;
        }
    }
}
