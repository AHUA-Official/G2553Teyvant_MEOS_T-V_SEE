/*===================DCO自校验程序功能描述========================
 * 本程序可以利用32.768kHz低频晶振校验任意DCO频率。
 * 校验后的参数按顺序存入了InfoFLashB中。
 * 由于校验环境与实际应用环境一致，自校验的频率值比出厂校验值更精确。
 * 自校验参数的格式与出厂校验参数的格式一致。
 * 校验时Launchpad G2上的红灯会亮，校验完毕绿灯亮。
 * 校验完毕后，P1.4将会循环输出校验后的DCO频率
 * ==================ATTENTION======================
 * 确保所有CCS工程的Erase Options改为选择“Erase main memory only”，
 * 只有这样存在InfoFlashB的校验参数才不会因为烧录程序而丢失。InfoFlashA有特殊
 * 写保护，不会因为烧录丢失。 从技术角度，当然可以把自校验参数写入InfoFlashA段
 * 首32字节（从0x10C0至0x10E0），同时保留出厂校验参数（0x10F8至0x10FF)，这
 * 部分内容留待读者完全入门后，自行编写代码实现。
 **************************************************************************************************/
#include "MSP430G2553.h"
#include "Flash.h"
#define       CALDCO_MHz(x)			* (unsigned char *)(0x1080+(x-1)*2)			//读取InfoFlashB
#define       CALBC1_MHz(x)				* (unsigned char *)(0x1080+(x-1)*2+1)
#define 		CAL_NUM 		16										/*待校验频率点总数*/
unsigned int Temp[128]={0};										/*存放实测的128个不含小数分频MODx的“纯净”频率*/
unsigned char RSELx_DCOx_Winer[CAL_NUM]={0};	/*存放校验出的SELx和DCOx值*/
unsigned char MODx_Winer[CAL_NUM]={0};				/*存放校验出的MODx值*/
unsigned int 	InfoFlash_Data[CAL_NUM]={0};				/*待写入infoB中的校验频率参数数组*/
unsigned int Ture_Freq[CAL_NUM]={0};						/*方便仿真时查看实测频率值*/

		// -----待校正的频率，单位KHz-----
const unsigned int Calibrate_Frequence[CAL_NUM]={1000,2000,3000,4000,5000,6000,
		                                 7000,8000,9000,10000,11000,12000,13000,14000,15000,16000};

void DCO_Set_Freq(unsigned char Num,unsigned char mod_Temp);
void Measure_Temp();
void Find_RSELx_DCOx_Winer();
void Measure_MODx();
void Calculate_InfoFlash_Data();
void Write_InfoFlashB();
void DCO_InfoFlashB(unsigned int freq);
void Calibrate_Start();
void Calibrate_Finish();
void DCOTest();

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;				//关狗
	//-----在P1.4上输出SMCLK，这样可以全程用示波器和频率计观测DCO----
	P1SEL |= BIT4;
	P1DIR |= BIT4;
	//-----初始化TA，开启WDT中断使能-----
	TA0CTL = TASSEL_2+MC_2+TACLR;          	// SMCLK时钟源，增计数开始
	IE1|=WDTIE;													//开启WDT中断
	 _enable_interrupts();									//等同_ENT，开启总中断
	//-----开始DCO频率校验，只运行一遍-----
	Calibrate_Start();						//步骤1：红灯亮，表示正在校验DCO
	Measure_Temp();					//步骤2：自动测量128个纯净频率
	Find_RSELx_DCOx_Winer();	//步骤3：从128个纯净频率中，找出最接近预设频率的那几个
	Measure_MODx();					//步骤4：在最近接的纯净频率基础上，穷举找到最优的MODx设置
	Calculate_InfoFlash_Data();	//步骤5：将最合适的频率设定参数组合成校验数组
	Write_InfoFlashB();					//步骤6：将校验数组写入InfoFlahB段
	Calibrate_Finish();					//步骤7：绿灯亮，表示完成校验DCO
	//-----完成DCO校验，从Flash中定时取出校准值，从P1.4输出供测试-----
	DCOTest();								//步骤8：循环输出DCO各个校验频率值，供观测
}
/******************************************************************************************************
 * 名       称：Calibrate_Start()
 * 功       能：红色LED亮，表明正在校验中
 * 入口参数：无
 * 出口参数：无
 * 说       明：DCO校验的时间比较长，利用红灯指示可以方便调试程序
 * 范       例：无
 ******************************************************************************************************/
void Calibrate_Start()		//步骤1：红灯亮，表示正在校验DCO
{
	P1DIR |=BIT0+BIT6;
	P1OUT |= BIT0;				//红亮绿灭表示正在校验
	P1OUT &=~BIT6;
}
/******************************************************************************************************
 * 名       称：Measure_Temp()
 * 功       能：自动依次测定128种纯净DCO的频率，并存入Temp[]数组
 * 入口参数：无
 * 出口参数：无
 * 说       明：纯净频率是指MOD=0时的非混频
 * 范       例：无
 ******************************************************************************************************/
void Measure_Temp()			//步骤2：自动测量128个纯净频率
{
	unsigned long Freq_Temp=0;
	unsigned char RSELx_DCOx_Num=0;
	for(RSELx_DCOx_Num=0;RSELx_DCOx_Num<128;RSELx_DCOx_Num++)
	{
		DCO_Set_Freq(RSELx_DCOx_Num,0);
		//-----定时测频代码开始-----
		TA0CTL |=TACLR;							//计数开始
		WDTCTL=WDT_ADLY_1_9;				//WDT定时长度为精心选择过的
		 _bis_SR_register(LPM0_bits);		//CPU精确休眠1.9ms(1/512秒)
		WDTCTL=WDTPW+WDTHOLD;
		Freq_Temp = TA0R;						//读取计数，这个值就是DCO在1/512秒中断区间的脉冲数
		//-----定时测频代码结束-----
		Temp[RSELx_DCOx_Num]=((Freq_Temp)*512)/1000;		//计算DCO准确频率
	}
}
/******************************************************************************************************
 * 名       称：Find_RSELx_DCOx_Winer()
 * 功       能：从128个Temp[]中，找出与16个校验频率最接近的Temp的序号
 * 入口参数：无
 * 出口参数：无
 * 说       明：Temp的序号里包含了RSELx和DCOx的取值信息
 * 范       例：无
 ******************************************************************************************************/
void Find_RSELx_DCOx_Winer()		//步骤3：从128个纯净频率中，找出最接近预设频率的那几个
{
	unsigned char i=0;
	unsigned int Delta_Min=0;
	unsigned int Delta_Now=0;
	unsigned char Calibrate_Num=0;
	for(Calibrate_Num=0;Calibrate_Num<CAL_NUM;Calibrate_Num++)
	{
		Delta_Min=65530;														//第一次比较时，故意定一个大差值
		for(i=0;i<128;i++)
		{
			//-----单方向取最小差值，一定要小于预设值-----
			if(Temp[i]<Calibrate_Frequence[Calibrate_Num])
				{
					Delta_Now=Calibrate_Frequence[Calibrate_Num]-Temp[i];
					if (Delta_Now<Delta_Min)							//如果当前差值比“世界记录”还小时
						{
						RSELx_DCOx_Winer[Calibrate_Num]=i;		//新Winner诞生
						Delta_Min=Delta_Now;								//取代前“世界记录”
						}
				}
		}
	}
}
/******************************************************************************************************
 * 名       称：Measure_MODx()
 * 功       能：基于已校验成功的RSELx和DCOx，进一步校验出最合适的MODx
 * 入口参数：无
 * 出口参数：无
 * 说       明：本函数将固定RSELx和DCOx，穷举32种MODx的取值，然后判断最优的MODx
 * 范       例：无
 ******************************************************************************************************/
void Measure_MODx()		//步骤4：在最近接的纯净频率基础上，穷举找到最优的MODx设置
{
	unsigned char mod_x = 0;
	unsigned long  Freq_Temp = 0;
	unsigned int Delta_Now=0;
	unsigned int Delta_Min=0;
    unsigned char Calibrate_Num=0;
	for(Calibrate_Num=0;Calibrate_Num<CAL_NUM;Calibrate_Num++)
	{
		Delta_Min=65530;											//第一次比较时，故意定一个大差值
		//-----RSELx和DCOx一定时，测量mod_x改变时的系列频率-----
		for(mod_x=0;mod_x <32;mod_x++)
		{
			DCO_Set_Freq(RSELx_DCOx_Winer[Calibrate_Num],mod_x);	//改变mod_x设置DCO
			//-----定时测频代码开始-----
			TA0CTL |=TACLR;
			WDTCTL=WDT_ADLY_1_9;
			 _bis_SR_register(LPM0_bits );					//CPU精确休眠1.9ms(1/512秒)
			WDTCTL=WDTPW+WDTHOLD;
			Freq_Temp = TA0R;									//读取"测频"值
			//-----定时测频代码结束-----
			Freq_Temp=((Freq_Temp)*512)/1000;		//计算DCO准确频率
			//-----与Find_RSELx_DCOx_Winer()不同，这里的要求是正负差值最小-----
			if(Freq_Temp<Calibrate_Frequence[Calibrate_Num])
			{
				Delta_Now=Calibrate_Frequence[Calibrate_Num]-(unsigned int )Freq_Temp;
			}
			else
			{
				Delta_Now=(unsigned int )Freq_Temp-Calibrate_Frequence[Calibrate_Num];
			}
			if (Delta_Now<Delta_Min)						//如果当前差值比“世界记录”还小时
			{
				MODx_Winer[Calibrate_Num]=mod_x;									//新Winner诞生
				Ture_Freq[Calibrate_Num]=(unsigned int )Freq_Temp;		//记录校验后的准确频率
				Delta_Min=Delta_Now;							//取代前“世界记录”
			}
		}
	}
}
/******************************************************************************************************
 * 名       称：Calculate_InfoFlash_Data()
 * 功       能：基于已校验成功的RSELx、DCOx和MODx封装待写入Flash的数据
 * 入口参数：无
 * 出口参数：无
 * 说       明：16位数据的格式完全仿照出厂校验参数，最高4位固定1000，然后依次为RSLEx、
 * 					 DCOx和MODx。
 * 范       例：无
 ******************************************************************************************************/
void Calculate_InfoFlash_Data()		//步骤5：将最合适的频率设定参数组合成校验数组
{
	unsigned char RSELx=0;
	unsigned char DCOx=0;
	unsigned char i=0;
	for(i=0;i<CAL_NUM;i++)
	{
		RSELx=RSELx_DCOx_Winer[i]/8;
		DCOx=RSELx_DCOx_Winer[i]%8;
		//-----出厂校准参数的16位格式为“1000_RSELx_DCOx_MODx"-----
		InfoFlash_Data[i]=0x8000+(RSELx<<8)+(DCOx<<5)+MODx_Winer[i];
	}
}
/******************************************************************************************************
 * 名       称：Write_InfoFlashB()
 * 功       能：将打包好的InfoFlash_Data[]数组，依次写入InfoFlashB段
 * 入口参数：无
 * 出口参数：无
 * 说       明：确保所有CCS工程的Erase Options改为选择“Erase main memory only”，
 * 					只有这样存在InfoFlashB的校验参数才不会因为烧录程序而丢失。
 * 范       例：无
 ******************************************************************************************************/
void Write_InfoFlashB()		//步骤6：将校验数组写入InfoFlahB段
{
	unsigned char i=0;
	//-----写Flash前DCO时钟一定要重新确认一遍-----
    BCSCTL1 = CALBC1_1MHZ;      										/* Set DCO to 8MHz */
    DCOCTL = CALDCO_1MHZ;
    Flash_Init(3,'B');																// 初始化Flash
	Flash_Erase();																	// 擦除Info_B
	//-----把InfoFlash_Data[CAL_NUM]存入InfoFlashB-----
	for(i=0;i<CAL_NUM;i++)
		Flash_Direct_WriteWord(i*2,InfoFlash_Data[i]);		//不擦直接写
}
/******************************************************************************************************
 * 名       称：Calibrate_Finish()
 * 功       能：绿色LED点亮表示DCO校验完成
 * 入口参数：无
 * 出口参数：无
 * 说       明：在程序运行中，一个恰当的LED指示往往能温暖人心
 * 范       例：无
 ******************************************************************************************************/
void Calibrate_Finish()			//步骤7：绿灯亮，表示完成校验DCO
{
	P1OUT |= BIT6;					//绿亮红灭表示结束校验
	P1OUT &=~BIT0;
}
/******************************************************************************************************
 * 名       称：DCOTest()
 * 功       能：间隔3秒循环读取InfoFlashB中的校验参数，并据此设定DCO频率
 * 入口参数：无
 * 出口参数：无
 * 说       明：可以用示波器和频率计接在P1.4（SMCLK输出口），实测DCO校验参数的准确性
 * 范       例：无
 ******************************************************************************************************/
void DCOTest()											//步骤8：循环输出DCO各个校验频率值，供观测
{
	unsigned char i=1,freqCnt=1;
	WDTCTL=WDT_ADLY_1000;					//WDT中断设为1s一次
	while(1)													//相当于是主循环了，重复切换DCO并输出
	{
		if(i++ >2)											//WTD中断唤醒三次(即定时3s),切换一次频率
		{
			i=1;
			if(freqCnt>CAL_NUM)					// 范围控制（1~CAL_NUM）
				freqCnt=1;
		//-----仿照出厂校验参数CALDCO_xMHz和CALBC1_xMHz的形式设置DCO频率-----
			DCOCTL=CALDCO_MHz(freqCnt);
			BCSCTL1=CALBC1_MHz(freqCnt);
			freqCnt++;
		}
		 _bis_SR_register(LPM0_bits);			//等待1s的WDT中断唤醒
	}
}
/******************************************************************************************************
 * 名       称：DCO_Set_Freq()
 * 功       能：根据传入参数，设定DCO频率
 * 入口参数：Num：Num的0~2位表示DCOx，3~6位表示RSELx
 * 					modTemp：MODx参数
 * 出口参数：无
 * 说       明：Num参数身兼二职，包含DCOx和RSELx的信息
 * 范       例：DCO_Set_Freq(10,5)，表示将DCO设定为RSELx=1，DCOx=2，MODx=5
 ******************************************************************************************************/
void DCO_Set_Freq(unsigned char Num,unsigned char modTemp) //用参数设定DCO频率
{
	DCOCTL = ((Num%8)<<5)+modTemp;	//数据拆分
	BCSCTL1 &=~0x0F;
	BCSCTL1 |=Num/8;
	__delay_cycles(15000);							//等待DCO频率稳定
}
/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：响应WDT定时中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：程序内部只有1句唤醒CPU代码，这种写法即用到了WDT的定时功能，
 * 					 同时避免将大量代码写入中断服务函数中，影响程序的可读性。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	__bic_SR_register_on_exit(LPM0_bits );		//退出中断时，唤醒CPU
}


