#include <msp430.h>
#include "stdint.h"


volatile int adcMode = 0; // 初始ADC模式为0

/*
 * @fn:     void InitSystemClock(void)
 * @brief:  初始化系统时钟
 * @para:   none
 * @return: none
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
 * @fn:     void InitUART(void)
 * @brief:  初始化串口，包括设置波特率，数据位，校验位等
 * @para:   none
 * @return: none
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
 * @fn:     void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:  通过串口发送字符串
 * @para:   pbuff:指向要发送字符串的指针
 *          num:要发送的字符个数
 * @return: none
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
 * @fn:     void PrintNumber(uint16_t num)
 * @brief:  通过串口发送数字
 * @para:   num:变量
 * @return: none
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
 * @fn:     void PrintFloat(float num)
 * @brief:  通过串口发送浮点数
 * @para:   num:变量
 * @return: none
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

void PrintFloat_T(float num)
{
    uint8_t charbuff[] = {0,0,'.',0,0};
    uint16_t temp = (uint16_t)(num * 100);
    charbuff[0] = (uint8_t)(temp / 1000) + '0';
    charbuff[1] = (uint8_t)((temp % 1000) / 100) + '0';
    charbuff[3] = (uint8_t)((temp % 100) / 10) + '0';
    charbuff[4] = (uint8_t)(temp % 10) + '0';
    UARTSendString(charbuff,5);
}

void InitADC(void)
{
      /*设置ADC时钟MCLK*/
      ADC10CTL1 |= ADC10SSEL_2;
      /*ADC 2分频*/
      ADC10CTL1 |= ADC10DIV_0;
      /*设置ADC基准源*/
      ADC10CTL0 |= SREF_1;
      /*设置ADC采样保持时间64CLK*/
      ADC10CTL0 |= ADC10SHT_3;
      /*设置ADC采样率200k*/
      ADC10CTL0 &= ~ADC10SR;
      /*ADC基准选择2.5V*/
      ADC10CTL0 |= REF2_5V;
      /*开启基准*/
      ADC10CTL0 |= REFON;
}

uint16_t GetADCValue(void)
{
      /*开始转换*/
      ADC10CTL0 |= ADC10SC|ENC;
      /*等待转换完成*/
      while(ADC10CTL1&ADC10BUSY);
      /*返回结果*/
      return ADC10MEM;
}


void Mea_T(void){
    /*选择ADC输入通道A0*/
    ADC10CTL1 |= INCH_10;
    /*允许A0模拟输入*/
    ADC10AE0 |= 0x0001;
    /*开启ADC*/
    ADC10CTL0 |= ADC10ON;
    uint16_t adcvalue = GetADCValue();
    UARTSendString("ADC10转接结果为：",17);
    PrintNumber(adcvalue);
    float voltage = (adcvalue/3.5)*0.28;
    UARTSendString("相应温度值为：",14);
    PrintFloat_T(voltage);
}


void Mea_V(void){
    /*选择ADC输入通道A0*/
    ADC10CTL1 |= INCH_0;
    /*允许A0模拟输入*/
    ADC10AE0 |= 0x0001;
    /*开启ADC*/
    ADC10CTL0 |= ADC10ON;
    uint16_t adcvalue = GetADCValue();
    UARTSendString("ADC10转接结果为：",17);
    PrintNumber(adcvalue);
    float voltage = (adcvalue * 2.5 / 1023)*1.25;
    UARTSendString("相应电压值为：",14);
    PrintFloat(voltage);
}


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

     /* 初始化LED2为输出 */
        P1DIR |= BIT6;

        /* 初始化LED2为低电平，熄灭 */
        P1OUT &= ~BIT6;

        /* 初始化KEY P1.3为输入 */
        P1DIR &= ~BIT3;
        /* 使能P1.3口的上拉电阻 */
        P1REN |= BIT3;
        P1OUT |= BIT3;

        /* 打开P1.3口的中断 */
        P1IE |= BIT3;
        /* 设定为下降沿触发 */
        P1IES |= BIT3;
        /* 清除中断标志位 */
        P1IFG &= ~BIT3;

        /* 打开全局中断 */
        __bis_SR_register(GIE);

        InitSystemClock();
        InitUART();
        InitADC();

        while(1)
         {
             if (adcMode == 1)
             {
                 Mea_V(); // 启动ADC_V模式
             }
             else if (adcMode == 2)
             {
                 Mea_T();// 启动ADC_T模式
             }
             // 程序的其他部分
             __delay_cycles(300000);
         }
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)    //Port1_ISR(void)   中断服务函数
{
    if(P1IFG & BIT3)   //判断是否P1.3产生中断
    {
        __bic_SR_register(GIE);  // 禁用中断
        P1OUT ^= BIT6;
        P1IFG  &= ~ BIT3;   //清除标志位
        // 切换ADC模式（1或2）
        adcMode++;
        if (adcMode > 2)
        {
            adcMode = 0;
        }
        __bis_SR_register(GIE);  // 恢复中断
    }
}
