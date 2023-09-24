
/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），
 *所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"
#include"HT1621.h"
#include"LCD_128.h"

#define myMCLK	16000000
void LCD_Display_Pass();
void main(void)
{
	unsigned char i=0,j=0;
	WDTCTL = WDTPW + WDTHOLD;	//关狗
	P1DIR = 0;
	P2DIR = 0;

        BCSCTL1 = CALBC1_16MHZ; /* Set DCO to16MHz */
        DCOCTL = CALDCO_16MHZ;
        TCA6416A_Init();	// 初始化IO扩展口
       //----提示初始化成功----
    	PinOUT(0,0);           // 指定0号管脚输出为0
    	PinOUT(1,0);           // 指定1号管脚输出为0
    	PinOUT(2,0);           // 指定0号管脚输出为0
    	PinOUT(3,0);           // 指定1号管脚输出为0
    	PinOUT(4,0);           // 指定0号管脚输出为0
    	PinOUT(5,0);           // 指定1号管脚输出为0
    	PinOUT(6,0);           // 指定0号管脚输出为0
    	PinOUT(7,0);           // 指定1号管脚输出为0
    	HT1621_init();			// 初始化lcd_128

	while(1)
	{
		//----从0段至127段依次点亮------
		for(i=0;i<=127;i++)
		{
			LCD_DisplaySeg(i);
			HT1621_Reflash(LCD_Buffer);
		}
		 __delay_cycles(myMCLK);		// 适当延时约为1s
		 //----从127段至0段依次熄灭
		for(i=128;i>0;i--)
		{
			LCD_ClearSeg(i-1);
			HT1621_Reflash(LCD_Buffer);
		}
		 __delay_cycles(myMCLK);		// 适当延时约为1s
		//-----所有数码段显示一致，从0切换到9----

		for(i=0;i<=9;i++)
		{
			for(j=1;j<=10;j++)
			{
				LCD_DisplayDigit(i,j);
			}
			HT1621_Reflash(LCD_Buffer);
			 __delay_cycles(myMCLK/2);		// 适当延时约为1/2s
		}
		LCD_Clear();
		//-----大数码段显示-12456----
		LCD_DisplayNum(-123456);
		HT1621_Reflash(LCD_Buffer);
		 __delay_cycles(myMCLK);			// 适当延时约为1s
		LCD_Clear();
		//----测试结束，显示PASS----
		LCD_Display_Pass();
		 __delay_cycles(2*myMCLK);		// 适当延时约为2s
		 _BIS_SR(LPM3_bits);	//自检完毕，休眠
	}
}

/******************************************************************************************************
 * 名    称：LCD_Display_Pass()
 * 功    能：显示PASS字母的函数
 * 入口参数：无
 * 出口参数：无
 * 说    明：使用先显示数字，后删除特定段的方法实现字母显示
 * 范    例：无
 ******************************************************************************************************/
void LCD_Display_Pass()
{
	LCD_DisplayNum(8855);
	LCD_ClearSeg(_LCD_3C);
	LCD_ClearSeg(_LCD_3D);
	LCD_ClearSeg(_LCD_4D);
	//----显示logo----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	HT1621_Reflash(LCD_Buffer);
}

