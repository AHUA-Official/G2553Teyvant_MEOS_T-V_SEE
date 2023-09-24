
#include "ADC10.h"
#include "HT1621.H"

unsigned long Result_sum=0; //��Ŵ�����ADC10����ֵ

//ADC10��ʼ��
void adc10_init(void)
{       //�ο���ѹ����������ʱ�䣬
        ADC10CTL0 |= ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
        ADC10CTL0 |= SREF_1 + REF2_5V + REFON;
        ADC10CTL1 |= INCH_6;                         // input A0
        ADC10AE0 |= 0x40;                           // PA.0 ADC option select   
}

//����һ��ADC10ת��
void adc10_start(void)
{
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
}

//ADC10�ɼ����ݴ���
void adc10_data(void)
{

          adc10_start( );
          Result_sum = ADC10MEM;
          
          Result_sum = Result_sum*244; //����100000��
          Result_sum = Result_sum/100;
        //�ж�ADC10����ֵ�Ƿ�ﵽ����ֵ

}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}


void display_adc10(void)
{     
      adc10_data( );
        
      Ht1621Tab[2]=SEG_7[0][0];       //����ߵ�һλ�����
      Ht1621Tab[3]=SEG_7[0][1];       //����ʾС���㣬�ڴ˵���SEG_8[R6][1]

      Ht1621Tab[4]=SEG_7[0][0];       //����ߵڶ�λ�����
      Ht1621Tab[5]=SEG_7[0][1];       //����ʾС���㣬�ڴ˵���SEG_8[R5][1]
      
      Ht1621Tab[6]=SEG_8[Result_sum/1000][0];       //����ߵ���λ�����
      Ht1621Tab[7]=SEG_8[Result_sum/1000][1];       //����ʾС���㣬�ڴ˵���SEG_8[R4][1]
      
      Ht1621Tab[8]=SEG_7[Result_sum/100%10][0];       //����ߵ���λ�����
      Ht1621Tab[9]=SEG_7[Result_sum/100%10][1];       //����ʾС���㣬�ڴ˵���SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[Result_sum%100/10][0];      //����ߵ���λ�����
      Ht1621Tab[11]=SEG_7[Result_sum%100/10][1];       //����ʾС���㣬�ڴ˵���SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[Result_sum%10][0];      //����ߵ���λ�����
      Ht1621Tab[13]=SEG_7[Result_sum%10][1];     
      
      Ht1621WrAllData(0,Ht1621Tab,16);//д1621�Ĵ������ݣ�����ʾ    
}
