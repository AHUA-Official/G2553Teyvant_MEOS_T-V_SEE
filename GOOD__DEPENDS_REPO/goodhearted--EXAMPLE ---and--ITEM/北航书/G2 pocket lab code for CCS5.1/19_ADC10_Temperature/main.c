/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */#include <msp430.h>
#include "LCD_128.h"
#include "HT1621.h"
#include "TCA6416A.h"

long temp;
long IntDeg;
void ADC10_ISR(void);
void ADC10_init(void);
void LCD_Init();
void LCD_Display();

void main()
{
	WDTCTL=WDTPW+WDTHOLD;
	ADC10_init();
	LCD_Init();
    while(1)
    {
      ADC10CTL0 |= ENC + ADC10SC;            // Sampling and conversion start
 //     _bis_SR_register(CPUOFF + GIE);       	 // LPM0 with interrupts enabled
      __delay_cycles(1000);
      //-----ADC转换完成中断唤醒CPU后才执行以下代码-----
      temp = ADC10MEM;							//读取AD采样值
      IntDeg= temp*4225/1024 - 2777;		//转换为摄氏度，并10倍处理
 //  IntDeg=-123;									//由于难以获得负温，直接给负值以测试LCD显示
      LCD_Display();										//调用LCD显示函数
    }
}
/******************************************************************************************************
 * 名       称：LCD_Init()
 * 功       能：初始化LCD显示相关的硬件，并预设固定不变的显示内容
 * 入口参数：无
 * 出口参数：无
 * 说       明：预设显示内容包括摄氏度oC，以及小数点。
 * 范       例：无
 ******************************************************************************************************/
void LCD_Init()
{
    TCA6416A_Init();
    HT1621_init();
	//-----显示固定不变的LCD段-----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	LCD_DisplaySeg(_LCD_DOT2);		//温度小数点
	//-----减法构造“o”-----
	LCD_DisplayDigit(9,5);
	LCD_ClearSeg(_LCD_5D);
	LCD_ClearSeg(_LCD_5C);
	//-----减法构造“C”-----
	LCD_DisplayDigit(0,6);
	LCD_ClearSeg(_LCD_6B);
	LCD_ClearSeg(_LCD_6C);
}
/******************************************************************************************************
 * 名       称：LCD_Displaly()
 * 功       能：将温度值显示出来
 * 入口参数：无
 * 出口参数：无
 * 说       明：包括对负温度的处理、拆分数字等几部分
 * 范       例：无
 ******************************************************************************************************/
void LCD_Display()
{
	 if( IntDeg>=0)	  LCD_ClearSeg(_LCD_1G);		//正温度，则清除负号
	      else
	      {
	    	  IntDeg=-IntDeg;						//负温度，则做绝对值处理
	    	  LCD_DisplaySeg(_LCD_1G);		//负温度，添加负号
	      }
	      //-----清除3位显示数字-----
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,2);
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,3);
	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
	      //-----拆分3位并显示数字-----
	      LCD_DisplayDigit(IntDeg/100,2);
	      LCD_DisplayDigit((IntDeg%100)/10,3);
	      LCD_DisplayDigit((IntDeg%100)%10,4);
	      //-----更新缓存，真正显示-----
	      HT1621_Reflash(LCD_Buffer);
}
/******************************************************************************************************
 * 名       称：ADC10_init(void)
 * 功       能：初始化ADC10采集内部温度传感器，单次手动采样
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接从CCS的Example中移植过来
 * 范       例：无
 ******************************************************************************************************/
void ADC10_init(void)
{
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10;
      __delay_cycles(30000);
    ADC10CTL0 |= ENC;
}
/******************************************************************************************************
 * 名       称：ADC10_ISR_HOOK
 * 功       能：ADC转换完成后唤醒CPU
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接从CCS的Example中移植过来
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	_bic_SR_register_on_exit(LPM4_bits);
}
