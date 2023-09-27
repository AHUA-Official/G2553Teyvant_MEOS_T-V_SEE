

/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），
 *所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"
#include"HT1621.h"
#include"LCD_128.h"

#define myMCLK	16000000


void main(void)
{
	unsigned char i=0;
	WDTCTL = WDTPW + WDTHOLD;	//关狗
	P1DIR = 0;
	P2DIR = 0;

        BCSCTL1 = CALBC1_16MHZ; /* Set DCO to16MHz */
        DCOCTL = CALDCO_16MHZ;
        TCA6416A_Init();	// 初始化IO扩展口
       //----关闭LED灯----
    	PinOUT(0,1);           // 指定1号管脚输出为1
    	PinOUT(2,1);           // 指定0号管脚输出为1
    	PinOUT(3,1);           // 指定1号管脚输出为1
    	PinOUT(4,1);           // 指定0号管脚输出为1
    	PinOUT(5,1);           // 指定1号管脚输出为1
    	PinOUT(6,1);           // 指定0号管脚输出为1
    	PinOUT(7,1);           // 指定1号管脚输出为1
    	HT1621_init();	// 初始化lcd_128
        LCD_Clear();    // LCD128清屏
	while(1)
	{
		//----从0段至127段依次点亮------
		for(i=0;i<=127;i++)
		{
			LCD_DisplaySeg(i);   //点亮一段
			HT1621_Reflash(LCD_Buffer);
                        __delay_cycles(myMCLK);	
		}
		// __delay_cycles(myMCLK);		// 适当延时约为1s
		//----从127段至0段依次熄灭、、先点亮后删除
		for(i=128;i>0;i--)
		{
			LCD_ClearSeg(i-1);    //删除一段
			HT1621_Reflash(LCD_Buffer);
                        __delay_cycles(myMCLK);	
		}
		// __delay_cycles(myMCLK);		// 适当延时约为1s
		//-----所有数码段显示一致，从0切换到9----
		/*8for(i=0;i<=9;i++)
		{
			for(j=1;j<=10;j++)
			{
				LCD_DisplayDigit(i,j);
			}
			HT1621_Reflash(LCD_Buffer);
			 __delay_cycles(myMCLK/2);		// 适当延时约为1/2s
		}*/
		
		//-----大数码段显示-12456----
		/*LCD_DisplayNum(-123456);
		  HT1621_Reflash(LCD_Buffer);
		  __delay_cycles(myMCLK);*/			// 适当延时约为1s*/
		
                //----数码管逐个点亮
                for(i=0;i<10;i++)
                {
		  LCD_DisplayDigit(i,i+1);  //数码管显示数字
                  HT1621_Reflash(LCD_Buffer);
                  __delay_cycles(myMCLK);
                }
		
                
		_BIS_SR(LPM3_bits);	//自检完毕，休眠
	}
}


