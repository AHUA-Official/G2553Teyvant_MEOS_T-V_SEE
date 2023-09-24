
#include "ADC10.h"
#include "HT1621.H"

unsigned long Result_sum=0; //存放处理后的ADC10数据值

//ADC10初始化
void adc10_init(void)
{       //参考电压，采样保持时间，
        ADC10CTL0 |= ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
        ADC10CTL0 |= SREF_1 + REF2_5V + REFON;
        ADC10CTL1 |= INCH_6;                         // input A0
        ADC10AE0 |= 0x40;                           // PA.0 ADC option select   
}

//启动一次ADC10转换
void adc10_start(void)
{
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
}

//ADC10采集数据处理
void adc10_data(void)
{

          adc10_start( );
          Result_sum = ADC10MEM;
          
          Result_sum = Result_sum*244; //扩大100000倍
          Result_sum = Result_sum/100;
        //判断ADC10的数值是否达到上限值

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
        
      Ht1621Tab[2]=SEG_7[0][0];       //最左边第一位数码管
      Ht1621Tab[3]=SEG_7[0][1];       //想显示小数点，在此调用SEG_8[R6][1]

      Ht1621Tab[4]=SEG_7[0][0];       //最左边第二位数码管
      Ht1621Tab[5]=SEG_7[0][1];       //想显示小数点，在此调用SEG_8[R5][1]
      
      Ht1621Tab[6]=SEG_8[Result_sum/1000][0];       //最左边第三位数码管
      Ht1621Tab[7]=SEG_8[Result_sum/1000][1];       //想显示小数点，在此调用SEG_8[R4][1]
      
      Ht1621Tab[8]=SEG_7[Result_sum/100%10][0];       //最左边第四位数码管
      Ht1621Tab[9]=SEG_7[Result_sum/100%10][1];       //想显示小数点，在此调用SEG_8[R3][1]
      
      Ht1621Tab[10]=SEG_7[Result_sum%100/10][0];      //最左边第五位数码管
      Ht1621Tab[11]=SEG_7[Result_sum%100/10][1];       //想显示小数点，在此调用SEG_8[R2][1]
      
      Ht1621Tab[12]=SEG_7[Result_sum%10][0];      //最左边第六位数码管
      Ht1621Tab[13]=SEG_7[Result_sum%10][1];     
      
      Ht1621WrAllData(0,Ht1621Tab,16);//写1621寄存器数据，即显示    
}
