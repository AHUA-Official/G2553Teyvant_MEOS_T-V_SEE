/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */
#include"MSP430G2553.h"
#include "Slope.h"
#include "TCA6416A.h"
#include "HT1621.h"
#include "LCD_128.h"

unsigned char Key=0;		//轮盘电位器的电阻等效按键值
//-----待测“传感器电阻”的分档值，实际接入了5k定值电阻+10k轮盘电位器-----
const unsigned char R_Class[8]={55,67,82,97,113,128,138,144};		//单位为百欧

void WDT_OnTime();
void Slope_R_Dect();			//测得待测电阻后的事件处理函数
void TCA6416A_LED(unsigned char num);		//LED灯柱显示函数
void Display_SLOPE();
void Display_RSENS(unsigned char Rsens);

void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1 = CALBC1_16MHZ;      /* Set DCO to16MHz */
    DCOCTL = CALDCO_16MHZ;
	__delay_cycles(100000);			//等待电压稳定
	TCA6416A_Init();
	Slope_Measure_Init();
	HT1621_init();
	LCD_Clear();
	Display_SLOPE();
	HT1621_Reflash(LCD_Buffer);
	//-----设定WDT为16ms定时中断-----
	WDTCTL=WDT_ADLY_16;
	//-----WDT中断使能-----
    IE1|=WDTIE;
	 _enable_interrupts();

	while(1);
}

/******************************************************************************************************
 * 名       称：WDT_ISR(void)
 * 功       能：WDT定时中断子函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：直接调用事件处理函数即可
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	 IE1&=~WDTIE;
	WDT_OnTime();
	 IE1|=WDTIE;
}
/******************************************************************************************************
 * 名       称：WDT_OnTime()
 * 功       能：WDT定时中断的事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：主要工作都在事件处理函数中体现
 * 范       例：无
 ******************************************************************************************************/
void WDT_OnTime()
{
	static unsigned char Charge_Ready=0;			//充满电标志位
	static unsigned char REF_Mreasure_Ready=0;	//等于8表明参考电阻已测完
	static unsigned long R_REF_Sum=0;			//暂存参考电阻的累加值
		if(Charge_Ready==0)		//未充满电
		{
			Slope_Port_Charge();	//充电
			Slope_R_Dect();			//执行显示LED的任务
			Charge_Ready=1;		//充满电标志
		}
		else
		{
			//-----测量8次参考电阻值-----
			if(REF_Mreasure_Ready<8)					// 判断是否继续测量参考电阻值
			{
				Slope_Measure_REF();						//测参考电阻值
				R_REF_Sum=R_REF_Sum+R_REF;		//累加电阻值
				if(REF_Mreasure_Ready==7)	R_REF=R_REF_Sum>>3;	//测量完毕求出参考电阻的平均值
				REF_Mreasure_Ready++;					//测量次数累加
			}
			//-----得到参考电阻值后，开始不停的测量传感器电阻值-----
			else
				Slope_Measure_SENS();					//测完参考电阻才测传感器电阻
				Charge_Ready=0;							//测完一次，又该充电了
		}
}

/******************************************************************************************************
 * 名       称：Slope_R_Dect()
 * 功       能：对传感器电阻判断档位，调用LED显示程序
 * 入口参数：无
 * 出口参数：无
 * 说       明：主要工作都在事件处理函数中体现
 * 范       例：无
 ******************************************************************************************************/
void Slope_R_Dect()
{
	static unsigned int Rsens=0;
	unsigned char Key_Last=0;		//用于判断是否数据有更新
	Key_Last=Key;
	//-----通过10k参考电阻，计算电阻绝对值，单位百欧-----
	Rsens=(((unsigned long int)100)*(unsigned long int)R_SENS)/R_REF;
	//-----传感器电阻分档判断，5k至15k之间，分8档-----
	if(Rsens<=R_Class[0]) 			Key=0;
	else if (Rsens<=R_Class[1]) 	Key=1;
	else if (Rsens<=R_Class[2])	Key=2;
	else if (Rsens<=R_Class[3]) 	Key=3;
	else if (Rsens<=R_Class[4]) 	Key=4;
	else if (Rsens<=R_Class[5]) 	Key=5;
	else if (Rsens<=R_Class[6]) 	Key=6;
	else 	if(Rsens<=R_Class[7]) 	Key=7;
	else											Key=8;
	//-----调用一系列显示任务-----
	Display_RSENS(Rsens);		//显示实际电阻值
	if(!(Key==Key_Last))			//显示电阻分档值
	{
		LCD_DisplayDigit(LCD_DIGIT_CLEAR ,6);
		LCD_DisplayDigit(Key,6);
		HT1621_Reflash_Digit(6);
	}
	TCA6416A_LED(Key);		 	//LED灯柱显示
}
/******************************************************************************************************
 * 名       称：TCA6416A_LED()
 * 功       能：根据传感器电阻值，实现LED灯柱效果
 * 入口参数：无
 * 出口参数：无
 * 说       明：需要调用I2C和TCA6416控制LED
 * 范       例：无
 ******************************************************************************************************/
void TCA6416A_LED(unsigned char num)
{
	unsigned char i = 0;
	for(i=0;i<8;i++)
	{
		if(i<num)					// 判断当前需要点亮几盏灯
			PinOUT(i,0);			//根据LED接法，IO低电平为点亮LED
		else
			PinOUT(i,1);			//根据LED接法，IO高电平为熄灭LED
	}
}
/******************************************************************************************************
 * 名       称：Display_SLOPE()
 * 功       能：在LED上显示固定不变的内容
 * 入口参数：无
 * 出口参数：无
 * 说       明：包括显示SLOPE，2个小数点，logo，kΩ单位。
 * 范       例：无
 ******************************************************************************************************/
void Display_SLOPE()
{
	//-----显示S-----
	LCD_DisplayDigit(5,1);
	//-----显示L-----
	LCD_DisplayDigit(0,2);
	LCD_ClearSeg(_LCD_2A);
	LCD_ClearSeg(_LCD_2B);
	LCD_ClearSeg(_LCD_2C);
	//-----显示O-----
	LCD_DisplayDigit(0,3);
	//-----显示P-----
	LCD_DisplayDigit(8,4);
	LCD_ClearSeg(_LCD_4C);
	LCD_ClearSeg(_LCD_4D);
	//-----显示E-----
	LCD_DisplayDigit(8,5);
	LCD_ClearSeg(_LCD_5B);
	LCD_ClearSeg(_LCD_5C);
	//-----显示小数点-----
	LCD_DisplaySeg(_LCD_DOT4);
	LCD_DisplaySeg(_LCD_DOT6);
	//-----显示logo-----
	LCD_DisplaySeg(_LCD_TI_logo);
	LCD_DisplaySeg(_LCD_QDU_logo);
	//-----显示kΩ-----
	LCD_DisplaySeg(_LCD_k_OHOM);
	LCD_DisplaySeg(_LCD_OHOM);
}
/******************************************************************************************************
 * 名       称：Display_RSENS()
 * 功       能：在LED的小8字段上显示实测传感器电阻值
 * 入口参数：无
 * 出口参数：无
 * 说       明：哪位内容有改变才更新该位的显存。
 * 范       例：无
 ******************************************************************************************************/
void Display_RSENS(unsigned char Rsens)
{
	unsigned char i=0;
	static unsigned char Digit[3]={0};
	unsigned char Digit_Past[3]={0};		//用于对比数值是否有改变
	for(i=0;i<3;i++){
		Digit_Past[i]=Digit[i];
	}
	//-----拆分数字-----
	Digit[2]=Rsens/100;
	Digit[1]=(Rsens%100)/10;
	Digit[0]=Rsens%10;
	//-----判别数位的改变，并更新该位显存-----
	for(i=0;i<3;i++)
	{
		if(!(Digit[i]==Digit_Past[i]))		//如果有变化
		{
			LCD_DisplayDigit(LCD_DIGIT_CLEAR ,i+8);	//清除该8字段
			LCD_DisplayDigit(Digit[i],i+8);						//写显存映射
			HT1621_Reflash_Digit(i+8);							//仅更新该8字段的显存
		}
	}
}

