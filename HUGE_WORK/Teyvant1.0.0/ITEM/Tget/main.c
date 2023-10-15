#include <msp430.h>
#include "stdint.h"

void InitSystemClock(void)
{
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    BCSCTL2 &= ~SELS;
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}

void InitUART(void)
{
    UCA0CTL1 |= UCSWRST;
    UCA0CTL0 &= ~UCSYNC;
    UCA0CTL1 |= UCSSEL1;
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 &= ~UCSWRST;
}

void UARTSendString(uint8_t *pbuff, uint8_t num)
{
    uint8_t cnt = 0;
    for (cnt = 0; cnt < num; cnt++)
    {
        while (UCA0STAT & UCBUSY);
        UCA0TXBUF = *(pbuff + cnt);
    }
}

void PrintNumber(uint16_t num)
{
    uint8_t buff[6] = {0, 0, 0, 0, 0, '\n'};
    uint8_t cnt = 0;
    for (cnt = 0; cnt < 5; cnt++)
    {
        buff[4 - cnt] = (uint8_t)(num % 10 + '0');
        num /= 10;
    }
    UARTSendString(buff, 6);
}

void PrintFloat(float num)
{
    uint8_t charbuff[] = {0, '.', 0, 0, 0};
    uint16_t temp = (uint16_t)(num * 1000);
    charbuff[0] = (uint8_t)(temp / 1000) + '0';
    charbuff[2] = (uint8_t)((temp % 1000) / 100) + '0';
    charbuff[3] = (uint8_t)((temp % 100) / 10) + '0';
    charbuff[4] = (uint8_t)(temp % 10) + '0';
    UARTSendString(charbuff, 5);
}

void InitADC(void)
{
    ADC10CTL1 |= ADC10SSEL_2;
    ADC10CTL1 |= ADC10DIV_0;
    ADC10CTL0 |= SREF_1;
    ADC10CTL0 |= ADC10SHT_3;
    ADC10CTL0 &= ~ADC10SR;
    ADC10CTL0 |= REF2_5V;
    ADC10CTL0 |= REFON;
    ADC10CTL1 |= INCH_0;
    ADC10AE0 |= 0x0001;
    ADC10CTL0 |= ADC10ON;
}

uint16_t GetADCValue(void)
{
    ADC10CTL0 |= ADC10SC | ENC;
    while (ADC10CTL1 & ADC10BUSY);
    return ADC10MEM;
}

void main(void)
{
    float temperature = 0;
    uint16_t adcvalue = 0;
    WDTCTL = WDTPW | WDTHOLD;
    InitSystemClock();
    InitUART();
    InitADC();
    while (1)
    {
        adcvalue = GetADCValue();
        temperature = (adcvalue * 0.01) - 50.0;  // Assuming specific scaling for temperature
        UARTSendString("Temperature (C): ", 18);
        PrintFloat(temperature);
        __delay_cycles(300000);
    }
}
