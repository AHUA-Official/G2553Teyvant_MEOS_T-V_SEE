
#include <msp430g2553.h>
#include "HT1621.H"
#include "ADC10.h"



void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
}
void int_WDT(void)                 //看门狗设置
{
   WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog
}

void init_devices(void)
{
   port_init();                     //IO口设置
   int_Clock();                     //时钟设置
   int_WDT();                       //看门狗设置
}

/******************************************************** 
函数名称：main() 
功能描述: 主程序
********************************************************/
void main() 
{ 
  
  init_devices( );                       // 初始化配置
  adc10_init( );
  Ht1621_Init( );                        //上电初始化LCD 
                                        //延时一段时间 
  Ht1621WrAllData(0,Ht1621Tab,16);      //清除1621寄存器数据，暨清屏
  _BIS_SR(GIE);
  while(1)
  {
     display_adc10( );
  }
      
} 


