

/*----------------------------------------------------
测试说明：观察液晶显示
****************************************************/
#include "msp430g2553.h"

#include  "Cry1602.h"


uchar *s1 = "welcome!";
void int_Clock(void);  

void main( void )
{
    
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    int_Clock();
    LcdReset();
    
    while(1)
    {
     DispStr(4,0,s1);

    }
}
void int_Clock(void)               //时钟初始化
{
  BCSCTL1 = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
  DCOCTL  = CALBC1_8MHZ;           //DCO数字振荡器=8MHZ
}