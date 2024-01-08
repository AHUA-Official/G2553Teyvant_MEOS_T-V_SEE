
#include <msp430x14x.h>

#define keyin    (P1IN & 0x0f)
#define uchar unsigned char
#define uint  unsigned int

void delay(void);


void main( void )
{
  
  uchar temp;
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  //P1DIR = ~(BIT0+BIT1+BIT2+BIT3);               //设置P1.0~P.3为输入状态，P.7为输出
  P2DIR = (BIT0+BIT1+BIT2+BIT3);
  P2OUT = (BIT0+BIT1+BIT2+BIT3);
  
  
  while(1)
  {
    
      if(keyin != 0x0f)       //如果有键被按下
        {
            delay();            //延时消抖
            if(keyin != 0x0f)   //再次检测按键状态
            {
                temp=keyin;
                while(keyin != 0x0f);   //等待按键被放开
          /*    switch(temp)    //转换键值    
                {
                case 0x0e:
                            keyval = 1;break;
                case 0x0d:
                            keyval = 2;break;
                case 0x0b:
                            keyval = 3;break;
                case 0x07:
                            keyval = 4;break;
                default:
                            keyval = 0;break;
                }*/
                P2OUT = temp;
                
                
            }
        } 
    
    
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
