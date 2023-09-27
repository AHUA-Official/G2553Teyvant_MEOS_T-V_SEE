//ICC-AVR application builder : 2006-02-12 14:00:00
// Target : ATmega48
// Crystal: 1.000Mhz
// Author:  jackyan

#include "iom48v.h"
//ADC initialize
// Conversion time: 6uS
void InitADC( void)
{
 	ADCSRA = 0x00; //disable adc
 	ADMUX = 0x43; //select adc input 3,AVCC
 	ACSR  = 0x80;	// 
 	ADCSRA = 0x80;	// ADC 功能开启，但是没有进行转换,单次转换，不中断,ADIF=0
 	ADCH=ADCL=0;
}

int ReadADC( unsigned char channel)
{
    int i;

    ADMUX |= channel;                        // Select channel
    
    ADCSRA |= 0x40;                          // Start conversion
    
    while (!(ADCSRA & 0x10));                // Check if converstion is ready
    
    ADCSRA |= 0x10;                          // Clear Conversion ready flag by setting the bit

    i = ADCL;                               // Read 8 low bits first (important)
    i += (int)ADCH << 8;                    // Read 2 high bits and multiply with 256

    return i;
}

