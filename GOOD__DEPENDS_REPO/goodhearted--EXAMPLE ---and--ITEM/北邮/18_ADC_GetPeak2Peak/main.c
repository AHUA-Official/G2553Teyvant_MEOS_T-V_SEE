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
 * @brief:	��ʼ��ϵͳʱ��
 * @para:	none
 * @return:	none
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
 * @fn:		void InitUART(void)
 * @brief:	��ʼ�����ڣ��������ò����ʣ�����λ��У��λ��
 * @para:	none
 * @return:	none
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
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	ͨ�����ڷ����ַ���
 * @para:	pbuff:ָ��Ҫ�����ַ�����ָ��
 * 			num:Ҫ���͵��ַ�����
 * @return:	none
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
 * @fn:		void PrintNumber(uint16_t num)
 * @brief:	ͨ�����ڷ�������
 * @para:	num:����
 * @return:	none
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
 * @fn:		void PrintFloat(float num)
 * @brief:	ͨ�����ڷ��͸�����
 * @para:	num:����
 * @return:	none
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
/*
 * @fn:		void InitADC(void)
 * @brief:	��ʼ��ADC
 * @para:	none
 * @return:	none
 * @comment:��ʼ��ADC
 */
void InitADC(void)
{
	  /*����ADCʱ��MCLK*/
	  ADC10CTL1 |= ADC10SSEL_2;
	  /*ADC 2��Ƶ*/
	  //ADC10CTL1 |= ADC10DIV_0;
	  /*����ADC��׼Դ*/
	  ADC10CTL0 |= SREF_1;
	  /*����ADC��������ʱ��16CLK*/
	  ADC10CTL0 |= ADC10SHT_2;
	  /*����ADC������200k*/
	  ADC10CTL0 |= ADC10SR;
	  /*ADC��׼ѡ��2.5V*/
	  ADC10CTL0 |= REF2_5V;
	  /*������׼*/
	  ADC10CTL0 |= REFON;
	  /*ѡ��ADC����ͨ��A4*/
	  ADC10CTL1 |= INCH_4;
	  /*����A4ģ������*/
	  ADC10AE0 |= 1 << 4;

	  /*DTC����ģʽ*/
	  ADC10DTC0 |= ADC10CT;
	  /*�������*/
	  ADC10DTC1 = 50;
	  /*��ʼ��ַ*/
	  ADC10SA = (uint16_t)(adcbuff);

	  /*����ADC*/
	  ADC10CTL0 |= ADC10ON;

	  /*����ת��*/
	  ADC10CTL0 |= ENC;
}
/*
 * @fn:		uint16_t GetADCValue(void)
 * @brief:	����һ��ADCת��������ADCת�����
 * @para:	none
 * @return:	ADCת�����
 * @comment:ADCת�����Ϊ10bit����uint16_t���ͷ��أ���10λΪ��Ч����
 */
uint16_t GetADCValue(void)
{
	  /*��ʼת��*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*�ȴ�ת�����*/
	  while(ADC10CTL1&ADC10BUSY);
	  /*���ؽ��*/
	  return ADC10MEM;
}
/*
 * @fn:		void StartADCConvert(void)
 * @brief:	����һ��ADCת��
 * @para:	none
 * @return:	none
 * @comment:ADCת�����Ϊ10bit
 */
void StartADCConvert(void)
{
	  /*��ʼת��*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*�ȴ�ת�����*/
	  while(ADC10CTL1&ADC10BUSY);
}
/*
 * @fn:		uint16_t Max(uint16_t *numptr,uint16_t num)
 * @brief:	�������ֵ
 * @para:
 * @return:	���ֵ
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
 * @brief:	������Сֵ
 * @para:
 * @return:	��Сֵ
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
 * @brief:	����ƽ��ֵ
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
