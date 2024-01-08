#include <msp430.h>
#include "stdint.h"


volatile int adcMode = 0; // ��ʼADCģʽΪ0

/*
 * @fn:     void InitSystemClock(void)
 * @brief:  ��ʼ��ϵͳʱ��
 * @para:   none
 * @return: none
 * @comment:��ʼ��ϵͳʱ��
 */
void InitSystemClock(void)
{
    /*����DCOΪ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*����SMCLK��ʱ��ԴΪDCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK�ķ�Ƶϵ����Ϊ1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:     void InitUART(void)
 * @brief:  ��ʼ�����ڣ��������ò����ʣ�����λ��У��λ��
 * @para:   none
 * @return: none
 * @comment:��ʼ������
 */
void InitUART(void)
{
    /*��λUSCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*ѡ��USCI_AxΪUARTģʽ*/
    UCA0CTL0 &= ~UCSYNC;

    /*����UARTʱ��ԴΪSMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*���ò�����Ϊ9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*ʹ�ܶ˿ڸ���*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*�����λλ��ʹ��UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:     void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:  ͨ�����ڷ����ַ���
 * @para:   pbuff:ָ��Ҫ�����ַ�����ָ��
 *          num:Ҫ���͵��ַ�����
 * @return: none
 * @comment:ͨ�����ڷ����ַ���
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
 * @brief:  ͨ�����ڷ�������
 * @para:   num:����
 * @return: none
 * @comment:ͨ�����ڷ�������
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
 * @brief:  ͨ�����ڷ��͸�����
 * @para:   num:����
 * @return: none
 * @comment:ͨ�����ڷ��͸��������ɷ���1λ����λ+3λС��λ
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
      /*����ADCʱ��MCLK*/
      ADC10CTL1 |= ADC10SSEL_2;
      /*ADC 2��Ƶ*/
      ADC10CTL1 |= ADC10DIV_0;
      /*����ADC��׼Դ*/
      ADC10CTL0 |= SREF_1;
      /*����ADC��������ʱ��64CLK*/
      ADC10CTL0 |= ADC10SHT_3;
      /*����ADC������200k*/
      ADC10CTL0 &= ~ADC10SR;
      /*ADC��׼ѡ��2.5V*/
      ADC10CTL0 |= REF2_5V;
      /*������׼*/
      ADC10CTL0 |= REFON;
}

uint16_t GetADCValue(void)
{
      /*��ʼת��*/
      ADC10CTL0 |= ADC10SC|ENC;
      /*�ȴ�ת�����*/
      while(ADC10CTL1&ADC10BUSY);
      /*���ؽ��*/
      return ADC10MEM;
}


void Mea_T(void){
    /*ѡ��ADC����ͨ��A0*/
    ADC10CTL1 |= INCH_10;
    /*����A0ģ������*/
    ADC10AE0 |= 0x0001;
    /*����ADC*/
    ADC10CTL0 |= ADC10ON;
    uint16_t adcvalue = GetADCValue();
    UARTSendString("ADC10ת�ӽ��Ϊ��",17);
    PrintNumber(adcvalue);
    float voltage = (adcvalue/3.5)*0.28;
    UARTSendString("��Ӧ�¶�ֵΪ��",14);
    PrintFloat_T(voltage);
}


void Mea_V(void){
    /*ѡ��ADC����ͨ��A0*/
    ADC10CTL1 |= INCH_0;
    /*����A0ģ������*/
    ADC10AE0 |= 0x0001;
    /*����ADC*/
    ADC10CTL0 |= ADC10ON;
    uint16_t adcvalue = GetADCValue();
    UARTSendString("ADC10ת�ӽ��Ϊ��",17);
    PrintNumber(adcvalue);
    float voltage = (adcvalue * 2.5 / 1023)*1.25;
    UARTSendString("��Ӧ��ѹֵΪ��",14);
    PrintFloat(voltage);
}


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

     /* ��ʼ��LED2Ϊ��� */
        P1DIR |= BIT6;

        /* ��ʼ��LED2Ϊ�͵�ƽ��Ϩ�� */
        P1OUT &= ~BIT6;

        /* ��ʼ��KEY P1.3Ϊ���� */
        P1DIR &= ~BIT3;
        /* ʹ��P1.3�ڵ��������� */
        P1REN |= BIT3;
        P1OUT |= BIT3;

        /* ��P1.3�ڵ��ж� */
        P1IE |= BIT3;
        /* �趨Ϊ�½��ش��� */
        P1IES |= BIT3;
        /* ����жϱ�־λ */
        P1IFG &= ~BIT3;

        /* ��ȫ���ж� */
        __bis_SR_register(GIE);

        InitSystemClock();
        InitUART();
        InitADC();

        while(1)
         {
             if (adcMode == 1)
             {
                 Mea_V(); // ����ADC_Vģʽ
             }
             else if (adcMode == 2)
             {
                 Mea_T();// ����ADC_Tģʽ
             }
             // �������������
             __delay_cycles(300000);
         }
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)    //Port1_ISR(void)   �жϷ�����
{
    if(P1IFG & BIT3)   //�ж��Ƿ�P1.3�����ж�
    {
        __bic_SR_register(GIE);  // �����ж�
        P1OUT ^= BIT6;
        P1IFG  &= ~ BIT3;   //�����־λ
        // �л�ADCģʽ��1��2��
        adcMode++;
        if (adcMode > 2)
        {
            adcMode = 0;
        }
        __bis_SR_register(GIE);  // �ָ��ж�
    }
}
