#include  "msp430g2553.h"

#define uint   unsigned int 
#define uchar  unsigned char

//������74HC138����ĺ궨��
#define LSA_H     P2OUT |= BIT0
#define LSA_L     P2OUT &= ~ BIT0
#define LSB_H     P2OUT |= BIT1
#define LSB_L     P2OUT &= ~ BIT1
#define LSC_H     P2OUT |= BIT2
#define LSC_L     P2OUT &= ~ BIT2

uchar const LedData[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                        0x7f,0x6f,0x77,0x7C,0x39,0x5E,0x79,0x71};//��������ܶ�ѡ����(0-F��
uchar DisplayData[8];


long temp;
long IntDegF;
long IntDegC;

void DigDisplay( );


void main(void)
{
 
  P1DIR |= 0XFF;
  P2DIR |= BIT0 + BIT1 +BIT2;
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL1 = INCH_10 + SHS_0 + ADC10DIV_3;         // Temp  mSensor ADC10CLK/4
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
  /*__enable_interrupt();                     // Enable interrupts.
  TACCR0 = 30;                              // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  LPM0;                                     // Wait for delay.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  __disable_interrupt();                    //��ֹȫ���ж�*/

  while(1)
  {
      
      ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
      __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
                       // == LPM0();                   //�ر�CPU ���ж� �͹���ģʽ1
      // oF = ((A10/1024)*1500mV)-923mV)*1/1.97mV = A10*761/1024 - 468
      //temp = ADC10MEM;
      //IntDegF = ((temp - 630) * 761) / 1024;

      // oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
      temp = ADC10MEM;
      IntDegC = ((temp - 673) * 423) / 1024;

      //__no_operation();                       // SET BREAKPOINT HERE
    
    DisplayData[0] = LedData[(IntDegC/1000)];
    DisplayData[1] = LedData[(IntDegC%1000/100)];
    DisplayData[2] = LedData[(IntDegC%1000%100/10)];
    DisplayData[3] = LedData[(IntDegC%1000%100%10)];
    DigDisplay( );
    
  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}//�˳�LPM0();  LPM0_EXIT;  


/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ʹ���������ʾ
* ��    ��         : ��     74LS138��̬��ʾ
* ��    ��         : ��
*******************************************************************************/
void DigDisplay( )
{
	uchar i;
	uint  j;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA_L;LSB_L;LSC_L; break;//��ʾ��0λ
			case(1):
				LSA_H;LSB_L;LSC_L; break;//��ʾ��1λ
			case(2):
				LSA_L;LSB_H;LSC_L; break;//��ʾ��2λ
			case(3):
				LSA_H;LSB_H;LSC_L; break;//��ʾ��3λ
			case(4):
				LSA_L;LSB_L;LSC_H; break;//��ʾ��4λ
			case(5):
				LSA_H;LSB_L;LSC_H; break;//��ʾ��5λ
			case(6):
				LSA_L;LSB_H;LSC_H; break;//��ʾ��6λ
			case(7):
				LSA_H;LSB_H;LSC_H; break;//��ʾ��7λ	
		}
		P1OUT = DisplayData[i];//���Ͷ���
		j=250;						 //ɨ����ʱ���趨
		while(j--);	
		P1OUT = 0x00;//����
	}
}

