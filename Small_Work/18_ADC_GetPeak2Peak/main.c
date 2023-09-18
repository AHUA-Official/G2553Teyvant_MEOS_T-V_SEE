#include <msp430.h> 
#include "stdint.h"

uint16_t adcbuff[50] = {0};
uint16_t maxval[50] = {0};
uint16_t minval[50] = {0};
uint16_t max = 0;
uint16_t min = 0;
uint16_t vpp = 0;

/*
 * @fn:		void InitSystemClock(void)
 * @brief:	初始化系统时钟
 * @para:	none
 * @return:	none
 * @comment:初始化系统时钟
 */
void InitSystemClock(void)
{
	/*配置DCO为1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*配置SMCLK的时钟源为DCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK的分频系数置为1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:		void InitUART(void)
 * @brief:	初始化串口，包括设置波特率，数据位，校验位等
 * @para:	none
 * @return:	none
 * @comment:初始化串口
 */
void InitUART(void)
{
    /*复位USCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*选择USCI_Ax为UART模式*/
    UCA0CTL0 &= ~UCSYNC;

    /*配置UART时钟源为SMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*配置波特率为9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*使能端口复用*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*清除复位位，使能UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	通过串口发送字符串
 * @para:	pbuff:指向要发送字符串的指针
 * 			num:要发送的字符个数
 * @return:	none
 * @comment:通过串口发送字符串
 */
void UARTSendString(uint8_t *pbuff,uint8_t num)
{
	uint8_t cnt = 0;
	for(cnt = 0;cnt < num;cnt ++)
	{
		while(UCA0STAT & UCBUSY);
		UCA0TXBUF = *(pbuff + cnt);
	}
}
/*
 * @fn:		void PrintNumber(uint16_t num)
 * @brief:	通过串口发送数字
 * @para:	num:变量
 * @return:	none
 * @comment:通过串口发送数字
 */
void PrintNumber(uint16_t num)
{
	uint8_t buff[6] = {0,0,0,0,0,'\n'};
	uint8_t cnt = 0;
	for(cnt = 0;cnt < 5;cnt ++)
	{
		buff[4 - cnt] = (uint8_t)(num % 10 + '0');
		num /= 10;
	}
	UARTSendString(buff,6);
}
/*
 * @fn:		void PrintFloat(float num)
 * @brief:	通过串口发送浮点数
 * @para:	num:变量
 * @return:	none
 * @comment:通过串口发送浮点数，可发送1位整数位+3位小数位
 */
void PrintFloat(float num)
{
	uint8_t charbuff[] = {0,'.',0,0,0};
	uint16_t temp = (uint16_t)(num * 1000);
	charbuff[0] = (uint8_t)(temp / 1000) + '0';
	charbuff[2] = (uint8_t)((temp % 1000) / 100) + '0';
	charbuff[3] = (uint8_t)((temp % 100) / 10) + '0';
	charbuff[4] = (uint8_t)(temp % 10) + '0';
	UARTSendString(charbuff,5);
}
/*
 * @fn:		void InitADC(void)
 * @brief:	初始化ADC
 * @para:	none
 * @return:	none
 * @comment:初始化ADC
 */
void InitADC(void)
{
	  /*设置ADC时钟MCLK*/
	  ADC10CTL1 |= ADC10SSEL_2;
	  /*ADC 2分频*/
	  //ADC10CTL1 |= ADC10DIV_0;
	  /*设置ADC基准源*/
	  ADC10CTL0 |= SREF_1;
	  /*设置ADC采样保持时间16CLK*/
	  ADC10CTL0 |= ADC10SHT_2;
	  /*设置ADC采样率200k*/
	  ADC10CTL0 |= ADC10SR;
	  /*ADC基准选择2.5V*/
	  ADC10CTL0 |= REF2_5V;
	  /*开启基准*/
	  ADC10CTL0 |= REFON;
	  /*选择ADC输入通道A4*/
	  ADC10CTL1 |= INCH_4;
	  /*允许A4模拟输入*/
	  ADC10AE0 |= 1 << 4;

	  /*DTC传输模式*/
	  ADC10DTC0 |= ADC10CT;
	  /*传输次数*/
	  ADC10DTC1 = 50;
	  /*起始地址*/
	  ADC10SA = (uint16_t)(adcbuff);

	  /*开启ADC*/
	  ADC10CTL0 |= ADC10ON;

	  /*允许转换*/
	  ADC10CTL0 |= ENC;
}
/*
 * @fn:		uint16_t GetADCValue(void)
 * @brief:	进行一次ADC转换并返回ADC转换结果
 * @para:	none
 * @return:	ADC转换结果
 * @comment:ADC转换结果为10bit，以uint16_t类型返回，低10位为有效数据
 */
uint16_t GetADCValue(void)
{
	  /*开始转换*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*等待转换完成*/
	  while(ADC10CTL1&ADC10BUSY);
	  /*返回结果*/
	  return ADC10MEM;
}
/*
 * @fn:		void StartADCConvert(void)
 * @brief:	进行一次ADC转换
 * @para:	none
 * @return:	none
 * @comment:ADC转换结果为10bit
 */
void StartADCConvert(void)
{
	  /*开始转换*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*等待转换完成*/
	  while(ADC10CTL1&ADC10BUSY);
}
/*
 * @fn:		uint16_t Max(uint16_t *numptr,uint16_t num)
 * @brief:	查找最大值
 * @para:
 * @return:	最大值
 * @comment:
 */
uint16_t Max(uint16_t *numptr,uint16_t num)
{
	uint16_t cnt = 0;
	uint16_t max = 0;
	for(cnt = 0;cnt < num;cnt ++)
	{
		if(numptr[cnt] > max){
			max = numptr[cnt];
		}
	}
	return max;
}
/*
 * @fn:		uint16_t Min(uint16_t *numptr,uint16_t num)
 * @brief:	查找最小值
 * @para:
 * @return:	最小值
 * @comment:
 */
uint16_t Min(uint16_t *numptr,uint16_t num)
{
	uint16_t cnt = 0;
	uint16_t min = 0;
	min = numptr[0];
	for(cnt = 0;cnt < num;cnt ++)
	{
		if(numptr[cnt] < min){
			min = numptr[cnt];
		}
	}
	return min;
}
/*
 * @fn:		uint16_t Average(uint16_t *datptr)
 * @brief:	计算平均值
 * @para:	none
 * @return:	none
 * @comment:
 */
uint16_t Average(uint16_t *datptr)
{
	uint32_t sum = 0;
	uint8_t cnt = 0;
	for(cnt = 0;cnt < 50;cnt ++)
	{
		sum += *(datptr + cnt);
	}
	return (uint16_t)(sum / 50);
}
/*
 * main.c
 */
int main(void)
{
	uint8_t cnt = 0,cnt1 = 0;
	volatile float fvpp = 0.0f;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitUART();
    InitADC();

    while(1)
    {
    	for(cnt1 = 0;cnt1 < 50;cnt1 ++)
    	{
        	for(cnt = 0;cnt < 50;cnt ++)
        	{
        		StartADCConvert();
        	}
        	maxval[cnt1] = Max(adcbuff,50);
        	minval[cnt1] = Min(adcbuff,50);
    	}
    	max = Max(maxval,50);
    	min = Min(minval,50);
    	vpp = max - min;
    	fvpp = (float)vpp * 2.5 / 1023;
    	fvpp = fvpp;
    	__delay_cycles(1000000);
    }
	return 0;
}
